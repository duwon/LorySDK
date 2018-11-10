/******************************************************************************
  * @file    main.c
  * @author  Duwon Jung
  * @version 
  * @date    2018-11-08
  * @brief   
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "lora.h"
#include "timeServer.h"
#include "version.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LPP_DATATYPE_DIGITAL_INPUT     0x0
#define LPP_DATATYPE_DIGITAL_OUTPUT    0x1
#define LPP_DATATYPE_HUMIDITY          0x68
#define LPP_DATATYPE_TEMPERATURE       0x67
#define LPP_DATATYPE_BAROMETER         0x73
#define LPP_APP_PORT                   99

#define APP_TX_DUTYCYCLE                            4000 //data transmission duty cycle. 5s, value in [ms].
#define LORAWAN_ADR_STATE                           LORAWAN_ADR_ON
#define LORAWAN_DEFAULT_DATA_RATE                   DR_0 //LoRaWAN Default data Rate Data Rate
#define LORAWAN_APP_PORT                            2 //LoRaWAN application port. do not use 224.
#define JOINREQ_NBTRIALS                            3 //Number of trials for the join request.
#define LORAWAN_DEFAULT_CLASS                       CLASS_C //LoRaWAN default endNode class port
#define LORAWAN_DEFAULT_CONFIRM_MSG_STATE           LORAWAN_UNCONFIRMED_MSG //LoRaWAN default confirm state
#define LORAWAN_APP_DATA_BUFF_SIZE                  64 //User application data buffer size
static uint8_t AppDataBuff[LORAWAN_APP_DATA_BUFF_SIZE]; //User application data
static lora_AppData_t AppData={ AppDataBuff,  0 ,0 }; //User application data structure

#define BUTTON_DUTYCYCLE                   1000
#define LED_RDY_DUTYCYCLE                  1000

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void OnButtonTimerEvent( void );
static void OnLedTimerEvent( void );
static void OnTxTimerEvent( void ); /* tx timer callback function*/

static void LORA_RxData( lora_AppData_t *AppData); /* call back when LoRa endNode has received a frame*/
static void LORA_HasJoined( void ); /* call back when LoRa endNode has just joined*/
static void LORA_ConfirmClass ( DeviceClass_t Class ); /* call back when LoRa endNode has just switch the class*/
static void LORA_TxNeeded ( void ); /* call back when server needs endNode to send a frame*/
static void Send( void ); /* LoRa endNode send request*/
static void LoraStartTx(TxEventType_t EventType); /* start the tx process*/

uint8_t HW_GetBatteryLevel( void );
uint16_t HW_GetTemperatureLevel( void );

/* Private variables ---------------------------------------------------------*/
static TimerEvent_t ButtonTimer;
static TimerEvent_t LedTimer;
static TimerEvent_t TxTimer;

static uint8_t AppLedStateOn = RESET; //Specifies the state of the application LED

/* load Main call backs structure*/
static LoRaMainCallback_t LoRaMainCallbacks = { HW_GetBatteryLevel,
                                                HW_GetTemperatureLevel,
                                                HW_GetUniqueId,
                                                HW_GetRandomSeed,
                                                LORA_RxData,
                                                LORA_HasJoined,
                                                LORA_ConfirmClass,
                                                LORA_TxNeeded};

static  LoRaParam_t LoRaParamInit= {LORAWAN_ADR_STATE,
                                    LORAWAN_DEFAULT_DATA_RATE,  
                                    LORAWAN_PUBLIC_NETWORK};
																						 
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{

  HAL_Init( ); // STM32 HAL library initialization
  SystemClock_Config( ); // Configure the system clock
	
	HW_Init();
	
	TimerInit( &ButtonTimer, OnButtonTimerEvent );
	TimerSetValue( &ButtonTimer,  BUTTON_DUTYCYCLE); 
	OnButtonTimerEvent();
	
	TimerInit( &LedTimer, OnLedTimerEvent );
	TimerSetValue( &LedTimer, LED_RDY_DUTYCYCLE); 
	OnLedTimerEvent();

	  /* Configure the Lora Stack*/
  LORA_Init( &LoRaMainCallbacks, &LoRaParamInit);
  
  PRINTF("VERSION: %X\n\r", VERSION);
  
  LORA_Join();
  
  LoraStartTx( TX_ON_TIMER) ;
	
  while(1)
  {

  }

}

static void OnButtonTimerEvent( void )
{
	TimerStart( &ButtonTimer);
	
}

static void OnLedTimerEvent( void )
{
	TimerStart( &LedTimer);
	LED_Toggle(LED_RDY);
}

static void LORA_HasJoined( void )
{
#if( OVER_THE_AIR_ACTIVATION != 0 )
  PRINTF("JOINED\n\r");
#endif
  LORA_RequestClass( LORAWAN_DEFAULT_CLASS );
}

static void Send( void )
{
  /* USER CODE BEGIN 3 */
  uint16_t pressure = 0;
  int16_t temperature = 0;
  uint16_t humidity = 0;
  
  if ( LORA_JoinStatus () != LORA_SET)
  {
    /*Not joined, try again later*/
    LORA_Join();
    return;
  }
  
  DBG_PRINTF("SEND REQUEST\n\r");

  uint8_t cchannel=0;
  temperature = HW_GetTemperatureLevel();
  pressure    = 900;  
  humidity    = 80;  
  uint32_t i = 0;

  AppData.Port = LPP_APP_PORT;

  AppData.Buff[i++] = cchannel++;
  AppData.Buff[i++] = LPP_DATATYPE_BAROMETER;
  AppData.Buff[i++] = ( pressure >> 8 ) & 0xFF;
  AppData.Buff[i++] = pressure & 0xFF;
  AppData.Buff[i++] = cchannel++;
  AppData.Buff[i++] = LPP_DATATYPE_TEMPERATURE; 
  AppData.Buff[i++] = ( temperature >> 8 ) & 0xFF;
  AppData.Buff[i++] = temperature & 0xFF;
  AppData.Buff[i++] = cchannel++;
  AppData.Buff[i++] = LPP_DATATYPE_HUMIDITY;
  AppData.Buff[i++] = humidity & 0xFF;

  AppData.BuffSize = i;
  
  LORA_send( &AppData, LORAWAN_DEFAULT_CONFIRM_MSG_STATE);
  
}


static void LORA_RxData( lora_AppData_t *AppData )
{

  DBG_PRINTF("PACKET RECEIVED ON PORT %d\n\r", AppData->Port);

  switch (AppData->Port)
  {
    case 3:
			/*this port switches the class*/
			if( AppData->BuffSize == 1 )
			{
				switch (  AppData->Buff[0] )
				{
					case 0:
					{
						LORA_RequestClass(CLASS_A);
						break;
					}
					case 1:
					{
						LORA_RequestClass(CLASS_B);
						break;
					}
					case 2:
					{
						LORA_RequestClass(CLASS_C);
						break;
					}
					default:
						break;
				}
			}
			break;
    case LORAWAN_APP_PORT:
			if( AppData->BuffSize == 1 )
			{
				AppLedStateOn = AppData->Buff[0] & 0x01;
				if ( AppLedStateOn == RESET )
				{
					PRINTF("LED OFF\n\r");
					LED_Off( LED_RDY ) ; 
				}
				else
				{
					PRINTF("LED ON\n\r");
					LED_On( LED_RDY ) ; 
				}
			}
    default:
			break;
  }
}

static void OnTxTimerEvent( void )
{
  Send( );
  /*Wait for next tx slot*/
  TimerStart( &TxTimer);
}

static void LoraStartTx(TxEventType_t EventType)
{
  if (EventType == TX_ON_TIMER)
  {
    /* send everytime timer elapses */
    TimerInit( &TxTimer, OnTxTimerEvent );
    TimerSetValue( &TxTimer,  APP_TX_DUTYCYCLE); 
    OnTxTimerEvent();
  }
  else
  {
    /* send everytime button is pushed */
    GPIO_InitTypeDef initStruct={0};
  
    initStruct.Mode =GPIO_MODE_IT_RISING;
    initStruct.Pull = GPIO_PULLUP;
    initStruct.Speed = GPIO_SPEED_HIGH;

    HW_GPIO_Init( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, &initStruct );
    HW_GPIO_SetIrq( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, 0, Send );
  }
}

static void LORA_ConfirmClass ( DeviceClass_t Class )
{
  PRINTF("switch to class %c done\n\r","ABC"[Class] );

  /*Optionnal*/
  /*informs the server that switch has occurred ASAP*/
  AppData.BuffSize = 0;
  AppData.Port = LORAWAN_APP_PORT;
  
  LORA_send( &AppData, LORAWAN_UNCONFIRMED_MSG);
}

static void LORA_TxNeeded ( void )
{
  AppData.BuffSize = 0;
  AppData.Port = LORAWAN_APP_PORT;
  
  LORA_send( &AppData, LORAWAN_UNCONFIRMED_MSG);
}

/**
  * @brief This function return the battery level
  * @param none
  * @retval the battery level  1 (very low) to 254 (fully charged)
  */
uint8_t HW_GetBatteryLevel( void ) 
{
  uint8_t batteryLevel = 100;
  return batteryLevel;
}

uint16_t HW_GetTemperatureLevel( void ) 
{

  uint16_t temperatureDegreeC = 30;
  
  return (uint16_t) temperatureDegreeC;
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
	PRINTF("ERROR");
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 

}
#endif /* USE_FULL_ASSERT */


