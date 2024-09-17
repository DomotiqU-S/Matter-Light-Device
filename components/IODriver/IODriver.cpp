#include "driver/gpio.h"
#include <esp_log.h>
#include "IODriver.hpp"
#include <esp_matter.h>
#include "LightDriver.hpp"
#include <stdlib.h>
#include <string.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "io_driver";
extern uint16_t light_endpoint_id;

LightDriver light_driver(5, 6);

esp_err_t set_color_from_attribute(uint32_t attribute_id, esp_matter_attr_val_t *val) {
    esp_err_t err = ESP_OK;
    uint32_t temperature = 0;
    
    #ifdef DEBUG_DRIVER
        ESP_LOGI(TAG, "ColorControl::Id");
        ESP_LOGI(TAG, "attribute_id: %lu", attribute_id);
    #endif

    switch(attribute_id) {
        case ColorControl::Attributes::CurrentX::Id:
                err = light_driver.set_color(val->val.u16, 0);
            break;
        case ColorControl::Attributes::CurrentY::Id:
                err = light_driver.set_color(0, val->val.u16);
            break;
        case ColorControl::Attributes::CurrentHue::Id:
                #ifdef DEBUG_DRIVER
                    ESP_LOGI(TAG, "HUE value: %u", val->val.u16);
                #endif
                err = light_driver.set_hue(val->val.u16);
            break;
        case ColorControl::Attributes::CurrentSaturation::Id:
                #ifdef DEBUG_DRIVER
                    ESP_LOGI(TAG, "Saturation value: %u", val->val.u16);
                #endif
                err = light_driver.set_saturation(val->val.u16);
            break;
        case ColorControl::Attributes::ColorTemperatureMireds::Id:
                temperature = REMAP_TO_RANGE_INVERSE(val->val.u32, STANDARD_TEMPERATURE_FACTOR);
                #ifdef DEBUG_DRIVER
                    ESP_LOGI(TAG, "TEMP value: %lu", temperature);
                #endif
                err = light_driver.set_temperature(temperature);
            break;
        default:
            break;
    }

    return err;
}

/* Do any conversions/remapping for the actual value here */
static esp_err_t app_driver_light_set_power(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    #ifdef DEBUG_DRIVER
        ESP_LOGE(TAG, "power: %d", val->val.b);
    #endif
    return light_driver.set_power(val->val.b);
}

static esp_err_t app_driver_light_set_brightness(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    int value = REMAP_TO_RANGE(val->val.u8, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
    #ifdef DEBUG_DRIVER
        ESP_LOGE(TAG, "brightness: %d", value);
    #endif
    return light_driver.set_brightness((uint16_t)value); //led_driver_set_brightness(handle, value);
}

static esp_err_t app_driver_light_set_hue(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    int value = REMAP_TO_RANGE(val->val.u8, MATTER_HUE, STANDARD_HUE);
    return light_driver.set_hue((uint16_t)value); //led_driver_set_hue(handle, value);
}

static esp_err_t app_driver_light_set_saturation(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    int value = REMAP_TO_RANGE(val->val.u8, MATTER_SATURATION, STANDARD_SATURATION);
    return light_driver.set_saturation((uint8_t)value); //led_driver_set_saturation(handle, value);
}

static esp_err_t app_driver_light_set_temperature(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    // Remap the temperature value to the range supported by the LED driver: Matter Application Cluster Doc: 0-0xFEFF Temperature Range 
    // Default value: 0xFA (4000K)
    uint32_t value = REMAP_TO_RANGE_INVERSE(val->val.u16, STANDARD_TEMPERATURE_FACTOR);
    return light_driver.set_temperature(value);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    led_driver_handle_t handle = (led_driver_handle_t)driver_handle;

    if (endpoint_id == light_endpoint_id) {
        switch(cluster_id) {
            case OnOff::Id:
                #ifdef DEBUG_DRIVER
                    ESP_LOGI(TAG, "OnOff::Id");
                #endif
                if(attribute_id == OnOff::Attributes::OnOff::Id) {
                    err = light_driver.set_power(val->val.b);
                }
                break;
            case LevelControl::Id:
                #ifdef DEBUG_DRIVER
                    ESP_LOGI(TAG, "LevelControl::Attributes::CurrentLevel::Id");
                #endif
                if(attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
                    err = light_driver.set_brightness(val->val.u8);
                }
                break;
            case ColorControl::Id:
                // Control temperature
                // Control Hue & saturation
                // Control XY color
                err = set_color_from_attribute(attribute_id, val);
                break;
            default:
                break;
        }
    }
    ESP_LOGI(TAG, "Endpoint ID: %d", endpoint_id);
    return err;
}


esp_err_t app_driver_light_set_defaults(uint16_t endpoint_id)
{
    esp_err_t err = ESP_OK;
    void *priv_data = endpoint::get_priv_data(endpoint_id);
    led_driver_handle_t handle = (led_driver_handle_t)priv_data;
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = NULL;
    attribute_t *attribute = NULL;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    /* Setting brightness */
    cluster = cluster::get(endpoint, LevelControl::Id);
    attribute = attribute::get(cluster, LevelControl::Attributes::CurrentLevel::Id);
    // attribute::get_val(attribute, &val);
    val.val.u8 = DEFAULT_BRIGHTNESS;
    err |= app_driver_light_set_brightness(handle, &val);

    /* Setting color */
    cluster = cluster::get(endpoint, ColorControl::Id);
    attribute = attribute::get(cluster, ColorControl::Attributes::ColorMode::Id);
    attribute::get_val(attribute, &val);

    if (val.val.u8 == (uint8_t)ColorControl::ColorMode::kCurrentHueAndCurrentSaturation) {
        /* Setting hue */
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentHue::Id);
        // attribute::get_val(attribute, &val);
        val.val.u8 = DEFAULT_HUE;
        err |= app_driver_light_set_hue(handle, &val);
        /* Setting saturation */
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentSaturation::Id);
        // attribute::get_val(attribute, &val);
        val.val.u8 = DEFAULT_SATURATION;
        err |= app_driver_light_set_saturation(handle, &val);
    } 
    else if (val.val.u8 == (uint8_t)ColorControl::ColorMode::kColorTemperature) {
        /* Setting temperature */
        attribute = attribute::get(cluster, ColorControl::Attributes::ColorTemperatureMireds::Id);
        // attribute::get_val(attribute, &val);
        val.val.u16 = DEFAULT_TEMPERATURE;
        err |= app_driver_light_set_temperature(handle, &val);
    }

    /* Setting power */
    cluster = cluster::get(endpoint, OnOff::Id);
    attribute = attribute::get(cluster, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);
    err |= app_driver_light_set_power(handle, &val);

    return err;
}

app_driver_handle_t app_driver_light_init()
{
    /* Initialize led */
    // TODO: Add LED driver 
    led_driver_handle_t handle = light_driver.init();
    return (app_driver_handle_t)handle;
}