#ifndef CUSTOM_CLUSTER_HPP
#define CUSTOM_CLUSTER_HPP
// See : https: // github.com/espressif/esp-matter/blob/d4cbddc4d385214ca520a3002372b6b948681ca0/components/esp_matter/esp_matter_cluster.h#L4

#define MAX_AUTOMATIONS_LEN 256

#include <esp_matter.h>
// using namespace esp_matter;
using namespace chip::app::Clusters;
// using namespace esp_matter::cluster;

#include "DistributedDevice.hpp"

#define TAG_CUSTOM_CLUSTER "CustomCluster"
namespace chip
{
    namespace app
    {
        namespace Clusters
        {
            namespace DistributedDevice
            {
                static constexpr ClusterId Id = 0x02;
                namespace attribute
                {
                    namespace automations
                    {
                        static constexpr AttributeId Id = 0x01;
                    }
                }
                namespace command
                {
                    namespace add_automation
                    {
                        static constexpr CommandId Id = 0x02;
                    }

                    namespace remove_automation
                    {
                        static constexpr CommandId Id = 0x03;
                    }
                    namespace set_automations
                    {
                        static constexpr CommandId Id = 0x04;
                    }
                }

            }
        }
    }
}

namespace esp_matter
{
    namespace cluster
    {
        namespace distributed_device
        {

            typedef struct config_t
            {
                char automations[MAX_AUTOMATIONS_LEN];
                config_t() : automations{0} {}
            } config_t;

            cluster_t *create(endpoint_t *endpoint, config_t *config_, uint8_t flags);
            esp_err_t add_automation_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr);
            esp_err_t remove_automation_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr);
            // esp_err_t set_automations_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr);
            esp_err_t set_automations_callback(esp_matter_attr_val_t *data);
            // esp_err_t parse_tlv(TLVReader &tlv_data, chip::CharSpan config_value);
        }
    }
}

#endif // CUSTOM_CLUSTER_HPP
