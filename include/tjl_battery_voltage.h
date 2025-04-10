#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>
#include <stdint.h>
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_oneshot.h"

// ADC 配置
#define ADC_CHANNEL ADC1_CHANNEL_2     // adc1 通道 GPIO3
#define ADC_ATTEN_DB ADC_ATTEN_DB_12   // 衰减
#define ADC_WIDTH_BIT ADC_WIDTH_BIT_12 // 12 位分辨率
#define ADC_UNIT ADC_UNIT_1            // 单通道 ADC
#define ADC_SAMOLING_COUNT 500         // 采样次数
#define DEFAULT_VREF    1100           // 默认内部参考电压 (mV)

// 电池配置
#define R1 300000.0f     // 分压电阻 1
#define R2 300000.0f     // 分压电阻 2
#define BATTERY_MIN 3.7f // 电池最低电压 (V)
#define BATTERY_MAX 4.2f // 电池最高电压 (V)
#define ADC_OFFSET 0 // ADC 偏移电压 (V) 200
#define VOL_OFFSET 0.39f // 电池补偿电压 (V)

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief 初始化
     */
    void battery_monitor_init();

    /**
     * @brief 获取 ADC检测端的电压 (单位: V)
     * @return float ADC电压
     */
    float get_adc_voltage();

    /**
     * @brief 获取电池电压 (单位: V)
     *
     * @return float 电池电压
     */
    float get_battery_voltage(float adc_voltage);

    /**
     * @brief 获取电池电量百分比
     *
     * @return float 电池电量百分比 (0%-100%)
     */
    float get_battery_percentage(float battery_battery);

#ifdef __cplusplus
}
#endif

#endif