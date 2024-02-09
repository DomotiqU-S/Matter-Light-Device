#include "LightDriver.hpp"

LightDriver::LightDriver(uint8_t gpio, uint8_t channel)
{
    this->gpio = gpio;
    this->channel = channel;
}

LightDriver::~LightDriver()
{
}

esp_err_t LightDriver::set_power(bool power)
{
    return ESP_OK;
}

esp_err_t LightDriver::set_brightness(uint8_t brightness)
{
    return ESP_OK;
}

esp_err_t LightDriver::set_hue(uint16_t hue)
{
    return ESP_OK;
}

esp_err_t LightDriver::set_saturation(uint8_t saturation)
{
    return ESP_OK;
}

esp_err_t LightDriver::set_temperature(uint32_t temperature)
{
    return ESP_OK;
}

led_driver_handle_t LightDriver::init()
{
    return nullptr;
}
