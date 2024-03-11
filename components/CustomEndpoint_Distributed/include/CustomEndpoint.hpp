#ifndef CUSTOM_ENDPOINT_HPP
#define CUSTOM_ENDPOINT_HPP

// See https://github.com/espressif/esp-matter/blob/d4cbddc4d385214ca520a3002372b6b948681ca0/components/esp_matter/esp_matter_endpoint.h
#include <stdio.h>
#include "CustomCluster.hpp"

// using namespace esp_matter;
using namespace chip::app::Clusters;
// using namespace esp_matter::cluster;

#define DISTRIBUTED_DEVICE_TYPE_ID 0x0000
#define DISTRIBUTED_DEVICE_TYPE_VERSION 1

#define TAG_CUSTOM_ENDPOINT "CustomEndpoint"

namespace esp_matter
{
    namespace endpoint
    {
        namespace distributed_device
        {
            typedef struct config_t
            {
                // cluster::descriptor::config_t descriptor;
                // cluster::identify::config_t identify;
                cluster::distributed_device::config_t distributed_device;
                cluster::on_off::config_t on_off;
            } config_t;
            uint32_t get_device_type_id();
            uint8_t get_device_type_version();
            endpoint_t *create(node_t *node, config_t *config_, uint8_t flags, void *priv_data);
            esp_err_t add(endpoint_t *endpoint, config_t *config_);
        }
    }
}

#endif // CUSTOM_ENDPOINT_HPP
