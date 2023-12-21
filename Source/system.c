#include "system.h"
#include "stdio.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "gpio.h"
#include "buttons.h"
#include "ui/core.h"
#include "comd.h"
#include "usbd_cdc_acm_if.h"
#include "crypto_io.h"
#include "usbd.h"
#include "adc.h"
#include "rtc.h"
#include "usb.h"

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
	// ADC read voltage enable
    HAL_GPIO_WritePin(ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_RESET);
	comd_add_receive_callback(SYS_IRQ_reveive, SYSTEM_PREFIX);
	comd_add_receive_callback(crypto_io_cmd_parse, CRYPTO_PREFIX);
	comd_set_tx_function(SYS_send_CDC);

	HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
	
	ssd1306_SetDisplayPower(1);
	ssd1306_SetDisplayOn(1);
	
	ssd1306_Init();
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
extern USBD_HandleTypeDef hUsbDevice;
extern PCD_HandleTypeDef hpcd_USB_FS;
void SYS_GoToSleep( void ){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//USBD_DeInit(&hUsbDeviceFS);
	//ssd1306_SetDisplayPower(0);
	HAL_Delay(1000);
	// EXTI PUSH
	USBD_Stop(&hUsbDevice);
	USBD_DeInit(&hUsbDevice);
	HAL_PCD_Stop(&hpcd_USB_FS);
	HAL_PCD_DeInit(&hpcd_USB_FS);
	//hpcd_USB_FS.Instance->CNTR = 0x0003;
	hpcd_USB_FS.Instance->CNTR |= (1 << USB_CNTR_FSUSP);
	hpcd_USB_FS.Instance->CNTR |= (1 << USB_CNTR_LPMODE);
	hpcd_USB_FS.Instance->CNTR |= (1 << USB_CNTR_PDWN);
	
	__HAL_RCC_USB_CLK_DISABLE();
	__HAL_PCD_DISABLE(&hpcd_USB_FS);
	HAL_ADC_Stop(&hadc);
	
	//HAL_ADC_DeInit(&hadc);
	//HAL_RTC_DeInit(&hrtc);
	GPIOA->MODER = 0xffffffff;
	GPIOB->MODER = 0xffffffff;
	GPIOC->MODER = 0xffffffff;
	GPIOH->MODER = 0xffffffff;
	GPIO_InitStruct.Pin = JOY_GND_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(JOY_GND_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = VBUS_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(VBUS_EN_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_SET);
	
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	
	ADC->CCR &= ~ADC_CCR_TSVREFE; // Temperature sensor and V REFINT channel disabled
	__HAL_ADC_DISABLE(&hadc);
	HAL_SuspendTick();
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);

	PWR->CR |= PWR_CR_CWUF;

	/* источник опорного напряжения Vref выключить автоматически */
	PWR->CR |= PWR_CR_ULP;
	PWR->CR &= ~PWR_CR_PVDE; // PVD disable
	FLASH->OBR &= ~FLASH_OBR_BOR_LEV; // BOR disable
	RCC->CR &= ~RCC_CR_HSEON;
	
	HAL_PWREx_EnableUltraLowPower();
	HAL_PWREx_DisableFastWakeUp();
	
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );

	SystemClock_Config();
	HAL_ResumeTick();
	HAL_Delay(500);
	NVIC_SystemReset();
	MX_GPIO_Init();
	
	HAL_GPIO_WritePin(ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(VBUS_EN_GPIO_Port, VBUS_EN_Pin, GPIO_PIN_RESET);
	
	ssd1306_SetDisplayPower(1);
	ssd1306_SetDisplayOn(1);

	//__HAL_RCC_USB_CLK_ENABLE();
	//USBD_Start(&hUsbDevice);
	//HAL_PCD_Start(&hpcd_USB_FS);
  	MX_USB_PCD_Init();
	MX_USB_DEVICE_Init();

	ssd1306_Init();
	__HAL_ADC_ENABLE(&hadc);
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
