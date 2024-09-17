#include "LightConfigurator.hpp"

static const char *TAG_CONFIGURATOR = "Configurator";

endpoint_t* priv_endpoint;
extern uint16_t light_endpoint_id;

esp_err_t configureLight(uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    #if LIGHT_TYPE == ON_OFF_TYPE_LIGHT
        on_off_light::config_t light;
        
        light.on_off.on_off = false;
        light.on_off.lighting.start_up_on_off = nullptr;

        priv_endpoint = on_off_light::create(node, &light, flags, priv_data);

    #elif LIGHT_TYPE == DIMMABLE_TYPE_LIGHT

        light.on_off.on_off = false;
        light.on_off.lighting.start_up_on_off = nullptr;
        light.level_control.current_level = 100;
        light.level_control.lighting.start_up_current_level = 100;

        priv_endpoint = dimmable_light::create(node, &light, flags, priv_data);

    #elif LIGHT_TYPE == COLOR_TEMPERATURE_TYPE_LIGHT
        color_temperature_light::config_t light;

        light.on_off.on_off = false;
        light.on_off.lighting.start_up_on_off = nullptr;
        light.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        light.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        light.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;
        light.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        light.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        light.color_control.color_temperature.startup_color_temperature_mireds = 217; // 217 is 4600 K

        priv_endpoint = color_temperature_light::create(node, &light, flags, priv_data);

    #elif LIGHT_TYPE == COLOR_TYPE_LIGHT

        extended_color_light::config_t light;

        light.on_off.on_off = false;
        light.on_off.lighting.start_up_on_off = nullptr;
        light.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        light.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        light.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;
        light.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kCurrentXAndCurrentY;
        light.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kCurrentXAndCurrentY;
        light.color_control.xy.current_x = 0x616b;
        light.color_control.xy.current_y = 0x607d;

        priv_endpoint = extended_color_light::create(node, &light, flags, priv_data);

    #endif

    #ifdef DEBUG_TAG
        ESP_LOGI(TAG_CONFIGURATOR, "Light created with endpoint_id %d", light_endpoint_id);
    #endif

    return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
}

// esp_err_t configureLight(on_off_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
// {
//     free(priv_endpoint);

//     light.on_off.on_off = false;
//     light.on_off.lighting.start_up_on_off = nullptr;

//     priv_endpoint = on_off_light::create(node, &light, flags, priv_endpoint);

//     #ifdef DEBUG_TAG
//         light_endpoint_id = endpoint::get_id(priv_endpoint);
//         ESP_LOGI(TAG_CONFIGURATOR, "Light created with endpoint_id %d", light_endpoint_id);
//     #endif

//     return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
// }

// esp_err_t configureLight(dimmable_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
// {
//     free(priv_endpoint);

//     light.on_off.on_off = true;
//     light.on_off.lighting.start_up_on_off = nullptr;
//     light.level_control.current_level = 100;
//     light.level_control.lighting.start_up_current_level = 100;

//     priv_endpoint = dimmable_light::create(node, &light, flags, priv_data);

//     #ifdef DEBUG_TAG
//         light_endpoint_id = endpoint::get_id(priv_endpoint);
//         ESP_LOGI(TAG_CONFIGURATOR, "Light created with endpoint_id %d", light_endpoint_id);
//     #endif

//     return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
// }

// esp_err_t configureLight(color_temperature_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
// {
//     free(priv_endpoint);

//     esp_matter::cluster::on_off::config_t on_off_light_config;
//     esp_matter::cluster::level_control::config_t dimmable_light_config;

//     priv_endpoint = endpoint::create(node, flags, priv_data);
//     descriptor::create(priv_endpoint, &light.descriptor, CLUSTER_FLAG_SERVER);
//     endpoint::add_device_type(priv_endpoint, color_temperature_light::get_device_type_id(), color_temperature_light::get_device_type_version());
//     endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());
//     endpoint::add_device_type(priv_endpoint, on_off_light::get_device_type_id(), on_off_light::get_device_type_version());

//     on_off::create(priv_endpoint, &light.on_off, CLUSTER_FLAG_SERVER, on_off::feature::lighting::get_id());
//     level_control::create(priv_endpoint, &light.level_control, CLUSTER_FLAG_SERVER, level_control::feature::on_off::get_id() | level_control::feature::lighting::get_id());
//     color_control::create(priv_endpoint, &light.color_control, CLUSTER_FLAG_SERVER, color_control::feature::color_temperature::get_id());

//     #ifdef DEBUG_TAG
//         light_endpoint_id = endpoint::get_id(priv_endpoint);
//         ESP_LOGI(TAG_CONFIGURATOR, "Light created with endpoint_id %d", light_endpoint_id);
//     #endif

//     return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
// }