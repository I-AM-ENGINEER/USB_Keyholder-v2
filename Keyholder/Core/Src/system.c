#include "system.h"
#include "stdio.h"
#include "main.h"
void SystemClock_Config(void);

struct {
	uint8_t needSleepFlag;
} power;

void power_GoToSleep(){
	power.needSleepFlag = 0;
	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

int exampleF(int number, char *string){
	
	sprintf(string, "%d", number);
	return 0;
}

void power_SetNeedSleepFlag( void ){
	power.needSleepFlag = 1;
}

uint8_t power_GetNeedSleepFlag( void ){
	return power.needSleepFlag;
}
