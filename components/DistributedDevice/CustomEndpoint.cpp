#include "CustomEndpoint.hpp"

namespace esp_matter
{
    namespace endpoint
    {
        namespace distributed_device
        {
            uint32_t get_device_type_id()
            {
                return DISTRIBUTED_DEVICE_TYPE_ID;
            }

            uint8_t get_device_type_version()
            {
                return DISTRIBUTED_DEVICE_TYPE_VERSION;
            }

            endpoint_t *create(node_t *node, config_t *config_, uint8_t flags, void *priv_data)
            {
                ESP_LOGI(TAG_CUSTOM_ENDPOINT, "create");
                endpoint_t *endpoint = endpoint::create(node, flags, priv_data);
                add(endpoint, config_);
                ESP_LOGI(TAG_CUSTOM_ENDPOINT, "created");
                return endpoint;
            }

            esp_err_t add(endpoint_t *endpoint, config_t *config)
            {
                if (!endpoint)
                {
                    ESP_LOGE(TAG_CUSTOM_ENDPOINT, "Endpoint cannot be NULL");
                    return ESP_ERR_INVALID_ARG;
                }
                esp_err_t err = add_device_type(endpoint, get_device_type_id(), get_device_type_version());
                if (err != ESP_OK)
                {
                    ESP_LOGE(TAG_CUSTOM_ENDPOINT, "Failed to add device type id:%" PRIu32 ",err: %d", get_device_type_id(), err);
                    return err;
                }

                // cluster::descriptor::create(endpoint, &(config->descriptor), CLUSTER_FLAG_SERVER);
                // cluster_t *identify_cluster = cluster::identify::create(endpoint, &(config->identify), CLUSTER_FLAG_SERVER);
                // cluster::identify::command::create_trigger_effect(identify_cluster);

                cluster::distributed_device::create(endpoint, &(config->distributed_device), CLUSTER_FLAG_SERVER);

                return ESP_OK;
            }

        }
    }
}