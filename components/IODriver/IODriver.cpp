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

/* Do any conversions/remapping for the actual value here */
static esp_err_t app_driver_light_set_power(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    ESP_LOGE(TAG, "power: %d", val->val.b);
    return light_driver.set_power(val->val.b);
}

static esp_err_t app_driver_light_set_brightness(led_driver_handle_t handle, esp_matter_attr_val_t *val)
{
    int value = REMAP_TO_RANGE(val->val.u8, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);
    ESP_LOGE(TAG, "brightness: %d", value);
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
    if (endpoint_id == light_endpoint_id) {
        led_driver_handle_t handle = (led_driver_handle_t)driver_handle;
        ESP_LOGE(TAG, "light_endpoint_id");
        if (cluster_id == OnOff::Id) {
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                ESP_LOGE(TAG, "OnOff::Attributes::OnOff::Id");
                err = app_driver_light_set_power(handle, val);
            }
        } else if (cluster_id == LevelControl::Id) {
            if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
                ESP_LOGE(TAG, "LevelControl::Attributes::CurrentLevel::Id");
                err = app_driver_light_set_brightness(handle, val);
            }
        } else if (cluster_id == ColorControl::Id) {
            ESP_LOGE(TAG, "ColorControl::Id");
            ESP_LOGE(TAG, "attribute_id: %lu", attribute_id);
            if (attribute_id == ColorControl::Attributes::CurrentHue::Id) {
                err = app_driver_light_set_hue(handle, val);
            } else if (attribute_id == ColorControl::Attributes::CurrentSaturation::Id) {
                err = app_driver_light_set_saturation(handle, val);
            } else if (attribute_id == ColorControl::Attributes::ColorTemperatureMireds::Id) {
                err = app_driver_light_set_temperature(handle, val);
            }

            if (attribute_id == ColorControl::Attributes::CurrentX::Id) {
                ESP_LOGI(TAG, "ColorControl::Attributes::CurrentX::Id");
            } else if (attribute_id == ColorControl::Attributes::CurrentY::Id) {
                ESP_LOGI(TAG, "ColorControl::Attributes::CurrentY::Id");
            }
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
    attribute::get_val(attribute, &val);
    err |= app_driver_light_set_brightness(handle, &val);

    /* Setting color */
    cluster = cluster::get(endpoint, ColorControl::Id);
    attribute = attribute::get(cluster, ColorControl::Attributes::ColorMode::Id);
    attribute::get_val(attribute, &val);
    if (val.val.u8 == (uint8_t)ColorControl::ColorMode::EMBER_ZCL_COLOR_MODE_CURRENT_HUE_AND_CURRENT_SATURATION) {
        /* Setting hue */
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentHue::Id);
        attribute::get_val(attribute, &val);
        err |= app_driver_light_set_hue(handle, &val);
        /* Setting saturation */
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentSaturation::Id);
        attribute::get_val(attribute, &val);
        err |= app_driver_light_set_saturation(handle, &val);
    } else if (val.val.u8 == (uint8_t)ColorControl::ColorMode::EMBER_ZCL_COLOR_MODE_COLOR_TEMPERATURE) {
        /* Setting temperature */
        attribute = attribute::get(cluster, ColorControl::Attributes::ColorTemperatureMireds::Id);
        attribute::get_val(attribute, &val);
        err |= app_driver_light_set_temperature(handle, &val);
    } else {
        ESP_LOGI(TAG, "Color mode to be implemented");
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentX::Id);
        attribute::get_val(attribute, &val);
        ESP_LOGI(TAG, "CurrentX: %d", val.val.u16);
        attribute = attribute::get(cluster, ColorControl::Attributes::CurrentY::Id);
        attribute::get_val(attribute, &val);
        ESP_LOGI(TAG, "CurrentY: %d", val.val.u16);
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