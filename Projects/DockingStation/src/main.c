/** 
  * @file    main.c
  * @author  Duwon Jung
  * @date    2018-11-08
  * @brif    Main
  */

/**
  * @mainpage DockingStation 
    @section intro 소개
    - LorySDK, Lorawan Class-C project

  * @section info 정보
    - DockingStaion에 Lorawan 포팅

  * @section  CREATEINFO      작성정보
    - 작성자      :   Duwon Jung
    - 작성일      :   2018-11-08

  * @section  MODIFYINFO      수정정보
    - 2018-11-08    :    일부 수정
  
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

#define APP_TX_DUTYCYCLE                            30000 /**< data transmission duty cycle. 60s, value in [ms]. */
#define LORAWAN_ADR_STATE                           LORAWAN_ADR_ON
#define LORAWAN_DEFAULT_DATA_RATE                   DR_0 /**< LoRaWAN Default data Rate Data Rate */
#define LORAWAN_APP_PORT                            2 /**< LoRaWAN application port. do not use 224. */
#define LORAWAN_DEFAULT_CLASS                       CLASS_C /**< LoRaWAN default endNode class */
#define LORAWAN_DEFAULT_CONFIRM_MSG_STATE           LORAWAN_UNCONFIRMED_MSG /**< LoRaWAN default confirm state */
#define LORAWAN_APP_DATA_BUFF_SIZE                  64 /**< User application data buffer size */
static uint8_t AppDataBuff[LORAWAN_APP_DATA_BUFF_SIZE]; /**< User application data */
static lora_AppData_t AppData={ AppDataBuff,  0 ,0 }; /**< User application data structure */

#define BUTTON_DUTYCYCLE                   3000
#define LED_RDY_DUTYCYCLE                  1000

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void OnButtonTimerEvent( void ); /**< Button timer callback function */
static void OnLedTimerEvent( void ); /**< LED timer callback function */
static void OnTxTimerEvent( void ); /**< TX timer callback function */

static void LORA_RxData( lora_AppData_t *AppData); /**< call back when LoRa endNode has received a frame */
static void LORA_HasJoined( void ); /**< call back when LoRa endNode has just joined */
static void LORA_ConfirmClass ( DeviceClass_t Class ); /**< call back when LoRa endNode has just switch the class */
static void LORA_TxNeeded ( void ); /**< call back when server needs endNode to send a frame */
static void Send( void ); /**< LoRa endNode send request */

uint8_t HW_GetBatteryLevel( void );
uint16_t HW_GetTemperatureLevel( void );

/* Private variables ---------------------------------------------------------*/
static TimerEvent_t ButtonTimer;
static TimerEvent_t LedTimer;
static TimerEvent_t TxTimer;

static uint8_t AppLedStateOn = RESET; /**<Specifies the state of the application LED */

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
	
	/* Configure the Lora Stack*/
  LORA_Init( &LoRaMainCallbacks, &LoRaParamInit);
  
  PRINTF("VERSION: %X\n\r", VERSION);
  
  LORA_Join();

	TimerInit( &ButtonTimer, OnButtonTimerEvent );
	TimerSetValue( &ButtonTimer,  BUTTON_DUTYCYCLE); 
	OnButtonTimerEvent();
	
	TimerInit( &LedTimer, OnLedTimerEvent );
	TimerSetValue( &LedTimer, LED_RDY_DUTYCYCLE); 
	OnLedTimerEvent();
	
	TimerInit( &TxTimer, OnTxTimerEvent );
	TimerSetValue( &TxTimer,  APP_TX_DUTYCYCLE); 
	OnTxTimerEvent();	
	
  while(1)
  {

  }

}

static void OnButtonTimerEvent( void )
{
	TimerStart( &ButtonTimer);
	if(BSP_PB_GetState(BUTTON_USER)==GPIO_PIN_RESET) NVIC_SystemReset();
}

static void OnLedTimerEvent( void )
{
	TimerStart( &LedTimer);
	LED_Toggle(LED_RDY);
}

static void LORA_HasJoined( void )
{
  PRINTF("%d JOINED\n\r", HW_RTC_GetTimerValue() );
  LORA_RequestClass( LORAWAN_DEFAULT_CLASS );
}

static void Send( void )
{
  uint16_t pressure = 900;
  int16_t temperature = 30;
  uint16_t humidity = 80;
  
  if ( LORA_JoinStatus () != LORA_SET) /*Not joined, try again later*/
  {
    LORA_Join();
    return;
  }

  uint8_t cchannel=0;
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

  PRINTF("%d PACKET RECEIVED ON PORT %d\n\r", HW_RTC_GetTimerValue(), AppData->Port);

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
					PRINTF("%d LED OFF\n\r", HW_RTC_GetTimerValue() );
					TimerStop(&LedTimer);
				}
				else
				{
					PRINTF("%d LED TOGGLE ON\n\r", HW_RTC_GetTimerValue() );
					OnLedTimerEvent();
				}
			}
    default:
			break;
  }
}

static void OnTxTimerEvent( void )
{
  TimerStart( &TxTimer);
	Send( );  
}

static void LORA_ConfirmClass ( DeviceClass_t Class )
{
  PRINTF("%d switch to class %c done\n\r", HW_RTC_GetTimerValue(),"ABC"[Class] );

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
	PRINTF("%d LORA_TxNeeded", HW_RTC_GetTimerValue() );
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


