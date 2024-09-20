#ifndef LED_SENSOR_HPP
#define LED_SENSOR_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include <cmath>
#include "LightDriver.hpp"

#define TAG_SENSOR          "LED_DRIVER"                            // Tag for the logs
#define LEDC_TIMER          LEDC_TIMER_0                            // LEDC timer
#define LEDC_MODE           LEDC_LOW_SPEED_MODE                     // LEDC speed mode
#define LEDC_WARM           4                                       // Define the output GPIO
#define LEDC_COOL           5                                       // Define the output GPIO
#define LEDC_DUTY_RES       LEDC_TIMER_10_BIT                       // Set duty resolution to 10 bits
#define MAX_DUTY            (uint32_t)1024                          // Maximum duty for 10 bits
#define LEDC_FREQUENCY      (uint32_t)20000                         // Frequency in Hertz. Set frequency at 20 kHz
#define MAX_TEMPERATURE     6500.0                                  // Maximum temperature in kelvin
#define MIN_TEMPERATURE     2700.0                                  // Minimum temperature in kelvin
#define MID_TEMPERATURE     (MAX_TEMPERATURE + MIN_TEMPERATURE)/2   // Minimum temperature in kelvin
#define RANGE_TEMPERATURE   (MAX_TEMPERATURE - MIN_TEMPERATURE)     // Range of temperature in kelvin
// #define INVERSOR            true                                    // Inversor for the temperature
#define DEBUG_SENSOR        true                                    // Enable debug logs for the temperature sensor
#define FADE_STEP           255                                      // Fade step in percentage
#define FADE_INTERVAL      2
#define FADE_ENABLE         1
#define FADE_DURATION      1000 / 50 // the maximum time of fade is 1 second and the PWM is updated 50 times per second: so generally the formula is (total time / update per second)

class LedDriver : public LightDriver
{
private:
    bool fade = false, is_update = false, is_started = false;
    ledc_fade_mode_t fade_time;
    esp_err_t ret;
    
    int32_t dutyWarm = 0;
    int32_t dutyCool = 0;
    int32_t target_duty_cool = 0;
    int32_t target_duty_warm = 0;

    uint16_t temperature = MID_TEMPERATURE;
    uint16_t temperature_target = 0;
    int16_t step_cool = 0;
    int16_t step_warm = 0;

    uint8_t intensity = 0;
    uint8_t intensity_target = 254;
    uint8_t fade_counter = 0;

    led_driver_handle_t handle;

    bool state = false;

    void changePWM(uint32_t dutyCool, uint32_t dutyWarm);

    void updateLoop();

public:
    /**
     * @brief Construct a new Led Driver object
     *
     * @param cool_pin The GPIO pin for the cool LED
     * @param warm_pin The GPIO pin for the warm LED
     * @param frequency The frequency of the PWM signal
     * @param intensity The intensity of the LED in percentage
     * @param temperature The temperature of the LED in degrees kelvin
     */
    LedDriver(int cool_pin = LEDC_COOL, int warm_pin = LEDC_WARM, uint32_t frequency = LEDC_FREQUENCY, uint16_t intensity = 0, uint16_t temperature = 4600, bool initial_state = false)
    {
        this->intensity = intensity;
        this->temperature = temperature;
        this->state = initial_state;

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
        cool_channel.duty = (uint32_t)dutyCool;
        cool_channel.intr_type = this->fade? LEDC_INTR_FADE_END: LEDC_INTR_DISABLE;
        cool_channel.hpoint = 0;

        ledc_channel_config_t warm_channel;
        warm_channel.speed_mode = LEDC_MODE;
        warm_channel.channel = LEDC_CHANNEL_1;
        warm_channel.timer_sel = LEDC_TIMER;
        warm_channel.gpio_num = warm_pin;
        warm_channel.duty = (uint32_t)dutyWarm;
        warm_channel.intr_type = this->fade? LEDC_INTR_FADE_END: LEDC_INTR_DISABLE;
        warm_channel.hpoint = 0;
        ESP_ERROR_CHECK(ledc_channel_config(&cool_channel));
        ESP_ERROR_CHECK(ledc_channel_config(&warm_channel));
    }
    ~LedDriver();

    /**
     * @brief This is the task that will change the level of the LED simultaneously
     * 
     * @param pvParameter 
     */
    static void changeLevel(void *pvParameter);

    /**
     * @brief The function update the value of the PWM
     * 
     */
    void routine();

    void toggleUpdate();

    // Virtual functions from the LightDriver class

    led_driver_handle_t init();

    uint16_t get_temperature();

    uint8_t get_intensity();

    uint32_t get_duty(uint8_t channel);

    uint16_t get_hue();

    uint8_t get_saturation();

    uint16_t get_x();

    uint16_t get_y();

    esp_err_t set_power(bool power);

    esp_err_t set_brightness(uint8_t brightness);

    esp_err_t set_color(uint16_t x, uint16_t y);

    esp_err_t set_hue(uint16_t hue);

    esp_err_t set_saturation(uint8_t saturation);

    esp_err_t set_temperature(uint32_t temperature);

    void led_routine();
};

#endif // LED_SENSOR_HPP