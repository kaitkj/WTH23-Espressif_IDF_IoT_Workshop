// File name : LDR.h

#pragma once

#include "esp_log.h"
#include "esp_err.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include "esp_timer.h"

#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

// You can change how frequent the sensor value is read
#define REPORTING_PERIOD 1 // in seconds

// Look up the ESP programming guide to see which pin is ADC1_CH0
#define ADC_UNIT ADC_UNIT_1
#define ADC_PIN ADC_CHANNEL_0 

// Can ignore the following four lines
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_BITWIDTH ADC_BITWIDTH_DEFAULT
#define ADC_RAW_MAX (4095)
#define SENSOR_RANGE (100)

// Can define your own threshold for bright and dark here
// you can then use these constants to decide what to do when certain thresholds are met
#define LDR_BRIGHT (80)
#define LDR_DARK (20)

int get_sensor_reading(void);

int sensor_init();