#include "system.h"
#include "stdio.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "gpio.h"
#include "buttons.h"
#include "ui_core.h"
#include "comd.h"
#include "usbd_cdc_acm_if.h"
#include "crypto_io.h"
#include "usbd.h"

#define CRYPTO_PREFIX "CRYPTO_"
#define SYSTEM_PREFIX "SYS_"

typedef struct{
	BTN_push_button_t *perimeterButtons[8];
	BTN_push_button_t *joyCenter;
	BTN_push_button_t *joyCCW;
	BTN_push_button_t *joyCW;
} buttonArray_t;


buttonArray_t buttonArray;

void SYS_send_CDC( const uint8_t* buf, uint16_t size ){
	CDC_Transmit(0, (uint8_t*)buf, size);
}

char* SYS_IRQ_reveive( const char* cmd ){
	//comd_send_str_IT(cmd);
	return (char*)cmd;
}

void SYS_init( void ){
	comd_add_receive_callback(SYS_IRQ_reveive, SYSTEM_PREFIX);
	comd_add_receive_callback(crypto_io_cmd_parse, CRYPTO_PREFIX);
	comd_set_tx_function(SYS_send_CDC);
	//comd_receive_IRQ(
}

BTN_button_state_t oldSt;

void SYS_BtnCallback( uint32_t button_id, BTN_button_state_t state ){
	UI_event_set_button( button_id, state );
}

bool SYS_GetSWstate ( GPIO_TypeDef *GPIO, uint32_t pin ){
	return !(bool)(GPIO->IDR & pin);
}

bool SYS_GetJoyPushState ( GPIO_TypeDef *GPIO, uint32_t pin ){
	JOY_GND_GPIO_Port->MODER &= ~(3 << GPIO_MODER_MODER1_Pos);
	JOY_GND_GPIO_Port->PUPDR |= (1 << GPIO_PUPDR_PUPDR1_Pos);
	return !(bool)(GPIO->IDR & pin);
	JOY_GND_GPIO_Port->MODER |= (1 << GPIO_MODER_MODER1_Pos);
	JOY_GND_GPIO_Port->ODR   &= ~GPIO_ODR_ODR_1;
}

bool SYS_GetJoySwipeState ( GPIO_TypeDef *GPIO, uint32_t pin ){
	JOY_GND_GPIO_Port->MODER |= (1 << GPIO_MODER_MODER1_Pos);
	JOY_GND_GPIO_Port->ODR   &= ~GPIO_ODR_ODR_1;
	bool out = !(bool)(GPIO->IDR & pin);
	JOY_GND_GPIO_Port->MODER &= ~(3 << GPIO_MODER_MODER1_Pos);
	JOY_GND_GPIO_Port->PUPDR |= (1 << GPIO_PUPDR_PUPDR1_Pos);
	return out;
}

void SYS_ButtonsInit( void ){
	
	buttonArray.perimeterButtons[0] = BTN_add_button(BTN_SW1_ID);
	buttonArray.perimeterButtons[1] = BTN_add_button(BTN_SW2_ID);
	buttonArray.perimeterButtons[2] = BTN_add_button(BTN_SW3_ID);
	buttonArray.perimeterButtons[3] = BTN_add_button(BTN_SW4_ID);
	buttonArray.perimeterButtons[4] = BTN_add_button(BTN_SW5_ID);
	buttonArray.perimeterButtons[5] = BTN_add_button(BTN_SW6_ID);
	buttonArray.perimeterButtons[6] = BTN_add_button(BTN_SW7_ID);
	buttonArray.perimeterButtons[7] = BTN_add_button(BTN_SW8_ID);
	buttonArray.joyCenter = BTN_add_button(BTN_JPUSH_ID);
	buttonArray.joyCCW 		= BTN_add_button(BTN_JCCW_ID);
	buttonArray.joyCW 		= BTN_add_button(BTN_JCW_ID);
	
	BTN_connect_button(buttonArray.perimeterButtons[0], SYS_GetSWstate, SW1_GPIO_Port, SW1_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[1], SYS_GetSWstate, SW2_GPIO_Port, SW2_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[2], SYS_GetSWstate, SW3_GPIO_Port, SW3_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[3], SYS_GetSWstate, SW4_GPIO_Port, SW4_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[4], SYS_GetSWstate, SW5_GPIO_Port, SW5_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[5], SYS_GetSWstate, SW6_GPIO_Port, SW6_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[6], SYS_GetSWstate, SW7_GPIO_Port, SW7_Pin);
	BTN_connect_button(buttonArray.perimeterButtons[7], SYS_GetSWstate, SW8_GPIO_Port, SW8_Pin);
	BTN_connect_button(buttonArray.joyCenter,SYS_GetJoyPushState,  JOY_GND_GPIO_Port, JOY_GND_Pin);
	BTN_connect_button(buttonArray.joyCCW, 	SYS_GetJoySwipeState, JOY_CCW_GPIO_Port, JOY_CCW_Pin);
	BTN_connect_button(buttonArray.joyCW, 		SYS_GetJoySwipeState, JOY_CW_GPIO_Port , JOY_CW_Pin );
	
	for(uint32_t i = 0; i < 8; i++){
		BTN_set_callback(buttonArray.perimeterButtons[i], SYS_BtnCallback);
	}
	BTN_set_callback(buttonArray.joyCenter, SYS_BtnCallback);
	BTN_set_callback(buttonArray.joyCCW, SYS_BtnCallback);
	BTN_set_callback(buttonArray.joyCW, SYS_BtnCallback);
}

void SystemClock_Config(void);

uint8_t needSleepFlag = 0;

extern USBD_HandleTypeDef hUsbDeviceFS;

void SYS_GoToSleep( void ){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//USBD_DeInit(&hUsbDeviceFS);
	
	HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_SET);
	
	// EXTI PUSH
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWREx_EnableUltraLowPower();
	__HAL_RCC_PLL_DISABLE();
	HAL_SuspendTick();
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
	
	PWR->CR = PWR_CR_VOS_0 | PWR_CR_CWUF_Msk;
	
	SystemClock_Config();
	HAL_ResumeTick();
	
	MX_GPIO_Init();
}



void power_GoToSleep(){
	needSleepFlag = 0;
	//HAL_SuspendTick();
	//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

int exampleF(int number, char *string){
	
	sprintf(string, "%d", number);
	return 0;
}

inline void power_SetNeedSleepFlag( void ){
	needSleepFlag = 1;
}

uint8_t power_GetNeedSleepFlag( void ){
	return needSleepFlag;
}
