#include "LightConfigurator.hpp"

static const char *TAG_CONFIGURATOR = "Configurator";

endpoint_t* priv_endpoint;
extern uint16_t light_endpoint_id;

esp_err_t configureLight(uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    priv_endpoint = endpoint::create(node, ENDPOINT_FLAG_NONE, priv_data);

    #if LIGHT_TYPE == ON_OFF_TYPE_LIGHT

        descriptor::create(priv_endpoint, CLUSTER_FLAG_SERVER);

        endpoint::add_device_type(priv_endpoint, on_off_light::get_device_type_id(), on_off_light::get_device_type_version());

    #elif LIGHT_TYPE == DIMMABLE_TYPE_LIGHT

        descriptor::create(priv_endpoint, CLUSTER_FLAG_SERVER);

        endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, on_off_light::get_device_type_id(), on_off_light::get_device_type_version());

    #elif LIGHT_TYPE == COLOR_TEMPERATURE_TYPE_LIGHT

        color_temperature_light::config_t temp_config;
        temp_config.on_off.on_off = false;
        temp_config.on_off.lighting.start_up_on_off = nullptr;
        temp_config.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        temp_config.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        temp_config.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;
        temp_config.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        temp_config.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        temp_config.color_control.color_temperature.startup_color_temperature_mireds = 217; // 217 is 4600 K

        color_temperature_light::add(priv_endpoint, &temp_config);

    #elif LIGHT_TYPE == COLOR_TYPE_LIGHT

        descriptor::create(priv_endpoint, CLUSTER_FLAG_SERVER);
        
        endpoint::add_device_type(priv_endpoint, extended_color_light::get_device_type_id(), extended_color_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, color_temperature_light::get_device_type_id(), color_temperature_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, on_off_light::get_device_type_id(), on_off_light::get_device_type_version());

    #endif

    #ifdef DEBUG_TAG
        ESP_LOGI(TAG_CONFIGURATOR, "temp_config created with endpoint_id %d", light_endpoint_id);
    #endif

    return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
}