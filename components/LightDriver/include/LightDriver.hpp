#ifndef LIGHT_DRIVER_HPP
#define LIGHT_DRIVER_HPP

#include <esp_err.h>
#include "led_driver.hpp"
#include <stdbool.h>
#include <stdint.h>

typedef void *led_driver_handle_t;

typedef enum {
    HAL_ESP = 0,
    HAL_CUSTOM,
    WS_DRIVER,
} driver_t;
typedef struct {
    uint8_t gpio_warm;
    uint8_t gpio_cold;

    uint8_t gpio_red_light;
    uint8_t gpio_green_light;
    uint8_t gpio_blue_light;

    uint8_t gpio_light_data;
} light_gpio_t;

class LightDriver
{
private:
    driver_t m_driver;
    light_gpio_t m_gpio_type;

    LedDriver led;
public:
    LightDriver(uint8_t gpio_warm, uint8_t gpio_cold) : led(gpio_warm, gpio_cold) {
        m_driver = HAL_CUSTOM;
        m_gpio_type.gpio_cold = gpio_cold;
        m_gpio_type.gpio_warm = gpio_warm;
    }
    // LightDriver(uint8_t gpio_r, uint8_t gpio_g, uint8_t gpio_b) : led(gpio_r, gpio_g) {
    //     m_driver = HAL_CUSTOM;
    //     m_gpio_type.gpio_red_light = gpio_r;
    //     m_gpio_type.gpio_green_light = gpio_g;
    //     m_gpio_type.gpio_blue_light = gpio_b;
    // }
    ~LightDriver();
    led_driver_handle_t init();
    esp_err_t set_power(bool power);
    esp_err_t set_brightness(uint8_t brightness);
    esp_err_t set_color(uint16_t x, uint16_t y);
    esp_err_t set_hue(uint16_t hue);
    esp_err_t set_saturation(uint8_t saturation);
    esp_err_t set_temperature(uint32_t temperature);
    void led_routine();
};

#endif // LIGHT_DRIVER_HPP
