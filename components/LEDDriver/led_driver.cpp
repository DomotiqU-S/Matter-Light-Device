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
        setIntensity(this->intensity);
    }
    else {
        setLevel(0, 0);
    }

    return ESP_OK;
}

esp_err_t LedDriver::setIntensity(uint8_t intensity)            
{
    this->intensity = intensity;
    this->state = this->intensity > 0 ? true : false;
    if(!this->state) {
        return setLevel(0, 0);
    }
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
    BaseType_t res = xTaskCreate(changeLevel, "changeLevel", 2048, (void *) duties, 6, NULL);
    return res == pdPASS ? ESP_OK : ESP_FAIL;
}

void LedDriver::changeLevel(void *pvParameters)
{
    ledsDuty *duties = (ledsDuty *)pvParameters;

    int fadeSteps = FADE_STEP / 10;
    int fadeStep1 = (duties->newDutyCool - duties->previousDutyCool) / fadeSteps;
    int fadeStep2 = (duties->newDutyWarm - duties->previousDutyWarm) / fadeSteps;

    int reminder1 = (duties->newDutyCool - duties->previousDutyCool) % fadeSteps;
    int reminder2 = (duties->newDutyWarm - duties->previousDutyWarm) % fadeSteps;

    int dutyCycle1 = duties->previousDutyCool;
    int dutyCycle2 = duties->previousDutyWarm;

    for (int i = 0; i < fadeSteps + 1; i++) {
        dutyCycle1 = duties->previousDutyCool + (i * fadeStep1);
        dutyCycle2 = duties->previousDutyWarm + (i * fadeStep2);

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, dutyCycle1);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_1, dutyCycle2);

        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_1);

        #ifdef FADE_ENABLE
            vTaskDelay(FADE_INTERVAL);
        #endif
    }

    dutyCycle1 += reminder1;
    dutyCycle2 += reminder2;

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