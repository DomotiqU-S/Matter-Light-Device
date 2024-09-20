#include "LightConfigurator.hpp"

static const char *TAG_CONFIGURATOR = "Configurator";

endpoint_t* priv_endpoint;
extern uint16_t light_endpoint_id;

esp_err_t configureLight(uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    priv_endpoint = endpoint::create(node, ENDPOINT_FLAG_NONE, priv_data);

    #if LIGHT_TYPE == ON_OFF_TYPE_LIGHT

        on_off_light::config_t on_off_config;

        on_off_config.on_off.on_off = false;
        on_off_config.lighting.start_up_on_off = nullptr;

        on_off_light::add(priv_endpoint, &on_off_config);

    #elif LIGHT_TYPE == DIMMABLE_TYPE_LIGHT

        dimmable_light::config_t dimmable_config;

        dimmable_config.on_off.on_off = false;
        dimmable_config.on_off.lighting.start_up_on_off = nullptr;
        dimmable_config.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        dimmable_config.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        dimmable_config.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;

        dimmable_light::add(priv_endpoint, &dimmable_config);

    #elif LIGHT_TYPE == COLOR_TEMPERATURE_TYPE_LIGHT

        color_temperature_light::config_t temp_config;

        temp_config.on_off.on_off = false;
        temp_config.on_off.lighting.start_up_on_off = nullptr;
        temp_config.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        temp_config.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        temp_config.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;

        // Color settings
        temp_config.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        temp_config.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
        temp_config.color_control.color_temperature.startup_color_temperature_mireds = 217; // 217 is 4600 K

        color_temperature_light::add(priv_endpoint, &temp_config);

    #elif LIGHT_TYPE == COLOR_TYPE_LIGHT

        extended_color_light::config_t color_config;

        color_config.on_off.on_off = false;
        color_config.on_off.lighting.start_up_on_off = nullptr;
        color_config.level_control.current_level = DEFAULT_LEVEL_LIGHT;
        color_config.level_control.on_level = DEFAULT_LEVEL_LIGHT;
        color_config.level_control.lighting.start_up_current_level = DEFAULT_LEVEL_LIGHT;

        // Color settings
        // Set the color mode to default(XY)
        color_config.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kXY;
        color_config.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kXY;
        color_config.color_control.color_temperature.startup_color_temperature_mireds = 217; // 217 is 4600 K

        extended_color_light::add(priv_endpoint, &color_config);

    #endif

    return priv_endpoint == nullptr ? ESP_FAIL : ESP_OK;
}