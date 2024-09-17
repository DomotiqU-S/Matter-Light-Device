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
    ESP_LOGE("LightDriver", "brightness: %d", brightness);
    return led.setIntensity(brightness);
}

esp_err_t LightDriver::set_hue(uint16_t hue)
{
    if(m_gpio_type.gpio_red_light != 0 && m_gpio_type.gpio_green_light != 0 && m_gpio_type.gpio_blue_light != 0) {

    }
    return ESP_OK;
}

esp_err_t LightDriver::set_saturation(uint8_t saturation)
{
    if(m_gpio_type.gpio_red_light != 0 && m_gpio_type.gpio_green_light != 0 && m_gpio_type.gpio_blue_light != 0) {
        
    }
    return ESP_OK;
}

esp_err_t LightDriver::set_temperature(uint32_t temperature)
{
    ESP_LOGE("LightDriver", "TEMP value: %lu", temperature);
    return led.setTemperature((uint16_t)temperature);
}

led_driver_handle_t LightDriver::init()
{
    return nullptr;
}

esp_err_t LightDriver::set_color(uint16_t x, uint16_t y) {
    return ESP_OK;
}
