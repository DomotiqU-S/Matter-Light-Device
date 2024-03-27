#include "CustomCluster.hpp"

namespace esp_matter
{
    namespace cluster
    {
        namespace distributed_device
        {
            const function_generic_t function_list[] = {};
            const int function_flags = CLUSTER_FLAG_NONE;

            // esp_err_t add_automation_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr)
            // {
            //     ESP_LOGI(TAG_CUSTOM_CLUSTER, "add_automation_callback");
            //     return ESP_OK;
            // }

            // esp_err_t remove_automation_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr)
            // {
            //     ESP_LOGI(TAG_CUSTOM_CLUSTER, "remove_automation_callback");
            //     return ESP_OK;
            // }

            // esp_err_t set_automations_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr)
            // {
            //     // /* Get ids */
            //     // uint16_t endpoint_id = command_path.mEndpointId;
            //     uint32_t cluster_id = command_path.mClusterId;
            //     uint32_t command_id = command_path.mCommandId;

            //     /* Return if this is not the set_automations command */
            //     if (cluster_id != chip::app::Clusters::DistributedDevice::Id || command_id != chip::app::Clusters::DistributedDevice::command::set_automations::Id)
            //     {
            //         ESP_LOGE(TAG_CUSTOM_CLUSTER, "Got DistributedDevice command callback for some other command. This should not happen.");
            //         return ESP_FAIL;
            //     }
            //     ESP_LOGI(TAG_CUSTOM_CLUSTER, "DistributedDevice set_automations command callback");

            //     // /* Parse the tlv data */
            //     chip::CharSpan config_value;
            //     chip::app::DataModel::Decode(tlv_data, config_value);
            //     if (!config_value.data() || config_value.size() <= 0)
            //     {
            //         ESP_LOGE(TAG_CUSTOM_CLUSTER, "Command data not found or was not decoded correctly. The expected data is a string or the"
            //                                      "format is \"<data>\"");
            //         return ESP_FAIL;
            //     }
            //     ESP_LOGI(TAG_CUSTOM_CLUSTER, "tlv data parse successfully: %.*s", config_value.size(), config_value.data());
            //     return ESP_OK;
            // }

            esp_err_t set_automations_callback(esp_matter_attr_val_t *data)
            {
                ESP_LOGI(TAG_CUSTOM_CLUSTER, "set_automations_callback");

                DistributedDevice::Instance().SetAutomationsFromJsonString(data->val);
                return ESP_OK;
            }

            cluster_t *create(endpoint_t *endpoint, config_t *config_, uint8_t flags)
            {
                cluster_t *cluster = cluster::create(endpoint, DistributedDevice::Id, flags);
                if (!cluster)
                {
                    ESP_LOGE(TAG_CUSTOM_CLUSTER, "Could not create cluster");
                    return NULL;
                }

                ESP_LOGI(TAG_CUSTOM_CLUSTER, "Custom distributed Cluster...");
                attribute::create(cluster, chip::app::Clusters::DistributedDevice::attribute::automations::Id, ATTRIBUTE_FLAG_WRITABLE, esp_matter_char_str(config_->automations, sizeof(config_->automations)));
                // command::create(cluster, chip::app::Clusters::DistributedDevice::command::add_automation::Id, COMMAND_FLAG_ACCEPTED | COMMAND_FLAG_CUSTOM, add_automation_callback);
                // command::create(cluster, chip::app::Clusters::DistributedDevice::command::remove_automation::Id, COMMAND_FLAG_ACCEPTED | COMMAND_FLAG_CUSTOM, remove_automation_callback);
                // command::create(cluster, chip::app::Clusters::DistributedDevice::command::set_automations::Id, COMMAND_FLAG_ACCEPTED | COMMAND_FLAG_CUSTOM, set_automations_callback);

                ESP_LOGI(TAG_CUSTOM_CLUSTER, "Custom distributed Cluster created");

                return cluster;
            }

        }
    }
}
