/**
  ******************************************************************************
  * @file    LorySDK_hw_conf.h
  * @brief   
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONF_L1_H__
#define __HW_CONF_L1_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
	 
/* Exported types ------------------------------------------------------------*/
	 
typedef enum 
{
  LED1 = 0,
  LED_RDY = LED1,
  LED2 = 1,
  LED_LINK =LED2,
  LED3 = 2,
  LED_LOW =LED3,
  LED4 = 3,
  LED_MID =LED4,
  LED5 = 4,
  LED_HIGH =LED5	
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef; 

	 
/* Exported constants --------------------------------------------------------*/

#define RADIO_DIO_4

/* LORA I/O definition */
#define RADIO_RESET_PORT                          GPIOC
#define RADIO_RESET_PIN                           GPIO_PIN_15

#define RADIO_MOSI_PORT                           GPIOA
#define RADIO_MOSI_PIN                            GPIO_PIN_7

#define RADIO_MISO_PORT                           GPIOA
#define RADIO_MISO_PIN                            GPIO_PIN_6

#define RADIO_SCLK_PORT                           GPIOA
#define RADIO_SCLK_PIN                            GPIO_PIN_5

#define RADIO_NSS_PORT                            GPIOA
#define RADIO_NSS_PIN                             GPIO_PIN_4

#define RADIO_DIO_0_PORT                          GPIOC
#define RADIO_DIO_0_PIN                           GPIO_PIN_13

#define RADIO_DIO_1_PORT                          GPIOB
#define RADIO_DIO_1_PIN                           GPIO_PIN_6

#define RADIO_DIO_2_PORT                          GPIOB
#define RADIO_DIO_2_PIN                           GPIO_PIN_7

#define RADIO_DIO_3_PORT                          GPIOB
#define RADIO_DIO_3_PIN                           GPIO_PIN_8

#ifdef RADIO_DIO_4 
#define RADIO_DIO_4_PORT                          GPIOB
#define RADIO_DIO_4_PIN                           GPIO_PIN_9
#endif

#define RADIO_ANT_SWITCH_PORT                     GPIOC //CRF1
#define RADIO_ANT_SWITCH_PIN                      GPIO_PIN_14

#define BAT_LEVEL_PORT                            GPIOB
#define BAT_LEVEL_PIN                             GPIO_PIN_1

/*  SPI MACRO redefinition */
#define SPI_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI1_AF                          GPIO_AF5_SPI1 

/* --------------------------- RTC HW definition -------------------------------- */
#define RTC_OUTPUT       DBG_RTC_OUTPUT


/* --------------------------- USART HW definition -------------------------------*/

#define USARTX                           USART1
#define USARTX_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USARTX_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTX_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

#define USARTX_FORCE_RESET()             __USART1_FORCE_RESET()
#define USARTX_RELEASE_RESET()           __USART1_RELEASE_RESET()


/* Definition for USARTx Pins */
#define USARTX_TX_PIN                  GPIO_PIN_9
#define USARTX_TX_GPIO_PORT            GPIOA  
#define USARTX_TX_AF                   GPIO_AF7_USART1
#define USARTX_RX_PIN                  GPIO_PIN_10
#define USARTX_RX_GPIO_PORT            GPIOA 
#define USARTX_RX_AF                   GPIO_AF7_USART1

/* Definition for USARTx's NVIC */
#define USARTX_IRQn                      USART1_IRQn
#define USARTX_IRQHandler                USART1_IRQHandler


/* --------------------------- LED definition -------------------------------*/
#define LEDn                               5

#define LED1_PIN                           GPIO_PIN_12
#define LED1_GPIO_PORT                     GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LED2_PIN                           GPIO_PIN_11
#define LED2_GPIO_PORT                     GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LED3_PIN                           GPIO_PIN_12
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED4_PIN                           GPIO_PIN_13
#define LED4_GPIO_PORT                     GPIOB
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED5_PIN                           GPIO_PIN_14
#define LED5_GPIO_PORT                     GPIOB
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case LED1: \
                                                    LED1_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED2: \
                                                    LED2_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED3: \
                                                    LED3_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED4: \
                                                    LED4_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED5: \
                                                    LED5_GPIO_CLK_ENABLE();   \
                                                    break; \
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case LED1: \
                                                    LED1_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED2: \
                                                    LED2_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED3: \
                                                    LED3_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED4: \
                                                    LED4_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED5: \
                                                    LED5_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)

																							
/* --------------------------- BUTTON definition -------------------------------*/
#define BUTTONn                          1  

#define USER_BUTTON_PIN                         GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT                   GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()   
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_2
#define USER_BUTTON_EXTI_IRQn                   EXTI2_IRQn
/* Aliases */
#define KEY_BUTTON_PIN                   USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT             USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()     USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()    USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn             USER_BUTTON_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == 0) USER_BUTTON_GPIO_CLK_ENABLE();} while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? USER_BUTTON_GPIO_CLK_DISABLE() : 0)


/* --------------------------- DEBUG redefinition -------------------------------*/
#define __HAL_RCC_DBGMCU_CLK_ENABLE()
#define __HAL_RCC_DBGMCU_CLK_DISABLE()

#define LED_Toggle( x )                 
#define LED_On( x )              
#define LED_Off( x )              


/* ------------------ SX1276 Power Control redefinition -------------------------*/
#define RFPOWER_PIN                           GPIO_PIN_8
#define RFPOWER_GPIO_PORT                     GPIOA
#define RFPOWER_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define RFPOWER_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#ifdef __cplusplus
}
#endif

#endif /* __HW_CONF_L1_H__ */

