#ifndef LED_SENSOR_HPP
#define LED_SENSOR_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define TAG_SENSOR          "LED_DRIVER"                            // Tag for the logs
#define LEDC_TIMER          LEDC_TIMER_0                            // LEDC timer
#define LEDC_MODE           LEDC_LOW_SPEED_MODE                     // LEDC speed mode
#define LEDC_WARM           5                                       // Define the output GPIO
#define LEDC_COOL           6                                       // Define the output GPIO
#define LEDC_DUTY_RES       LEDC_TIMER_10_BIT                       // Set duty resolution to 13 bits
#define MAX_DUTY            (uint32_t)1024                          // Maximum duty for 13 bits
#define LEDC_FREQUENCY      (uint32_t)25000                         // Frequency in Hertz. Set frequency at 50 kHz for hearing protection
#define LEDC_FADE_TIME      (ledc_fade_mode_t)500                  // Fade time in milliseconds 
#define MAX_TEMPERATURE     6500.0                                  // Maximum temperature in kelvin
#define MIN_TEMPERATURE     2700.0                                  // Minimum temperature in kelvin
#define MID_TEMPERATURE     (MAX_TEMPERATURE + MIN_TEMPERATURE)/2   // Minimum temperature in kelvin
#define RANGE_TEMPERATURE   (MAX_TEMPERATURE - MIN_TEMPERATURE)     // Range of temperature in kelvin
// #define INVERSOR            true                                    // Inversor for the temperature
#define DEBUG_SENSOR        true                                    // Enable debug logs for the temperature sensor


class LedDriver
{
private:
    bool fade;
    ledc_fade_mode_t fade_time;
    esp_err_t ret;
    uint32_t dutyWarm = 0;
    uint32_t dutyCool = 0;
    uint16_t temperature = MID_TEMPERATURE;
    uint8_t intensity = 0;
    SemaphoreHandle_t semaphore_cool = NULL;
    SemaphoreHandle_t semaphore_warm = NULL;

    /*
     * This callback function will be called when fade operation has ended
     * Use callback only if you are aware it is being called inside an ISR
     * Otherwise, you can use a semaphore to unblock tasks
     */
    static IRAM_ATTR bool cb_ledc_fade_end_event_cool(const ledc_cb_param_t *param, void *user_arg)
    {
        BaseType_t taskAwoken = pdFALSE;

        if (param->event == LEDC_FADE_END_EVT)
        {
            SemaphoreHandle_t counting_sem = (SemaphoreHandle_t)user_arg;
            xSemaphoreGiveFromISR(counting_sem, &taskAwoken);
        }

        return (taskAwoken == pdTRUE);
    }

    static IRAM_ATTR bool cb_ledc_fade_end_event_warm(const ledc_cb_param_t *param, void *user_arg)
    {
        BaseType_t taskAwoken = pdFALSE;

        if (param->event == LEDC_FADE_END_EVT)
        {
            SemaphoreHandle_t counting_sem = (SemaphoreHandle_t)user_arg;
            xSemaphoreGiveFromISR(counting_sem, &taskAwoken);
        }

        return (taskAwoken == pdTRUE);
    }

public:
    /**
     * @brief Construct a new Led Driver object
     *
     * @param cool_pin The GPIO pin for the cool LED
     * @param warm_pin The GPIO pin for the warm LED
     * @param frequency The frequency of the PWM signal
     * @param intensity The intensity of the LED in percentage
     * @param temperature The temperature of the LED in degrees kelvin
     * @param fade The fade option
     * @param fade_time The fade time
     */
    LedDriver(int cool_pin = LEDC_COOL, int warm_pin = LEDC_WARM, uint32_t frequency = LEDC_FREQUENCY, uint16_t intensity = 50, uint16_t temperature = 4600, bool fade = true, ledc_fade_mode_t fade_time = LEDC_FADE_TIME)
    {
        this->intensity = intensity;
        this->temperature = temperature;
        this->fade = fade;
        this->fade_time = fade_time;
        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_MODE,
            .duty_resolution = LEDC_DUTY_RES,
            .timer_num = LEDC_TIMER,
            .freq_hz = frequency, // Set output frequency
            .clk_cfg = LEDC_AUTO_CLK};
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
        // Prepare and then apply the LEDC PWM channel configuration
        ledc_channel_config_t cool_channel;
        cool_channel.speed_mode = LEDC_MODE;
        cool_channel.channel = LEDC_CHANNEL_0;
        cool_channel.timer_sel = LEDC_TIMER;
        cool_channel.gpio_num = cool_pin;
        cool_channel.duty = (uint32_t)256; // Set duty to 25%
        cool_channel.intr_type = this->fade? LEDC_INTR_FADE_END: LEDC_INTR_DISABLE;
        cool_channel.hpoint = 0;
        ledc_channel_config_t warm_channel;
        warm_channel.speed_mode = LEDC_MODE;
        warm_channel.channel = LEDC_CHANNEL_1;
        warm_channel.timer_sel = LEDC_TIMER;
        warm_channel.gpio_num = warm_pin;
        warm_channel.duty = (uint32_t)256; // Set duty to 25%
        warm_channel.intr_type = this->fade? LEDC_INTR_FADE_END: LEDC_INTR_DISABLE;
        warm_channel.hpoint = 0;
        ESP_ERROR_CHECK(ledc_channel_config(&cool_channel));
        ESP_ERROR_CHECK(ledc_channel_config(&warm_channel));
        if (fade == true)
        {
            ledc_fade_func_install(0);
            ledc_cbs_t callbacks_cool = {
                .fade_cb = this->cb_ledc_fade_end_event_cool
            };
            ledc_cbs_t callbacks = {
                .fade_cb = this->cb_ledc_fade_end_event_warm
            };
            this->semaphore = xSemaphoreCreateCounting(2, 0);
            ledc_cb_register(LEDC_MODE, LEDC_CHANNEL_0, &callbacks_cool, (void *) this->semaphore_cool);
            ledc_cb_register(LEDC_MODE, LEDC_CHANNEL_1, &callbacks_warm, (void *) this->semaphore_warm);
        }
    }
    ~LedDriver();
    /**
     * @brief Switch the state of the LED
     * The state of the LED
     * @param state The state of the LED
     * @return esp_err_t the error code
     */
    esp_err_t switchState(bool state);
    /**
     * @brief Set the intensity of the LED
     * The intensity of the LED in percentage
     * @param intensity The intensity of the LED in percentage
     * @return esp_err_t the error code
     */
    esp_err_t setIntensity(uint8_t intensity);
    /**
     * @brief Set the temperature of the LED in degrees kelvin
     * The temperature color of the light bulb.
     * MAX 6500K
     * MIN 2700K
     * @param temperature The temperature of the LED in degrees kelvin
     * @return esp_err_t the error code
     */
    esp_err_t setTemperature(uint16_t temperature);
    /**
     * @brief Get the temperature of the LED
     * The temperature color of the light bulb.
     * @return uint16_t The temperature of the LED in degrees kelvin
     */
    uint16_t getTemperature();
    /**
     * @brief Get the intensity of the LED
     * The intensity of the LED in percentage
     * @return uint16_t The intensity of the LED in percentage
     */
    uint8_t getIntensity();
    /**
     * @brief Get the duty of the LED
     * The duty of the LED in percentage
     * @return uint8_t The duty of the LED in percentage
     */
    uint32_t getDuty(int channel);
};

#endif // LED_SENSOR_HPP