/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H__
#define __BSP_H__

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "hw.h"
	 
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

void            BSP_LED_Init(Led_TypeDef Led);
void            BSP_LED_DeInit(Led_TypeDef Led);
void            BSP_LED_On(Led_TypeDef Led);
void            BSP_LED_Off(Led_TypeDef Led);
void            BSP_LED_Toggle(Led_TypeDef Led);	 
	 

void            BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void            BSP_PB_DeInit(Button_TypeDef Button);
uint32_t        BSP_PB_GetState(Button_TypeDef Button);


void            BSP_SX1276POWER_Init( void );
void            BSP_SX1276POWER_DeInit( void );
void            BSP_SX1276POWER_On( void );
void            BSP_SX1276POWER_Off( void );

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H__ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
