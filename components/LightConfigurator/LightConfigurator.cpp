#include "LightConfigurator.hpp"

endpoint_t* priv_endpoint;

endpoint_t* configureLight(extended_color_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    light.on_off.on_off = false;
    light.on_off.lighting.start_up_on_off = nullptr;
    light.level_control.current_level = 100;
    light.level_control.lighting.start_up_current_level = 100;
    light.color_control.color_mode = (uint8_t)(uint8_t)ColorControl::ColorMode::kCurrentXAndCurrentY;
    light.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kCurrentXAndCurrentY;
    light.color_control.xy.current_x = 0x616b;
    light.color_control.xy.current_y = 0x607d;

    priv_endpoint =  extended_color_light::create(node, &light, flags, priv_data);

    return priv_endpoint;
}

endpoint_t* configureLight(on_off_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    light.on_off.on_off = false;
    light.on_off.lighting.start_up_on_off = nullptr;

    priv_endpoint = on_off_light::create(node, &light, flags, priv_endpoint);

    return priv_endpoint;
}

endpoint_t* configureLight(dimmable_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    light.on_off.on_off = false;
    light.on_off.on_off = false;
    light.on_off.lighting.start_up_on_off = nullptr;
    light.level_control.current_level = 100;
    light.level_control.lighting.start_up_current_level = 100;

    priv_endpoint = dimmable_light::create(node, &light, flags, priv_data);
    printf("priv_endpoint: %d\n", priv_endpoint != nullptr);

    return priv_endpoint;
}

endpoint_t* configureLight(color_temperature_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    light.on_off.on_off = false;
    light.on_off.lighting.start_up_on_off = nullptr;
    light.level_control.current_level = 100;
    light.level_control.lighting.start_up_current_level = 100;
    light.color_control.color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
    light.color_control.enhanced_color_mode = (uint8_t)ColorControl::ColorMode::kColorTemperature;
    light.color_control.color_temperature.startup_color_temperature_mireds = nullptr;

    priv_endpoint = color_temperature_light::create(node, &light, flags, priv_data);

    return priv_endpoint;
}
