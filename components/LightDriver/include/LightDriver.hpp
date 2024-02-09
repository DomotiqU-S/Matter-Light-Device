#ifndef LIGHT_DRIVER_HPP
#define LIGHT_DRIVER_HPP

#include <esp_err.h>
#include <stdbool.h>
#include <stdint.h>

typedef void *led_driver_handle_t;

class LightDriver
{
private:
    uint8_t gpio;
    uint8_t channel;
public:
    LightDriver(uint8_t gpio, uint8_t channel);
    ~LightDriver();
    led_driver_handle_t init();
    esp_err_t set_power(bool power);
    esp_err_t set_brightness(uint8_t brightness);
    esp_err_t set_hue(uint16_t hue);
    esp_err_t set_saturation(uint8_t saturation);
    esp_err_t set_temperature(uint32_t temperature);
};

#endif // LIGHT_DRIVER_HPP
