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
#include "timeServer.h"
#include "version.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BUTTON_DUTYCYCLE                   1000
#define LED_RDY_DUTYCYCLE                  1000

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void OnButtonTimerEvent( void );
static void OnLedTimerEvent( void );

/* Private variables ---------------------------------------------------------*/
static TimerEvent_t ButtonTimer;
static TimerEvent_t LedTimer;
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


