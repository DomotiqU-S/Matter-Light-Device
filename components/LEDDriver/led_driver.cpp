#include "led_driver.hpp"

LedDriver::~LedDriver()
{
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_0, 0);
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_1, 0);
}

esp_err_t LedDriver::switchState(bool state)
{
    this->state = state;
    
    if(state) {
        setIntensityTarget(this->intensity);
    }
    else {
        setLevel(0, 0);
    }

    return ESP_OK;
}

esp_err_t LedDriver::setIntensityTarget(uint8_t intensity)            
{
    this->intensity_target = intensity;

    return ESP_OK;
}

esp_err_t LedDriver::setTemperature(uint16_t temperature)
{
    ESP_LOGI(TAG_SENSOR, "Setting temperature to %d", temperature);
    esp_err_t ret;
    if(temperature < MIN_TEMPERATURE) {
        temperature = MIN_TEMPERATURE;
    }
    else if(temperature > MAX_TEMPERATURE) {
        temperature = MAX_TEMPERATURE;
    }
    else {
        this->temperature = temperature;
    }
    
    if (this->temperature == MIN_TEMPERATURE)
    {
        this->target_duty_warm = (uint32_t)0;
        this->target_duty_cool = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 254.0));
    }
    else if (this->temperature == MAX_TEMPERATURE)
    {
        this->target_duty_warm = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 254.0));
        this->target_duty_cool = (uint32_t)0;
    }
    else
    {
        this->target_duty_warm = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 254.0)) * (float)((MAX_TEMPERATURE - this->temperature) / RANGE_TEMPERATURE));
        this->target_duty_cool = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 254.0)) * (float)((this->temperature - MIN_TEMPERATURE) / RANGE_TEMPERATURE));
    }
        #ifdef INVERSOR
            this->dutyWarm = MAX_DUTY - this->dutyWarm;
            this->dutyCool = MAX_DUTY - this->dutyCool;
        #endif
        #ifdef DEBUG_SENSOR
            ESP_LOGI(TAG_SENSOR,"intensity: %"PRIu16, this->intensity);
            ESP_LOGI(TAG_SENSOR,"temp: %"PRIu16, this->temperature);
            ESP_LOGI(TAG_SENSOR,"dutyWarm target: %"PRIu32, this->target_duty_cool);
            ESP_LOGI(TAG_SENSOR,"dutyCool target: %"PRIu32, this->target_duty_cool);
        #endif

    return ret;
}

esp_err_t LedDriver::setLevel(uint32_t _dutyCool, uint32_t _dutyWarm)
{
    return ESP_OK;
}

uint32_t LedDriver::getDuty(uint8_t channel) {
    if(channel == 0) {
        return dutyCool;
    }
    else { 
        return dutyWarm;
    }
}

uint16_t LedDriver::getTemperature() {
    return temperature;
}

uint8_t LedDriver::getIntensity() {
    return intensity;
}

void LedDriver::routine() {
    if(is_update) {
        if(state) {
            if(!is_started) {
                this->step_cool = (target_duty_cool - dutyCool) / 50;
                this->step_warm = (target_duty_warm - dutyWarm) / 50;
                this->is_started = true;
                this->fade_counter = 0;
            }

            this->dutyCool += step_cool;
            this->dutyWarm += step_warm;

            this->changePWM(dutyCool, dutyWarm);

            this->fade_counter++;

            if(fade_counter == FADE_DURATION) {
                this->is_update = false;
                this->is_started = false;

                this->dutyCool = target_duty_cool;
                this->dutyWarm = target_duty_warm;

                this->changePWM(dutyCool, dutyWarm);
            }
        }
        else {
            if(this->dutyCool != 0 || this->dutyWarm != 0) {
                // clsoe the light
            }
        }
    }
}

void LedDriver::toggleUpdate() {
    is_update = true;
}

void LedDriver::changePWM(uint32_t dutyCool, uint32_t dutyWarm) {
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, dutyCool);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, dutyWarm);

    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);
}