#include "led_driver.hpp"

LedDriver::~LedDriver()
{
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_0, 0);
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_1, 0);
}

void LedDriver::updateLoop() {
    if(!is_started) {
        this->step_cool = (target_duty_cool - dutyCool) / 20;
        this->step_warm = (target_duty_warm - dutyWarm) / 20;
        this->is_started = true;
        this->fade_counter = 0;
    }

    this->dutyCool += step_cool;
    this->dutyWarm += step_warm;

    this->changePWM(this->dutyCool, this->dutyWarm);

    this->fade_counter++;

    if(fade_counter >= FADE_DURATION) {
        this->is_update = false;
        this->is_started = false;

        this->dutyCool = target_duty_cool;
        this->dutyWarm = target_duty_warm;

        this->changePWM(this->dutyCool, this->dutyWarm);
    }
}

void LedDriver::routine() {
    if(is_update) {
        if(state) {
            this->updateLoop();
        }
        else {
            if(this->dutyCool != 0 || this->dutyWarm != 0) {

                ESP_LOGI(TAG_SENSOR, "Turning off the light");
                
                // close the light
                // FIXME: the light will not turn on after, only if the intensity is changed or the temperature
                this->target_duty_cool = 0;
                this->target_duty_warm = 0;

                this->updateLoop();
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

led_driver_handle_t LedDriver::init() {
    return this->handle;
}

uint16_t LedDriver::get_temperature() {
    return temperature;
}

uint8_t LedDriver::get_intensity() {
    return intensity;
}

uint32_t LedDriver::get_duty(uint8_t channel) {
    if(channel == 0) {
        return dutyCool;
    }
    else { 
        return dutyWarm;
    }
}

uint16_t LedDriver::get_hue() {
    return 0;
}

uint8_t LedDriver::get_saturation() {
    return 0;
}

uint16_t LedDriver::get_x() {
    return 0;
}

uint16_t LedDriver::get_y() {
    return 0;
}

esp_err_t LedDriver::set_power(bool power) {
    this->state = power;
    this->toggleUpdate();

    return ESP_OK;
}

esp_err_t LedDriver::set_brightness(uint8_t brightness) {
    this->intensity_target = brightness;
    this->set_temperature(this->temperature);

    this->toggleUpdate();

    return ESP_OK;
}

esp_err_t LedDriver::set_color(uint16_t x, uint16_t y) {
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t LedDriver::set_hue(uint16_t hue) {
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t LedDriver::set_saturation(uint8_t saturation) {
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t LedDriver::set_temperature(uint32_t temperature) {
    esp_err_t ret;

    if(temperature < MIN_TEMPERATURE) {
        temperature = MIN_TEMPERATURE;

        this->target_duty_warm = 0;
        this->target_duty_cool = (MAX_DUTY * (float)((this->intensity_target) / 254.0));
    }
    else if(temperature > MAX_TEMPERATURE) {
        temperature = MAX_TEMPERATURE;

        this->target_duty_warm = (MAX_DUTY * (float)((this->intensity_target) / 254.0));
        this->target_duty_cool = 0;
    }
    else {
        this->temperature = (int32_t)temperature;
        ESP_LOGI(TAG_SENSOR,"temperature: %d", this->temperature);

        this->target_duty_warm = (int32_t)(MAX_DUTY * (float)((float)((this->intensity_target) / 254.0)) * (float)((MAX_TEMPERATURE - this->temperature) / RANGE_TEMPERATURE));
        ESP_LOGI(TAG_SENSOR,"target_duty_warm: %lu", this->target_duty_warm);
        this->target_duty_cool = (int32_t)(MAX_DUTY * (float)((float)((this->intensity_target) / 254.0)) * (float)((this->temperature - MIN_TEMPERATURE) / RANGE_TEMPERATURE));
        ESP_LOGI(TAG_SENSOR,"target_duty_cool: %lu", this->target_duty_cool);
    }
    
    #ifdef INVERSOR
        this->dutyWarm = MAX_DUTY - this->dutyWarm;
        this->dutyCool = MAX_DUTY - this->dutyCool;
    #endif
    #ifdef DEBUG_SENSOR
        ESP_LOGI(TAG_SENSOR,"intensity: %"PRIu16, this->intensity_target);
        ESP_LOGI(TAG_SENSOR,"temp: %"PRIu16, this->temperature);
        ESP_LOGI(TAG_SENSOR,"dutyWarm target: %"PRIu32, this->target_duty_warm);
        ESP_LOGI(TAG_SENSOR,"dutyCool target: %"PRIu32, this->target_duty_cool);
    #endif

    this->toggleUpdate();

    // Log the temperature and the value of is_update
    #ifdef DEBUG_SENSOR
        ESP_LOGI(TAG_SENSOR,"temperature: %d", this->temperature);
        ESP_LOGI(TAG_SENSOR,"is_update: %d", this->is_update);
    #endif

    return ret;
}

void LedDriver::led_routine() {
    routine();
}