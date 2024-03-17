#include "LightConfigurator.hpp"

endpoint_t* priv_endpoint;

endpoint_t* configureLight(uint8_t flags, void *priv_data, esp_matter::node_t *node)
{
    free(priv_endpoint);

    esp_matter::cluster::on_off::config_t on_off_config;
    esp_matter::cluster::level_control::config_t level_control_config;
    esp_matter::cluster::color_control::config_t color_control_config;

    extended_color_light::config_t extended_color_light_config;

    priv_endpoint = endpoint::create(node, flags, priv_data);
    descriptor::create(priv_endpoint, &extended_color_light_config.descriptor, CLUSTER_FLAG_SERVER);

    endpoint::add_device_type(priv_endpoint, on_off_light::get_device_type_id(), on_off_light::get_device_type_version());
    on_off::create(priv_endpoint, &on_off_config, CLUSTER_FLAG_SERVER, on_off::feature::lighting::get_id());

    #if LIGHT_TYPE == COLOR_TEMPERATURE_TYPE_LIGHT
        endpoint::add_device_type(priv_endpoint, color_temperature_light::get_device_type_id(), color_temperature_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());

        level_control::create(priv_endpoint, &level_control_config, CLUSTER_FLAG_SERVER, level_control::feature::lighting::get_id());
        color_control::create(priv_endpoint, &color_control_config, CLUSTER_FLAG_SERVER, color_control::feature::color_temperature::get_id());

    #elif LIGHT_TYPE == COLOR_TYPE_LIGHT
        endpoint::add_device_type(priv_endpoint, extended_color_light::get_device_type_id(), extended_color_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, color_temperature_light::get_device_type_id(), color_temperature_light::get_device_type_version());
        endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());

        level_control::create(priv_endpoint, &level_control_config, CLUSTER_FLAG_SERVER, level_control::feature::lighting::get_id());
        color_control::create(priv_endpoint, &color_control_config, CLUSTER_FLAG_SERVER, color_control::feature::xy::get_id());

    #elif LIGHT_TYPE == DIMMABLE_TYPE_LIGHT
        endpoint::add_device_type(priv_endpoint, dimmable_light::get_device_type_id(), dimmable_light::get_device_type_version());
        level_control::create(priv_endpoint, &level_control_config, CLUSTER_FLAG_SERVER, level_control::feature::lighting::get_id());

    #else
        #error "Invalid LIGHT_TYPE"
    #endif

    return priv_endpoint;
}