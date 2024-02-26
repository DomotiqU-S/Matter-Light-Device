#include "LightDriver.hpp"

LightDriver::~LightDriver()
{
}

esp_err_t LightDriver::set_power(bool power)
{
    ESP_LOGE("LightDriver", "power: %d", power);
    return led.switchState(power);
}

esp_err_t LightDriver::set_brightness(uint8_t brightness)
{
    return led.setIntensity(brightness);
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
    return led.setTemperature((uint16_t)temperature);
}

led_driver_handle_t LightDriver::init()
{
    return nullptr;
}
