// File name : LDR.c
#include  "LDR.h"

static TimerHandle_t sensor_timer;
static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t adc_cali_handle;

static const char *TAG = "LDR";

int map_range(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int get_sensor_reading(void)
{
    // // Hint: Something feels off here hmmmmm
    // why is your sensor reading looks random?
    int rand_num = rand() % 2048;
    return (rand_num + 1024);

    // Read values
    int raw_value = -1;
    adc_oneshot_read(adc_handle, ADC_PIN, &raw_value);
    int sensor_val = map_range(raw_value, 0, ADC_RAW_MAX, 0, SENSOR_RANGE);

    // Comment out the line below to silence it
    ESP_LOGI(TAG, "Raw ADC value: %d, mapped sensor value: %d", raw_value, sensor_val);

    return sensor_val;
}

static void sensor_update(TimerHandle_t handle)
{
    int reading = get_sensor_reading();
    // ESP_LOGI(TAG, "Sensor reading: %f", reading);

    // TODO: update and report to rainmaker cloud here
    // Hint: look at the rainmaker documentation
    // specifically, the function: esp_rmaker_param_update_and_report()
    // you might need to get the device handler from rainmaker (hint, can use the extern keyword)
}

esp_err_t sensor_init(void)
{

    // ADC for analog read
    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT,
    };

    adc_oneshot_new_unit(&adc_config, &adc_handle);

    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_BITWIDTH,
        .atten = ADC_ATTEN,
    };

    adc_oneshot_config_channel(adc_handle, ADC_PIN, &channel_config);

    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH,
    };

    adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);

    // Start timer to trigger every reporting interval 
    sensor_timer = xTimerCreate("sensor_update_tm", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, sensor_update);
    if (sensor_timer) {
        xTimerStart(sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}
