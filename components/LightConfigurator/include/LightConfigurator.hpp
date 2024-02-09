#ifndef LIGHT_CONFIGURATOR_HPP
#define LIGHT_CONFIGURATOR_HPP

#include <stdio.h>

#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_ota.h>

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

endpoint_t* configureLight(extended_color_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node);
endpoint_t* configureLight(on_off_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node);
endpoint_t* configureLight(dimmable_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node);
endpoint_t* configureLight(color_temperature_light::config_t &light, uint8_t flags, void *priv_data, esp_matter::node_t *node);

#endif // LIGHT_CONFIGURATOR_HPP