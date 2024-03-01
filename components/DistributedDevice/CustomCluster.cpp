#include "CustomCluster.hpp"

namespace esp_matter
{
    namespace cluster
    {
        namespace distributed_device
        {
            const function_generic_t function_list[] = {};
            const int function_flags = CLUSTER_FLAG_NONE;

            cluster_t *create(endpoint_t *endpoint, config_t *config_, uint8_t flags)
            {
                cluster_t *cluster = cluster::create(endpoint, DistributedDevice::Id, flags);
                if (!cluster)
                {
                    ESP_LOGE(TAG_CUSTOM_CLUSTER, "Could not create cluster");
                    return NULL;
                }

                ESP_LOGI(TAG_CUSTOM_CLUSTER, "Attribute");
                attribute::create(cluster, Globals::Attributes::ClusterRevision::Id, ATTRIBUTE_FLAG_NONE, esp_matter_uint16(config_->cluster_revision));
                ESP_LOGI(TAG_CUSTOM_CLUSTER, "Attribute created");
                // attribute::create(cluster, DistributedDevice::Id, ATTRIBUTE_FLAG_WRITABLE, esp_matter_char_str(config_->automations, sizeof(config_->automations)));

                // command::create(cluster, chip::Clusters::DistributedDevice::command::add_automation::Id, COMMAND_FLAG_ACCEPTED | COMMAND_FLAG_CUSTOM, add_command_callback);
                return cluster;
            }

        }
    }
}
