#include "tjl_battery_voltage.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "BatteryMonitor";

// 全局变量
static esp_adc_cal_characteristics_t *adc_char;
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc_cali_handle = NULL;
// 初始化电池监测模块
void battery_monitor_init()
{
    // 初始化 ADC 单元
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT, // 使用 ADC1
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    // 配置 ADC 通道
    adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB,     // 衰减值
        .bitwidth = ADC_WIDTH_BIT, // 位宽
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL, &channel_config));
    // 初始化校准方案
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN_DB,
        .bitwidth = ADC_WIDTH_BIT,
    };
    esp_err_t ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Battery monitor successfully!");
    }
    else
    {
        ESP_LOGI(TAG, "Battery monitor Failed!");
    }
}


/**
 *  获取ADC电压
 */
float get_adc_voltage()
{
    // 多次采样求平均值
    int adc_raw_value = 0;
    int adc_temp = 0;
    for (int i = 0; i < ADC_SAMOLING_COUNT; i++)
    {
        adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_temp);
        adc_raw_value = adc_raw_value + adc_temp;
        vTaskDelay(pdMS_TO_TICKS(1)); // 延时 1ms
    }
    adc_raw_value = adc_raw_value / ADC_SAMOLING_COUNT;
    //转换为电压值
    int voltage = 0;
    if (adc_cali_raw_to_voltage(adc_cali_handle, adc_raw_value, &voltage) == ESP_OK) {
        float adc_voltage = (float) (voltage - ADC_OFFSET)/DEFAULT_VREF;
        return adc_voltage;
        //printf("ADC Raw Value: %d\tVoltage: %.2fV\n", adc_raw_value,(float) (voltage - ADC_OFFSET)/DEFAULT_VREF);
    } else {
        return -1;
        //printf("Failed to convert raw value to voltage\n");
    }
}

/**
 * 推算电池电压
 */
float get_battery_voltage(float adc_voltage)
{
    float battery_voltage = (R1 + R2) / R2 * adc_voltage + VOL_OFFSET; // 推算电池电压
    return battery_voltage;
    //printf(" bat - Voltage: %.2fV\n", battery_voltage);
}

/**
 *  推算电池百分比
 */
float get_battery_percentage(float battery_battery)
{
    float percentage = ((battery_battery - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN)) * 100.0f;

    if (percentage < 0) {
        percentage = 0;
    } else if (percentage > 100) {
        percentage = 100;
    }

    return percentage;
}
