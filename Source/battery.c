#include "battery.h"
#include "adc.h"

#define BATTERY_VREF_VOLTAGE    (1224UL)      // mV
// MUST be under 100kOm, else maybe probles with calculations
#define BATTERY_R_VCC_VALUE     (1UL)       // kOhm
#define BATTERY_R_GND_VALUE     (1UL)       // kOhm

#define JITTER_THRESHOLD 16  // Adjust this threshold based on the level of jitter

static volatile int8_t battery_level;
static int32_t mcu_voltage_adc_value_filtered;
static int32_t bat_voltage_adc_value_filtered;

// https://lygte-info.dk/info/BatteryChargePercent%20UK.html
// Table 3.3V...4.2V
static const int8_t votage2percentage[10] = {
    0, 13, 26, 39, 53, 64, 74, 84, 100, 100
};

void battery_process( void ){
    ADC_ChannelConfTypeDef adc_ch_cfg;
    adc_ch_cfg.Channel = ADC_CHANNEL_VREFINT;
    adc_ch_cfg.Rank = ADC_REGULAR_RANK_1;
    adc_ch_cfg.SamplingTime = ADC_SAMPLETIME_384CYCLES;
    
    HAL_ADC_ConfigChannel(&hadc, &adc_ch_cfg);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1000);

    int32_t adc_value = HAL_ADC_GetValue(&hadc) << BATTERY_MCU_VOLTAGE_FILTER_ORDER;
    mcu_voltage_adc_value_filtered += ((adc_value - mcu_voltage_adc_value_filtered) >> BATTERY_MCU_VOLTAGE_FILTER_ORDER);

    adc_ch_cfg.Channel = ADC_CHANNEL_20;
    adc_ch_cfg.Rank = ADC_REGULAR_RANK_1;
    adc_ch_cfg.SamplingTime = ADC_SAMPLETIME_384CYCLES;

    HAL_ADC_ConfigChannel(&hadc, &adc_ch_cfg);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1000);

    adc_value = HAL_ADC_GetValue(&hadc) << BATTERY_BAT_VOLTAGE_FILTER_ORDER;
    bat_voltage_adc_value_filtered += ((adc_value - bat_voltage_adc_value_filtered) >> BATTERY_BAT_VOLTAGE_FILTER_ORDER);

    battery_level = battery_get_charge_level();// For debug only
}

static int16_t battery_fix_jitter( int16_t input_value ){
    static int16_t previous_value = 0;

    if(abs(input_value - previous_value) > JITTER_THRESHOLD){
        previous_value += (input_value - previous_value)/JITTER_THRESHOLD;
    }
    return previous_value;
}

uint16_t battery_get_reference_voltage( void ){
    uint32_t adc_value = (uint32_t)(mcu_voltage_adc_value_filtered >> BATTERY_MCU_VOLTAGE_FILTER_ORDER);
    if(adc_value == 0){
        return 0;
    }
    return (uint16_t)((BATTERY_VREF_VOLTAGE*4095UL)/adc_value);
}

uint16_t battery_get_voltage( void ){
    uint32_t adc_value = (uint32_t)(bat_voltage_adc_value_filtered >> BATTERY_BAT_VOLTAGE_FILTER_ORDER);
    uint32_t mcu_voltage = (uint32_t)battery_get_reference_voltage();
    uint16_t battery_voltage = (uint16_t)((mcu_voltage*adc_value*(BATTERY_R_VCC_VALUE+BATTERY_R_GND_VALUE))/(BATTERY_R_GND_VALUE*4095UL));
    return battery_voltage;
}

int8_t battery_get_charge_level( void ){
    uint16_t battery_voltage = (uint16_t)battery_fix_jitter(battery_get_voltage());
    if (battery_voltage >= 4500){
        return -1;  // Battery error
    }
    if(battery_voltage < 2000){
        return -1;  // Battery error
    }
    if (battery_voltage < 3300){
        return 0;
    }
    if (battery_voltage >= 4200){
        return 100;
    }
    battery_voltage -= 3300; // 0...1099

    int16_t battery_voltage_rounded = battery_voltage/100;
    int16_t battery_voltage_fract   = battery_voltage%100;

    int16_t battery_level_lower = (int16_t)votage2percentage[battery_voltage_rounded];
    int16_t battery_level_upper = (int16_t)votage2percentage[battery_voltage_rounded+1];

    int8_t charge = (int8_t)((battery_voltage_fract * battery_level_upper + (100 - battery_voltage_fract) * battery_level_lower)/100);
    return charge;
}
