# battery_voltage 电池电压监测
## 概述
`battery_voltage` 用于监测电池电压，基于 ESP32S3 的 ADC（模数转换器）功能。通过测量电池电压，计算出电池的剩余电量百分比。

**支持功能:**
- **电压测量：** 通过 ADC 测量电池电压。
- **电量计算：** 根据电压值计算电池的剩余电量百分比。
- **校准功能：** 用于补偿 ADC 和电池电压的测量误差。

## 硬件要求
- 电池电压分压电路，将电池电压分压到 ADC 输入引脚。


## 使用方法
``` c
// 在tjl_battery_voltage.h中修改下面的参数配置
/* ADC 配置 */
// #define ADC_CHANNEL ADC1_CHANNEL_2     // adc1 通道 GPIO3
// #define ADC_ATTEN_DB ADC_ATTEN_DB_12   // 衰减
// #define ADC_WIDTH_BIT ADC_WIDTH_BIT_12 // 12 位分辨率
// #define ADC_UNIT ADC_UNIT_1            // 单通道 ADC
// #define ADC_SAMOLING_COUNT 500         // 采样次数
// #define DEFAULT_VREF    1100           // 默认内部参考电压 (mV)
// /* 电池配置 */
// #define R1 300000.0f     // 分压电阻 1
// #define R2 300000.0f     // 分压电阻 2
// #define BATTERY_MIN 3.7f // 电池最低电压 (V)
// #define BATTERY_MAX 4.2f // 电池最高电压 (V)
// #define ADC_OFFSET 0 // ADC 偏移电压 (V) 200
// #define VOL_OFFSET 0.39f // 电池补偿电压 (V)

#include "tjl_battery_voltage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "battery_monitor"

void app_main()
{
    // 初始化电池监测模块
    battery_monitor_init();

    while (1) {
        float adc_raw_value = get_adc_voltage();
        float voltage = get_battery_voltage(adc_raw_value);
        float percentage = get_battery_percentage(voltage);

        ESP_LOGI(TAG, "Battery Voltage: %.2f V", voltage);
        ESP_LOGI(TAG, "Battery Percentage: %.2f%%", percentage);

        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

```

## 依赖项

- ESP-IDF >= 5.3.1

## 许可证

本项目采用 MIT 许可证。更多信息请参阅 [LICENSE](LICENSE) 文件。


