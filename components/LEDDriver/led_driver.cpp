#include "led_driver.hpp"

LedDriver::~LedDriver()
{
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_0, 0);
    ledc_stop(LEDC_MODE, LEDC_CHANNEL_1, 0);
}

esp_err_t LedDriver::switchState(bool state)
{
    ESP_LOGI(TAG_SENSOR, "Switching state to %d", state);
    this->state = state;
    
    if(state) {
        setIntensity(this->intensity);
    }
    else {
        setLevel(0, 0);
    }

    return ESP_OK;
}

esp_err_t LedDriver::setIntensity(uint8_t intensity)            
{
    ESP_LOGI(TAG_SENSOR, "Setting intensity to %d", intensity);
    this->intensity = intensity;
    this->state = this->intensity > 0 ? true : false;
    if(!this->state) {
        return setLevel(0, 0);
    }
    return this->setTemperature(this->temperature);
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
        this->dutyWarm = (uint32_t)0;
        this->dutyCool = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 254.0));
    }
    else if (this->temperature == MAX_TEMPERATURE)
    {
        this->dutyWarm = (uint32_t)(MAX_DUTY * (float)((this->intensity) / 254.0));
        this->dutyCool = (uint32_t)0;
    }
    else
    {
        this->dutyWarm = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 254.0)) * (float)((MAX_TEMPERATURE - this->temperature) / RANGE_TEMPERATURE));
        this->dutyCool = (uint32_t)(MAX_DUTY * (float)((float)((this->intensity) / 254.0)) * (float)((this->temperature - MIN_TEMPERATURE) / RANGE_TEMPERATURE));
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

    // execute the change
    ret = setLevel(this->dutyCool, this->dutyWarm);

    return ret;
}

esp_err_t LedDriver::setLevel(uint32_t _dutyCool, uint32_t _dutyWarm)
{
    duties->newDutyCool = _dutyCool;
    duties->newDutyWarm = _dutyWarm;
    duties->previousDutyCool = previousDutyCool;
    duties->previousDutyWarm = previousDutyWarm;

    previousDutyCool = _dutyCool;
    previousDutyWarm = _dutyWarm;

    // Create a task to change the level of the LED
    BaseType_t res = xTaskCreate(changeLevel, "changeLevel", 2048, (void *) duties, 3, NULL);
    return res == pdPASS ? ESP_OK : ESP_FAIL;
}

void LedDriver::changeLevel(void *pvParameters)
{
    ledsDuty *duties = (ledsDuty *)pvParameters;

    #ifdef FADE_ENABLE
        int fadeSteps = FADE_STEP / 10;
        int fadeStep1 = (duties->newDutyCool - duties->previousDutyCool) / fadeSteps;
        int fadeStep2 = (duties->newDutyWarm - duties->previousDutyWarm) / fadeSteps;

        for (int i = 0; i < fadeSteps + 1; i++) {
            uint32_t dutyCycle1 = duties->previousDutyCool + (i * fadeStep1);
            uint32_t dutyCycle2 = duties->previousDutyWarm + (i * fadeStep2);

            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, dutyCycle1);
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, dutyCycle2);

            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);

            vTaskDelay(FADE_INTERVAL);
        }
    #endif

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, duties->newDutyCool);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, duties->newDutyWarm);

    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);

    // Destroy the task after the execution
    vTaskDelete(NULL);
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