#include "led_driver.hpp"

LedDriver::~LedDriver()
{
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_0, 0);
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_1, 0);
}

esp_err_t LedDriver::switchState(bool state)
{
    if (this->fade)
    {
        esp_err_t ret = ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_0, state ? this->dutyCool : 0, this->fade_time);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 0 in fade mode");
#endif
            return ret;
        }
        ret = ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        xSemaphoreTake(this->semaphore, portMAX_DELAY);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 0 in fade mode");
#endif
            return ret;
        }
        ret = ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_1, state ? this->dutyWarm : 0, this->fade_time);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 1 in fade mode");
#endif
            return ret;
        }
        ret = ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_1, LEDC_FADE_WAIT_DONE);
        xSemaphoreTake(this->semaphore, portMAX_DELAY);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 1 in fade mode");
#endif
            return ret;
        }
        return ret;
    }
    else
    {
        esp_err_t ret = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, state ? this->dutyCool : 0);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 0 in direct mode");
#endif
            return ret;
        }
        ret = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 0 in direct mode");
#endif
            return ret;
        }
        ret = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, state ? this->dutyWarm : 0);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 1 in direct mode");
#endif
            return ret;
        }
        ret = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 1 in direct mode");
#endif
            return ret;
        }
        return ret;
    }
}

esp_err_t LedDriver::setIntensity(uint8_t intensity)
{
    this->intensity = intensity;
    return this->setTemperature(this->temperature);
}

esp_err_t LedDriver::setTemperature(uint16_t temperature)
{
    esp_err_t ret;
    this->temperature = temperature;
    if (this->temperature == MIN_TEMPERATURE)
    {
        this->dutyWarm = (uint32_t)0;
        this->dutyCool = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 100.0));
    }
    else if (this->temperature == MAX_TEMPERATURE)
    {
        this->dutyWarm = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 100.0));
        this->dutyCool = (uint32_t)0;
    }
    else
    {
        this->dutyWarm = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 100.0)) * (float)((MAX_TEMPERATURE - this->temperature) / RANGE_TEMPERATURE));
        this->dutyCool = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 100.0)) * (float)((this->temperature - MIN_TEMPERATURE) / RANGE_TEMPERATURE));
    }
#ifdef INVERSOR
    this->dutyWarm = MAX_DUTY - this->dutyWarm;
    this->dutyCool = MAX_DUTY - this->dutyCool;
#endif
#ifdef DEBUG_SENSOR
        ESP_LOGI(TAG_SENSOR,"intensity: %"PRIu16, this->intensity);
        ESP_LOGI(TAG_SENSOR,"temp: %"PRIu16, this->temperature);
        ESP_LOGI(TAG_SENSOR,"dutyWarm: %"PRIu32, this->dutyWarm);
        ESP_LOGI(TAG_SENSOR,"dutyCool: %"PRIu32, this->dutyCool);
#endif
    if (fade)
    {
        esp_err_t ret = ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_0, this->dutyCool, this->fade_time);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 0 in fade mode");
#endif
            return ret;
        }
        ret = ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        xSemaphoreTake(this->semaphore, portMAX_DELAY);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 0 in fade mode");
#endif
            return ret;
        }
        ret = ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL_1, this->dutyWarm, this->fade_time);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 1 in fade mode");
#endif
            return ret;
        }
        ret = ledc_fade_start(LEDC_MODE, LEDC_CHANNEL_1,LEDC_FADE_WAIT_DONE);
        xSemaphoreTake(this->semaphore, portMAX_DELAY);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 1 in fade mode");
#endif
            return ret;
        }
        return ret;
    }
    else
    {
        ret = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, this->dutyCool);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 0");
#endif
            return ret;
        }
        ret = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 0");
#endif
            return ret;
        }
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, this->dutyWarm);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to set duty for LEDC channel 1");
#endif
            return ret;
        }
        ret = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);
        if (ret != ESP_OK)
        {
#ifdef DEBUG_SENSOR
            ESP_LOGE(TAG_SENSOR, "failed to update duty for LEDC channel 1");
#endif
            return ret;
        }
        return ret;
    }
}

uint16_t LedDriver::getTemperature()
{
    return this->temperature;
}

uint8_t LedDriver::getIntensity()
{
    return this->intensity;
}

uint32_t LedDriver::getDuty(int channel)
{
    if (channel == 0)
    {
        return this->dutyCool;
    }
    else
    {
        return this->dutyWarm;
    }
}
