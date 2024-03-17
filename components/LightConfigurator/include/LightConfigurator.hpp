#ifndef LIGHT_CONFIGURATOR_HPP
#define LIGHT_CONFIGURATOR_HPP

#include <stdio.h>

#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_ota.h>

#define ON_OFF_TYPE_LIGHT 0
#define DIMMABLE_TYPE_LIGHT 1
#define COLOR_TEMPERATURE_TYPE_LIGHT 2
#define COLOR_TYPE_LIGHT 3

#define LIGHT_TYPE COLOR_TEMPERATURE_TYPE_LIGHT

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace esp_matter::cluster;


endpoint_t* configureLight(uint8_t flags, void *priv_data, esp_matter::node_t *node);

#endif // LIGHT_CONFIGURATOR_HPP