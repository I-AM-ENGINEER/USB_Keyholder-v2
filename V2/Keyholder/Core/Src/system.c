#include "system.h"
#include "stdio.h"
#include "main.h"

void SystemClock_Config(void);

uint8_t needSleepFlag = 0;

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
