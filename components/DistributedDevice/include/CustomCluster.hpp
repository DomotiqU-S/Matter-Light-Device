#ifndef CUSTOM_CLUSTER_HPP
#define CUSTOM_CLUSTER_HPP
// See : https: // github.com/espressif/esp-matter/blob/d4cbddc4d385214ca520a3002372b6b948681ca0/components/esp_matter/esp_matter_cluster.h#L4

#define MAX_AUTOMATIONS_LEN 512

#include <esp_matter.h>
// using namespace esp_matter;
using namespace chip::app::Clusters;
// using namespace esp_matter::cluster;

#define TAG_CUSTOM_CLUSTER "CustomCluster"
namespace chip
{
    namespace app
    {
        namespace Clusters
        {
            namespace DistributedDevice
            {
                static constexpr ClusterId Id = 0x0000;

                namespace command
                {
                    namespace add_automation
                    {
                        static constexpr CommandId Id = 0x00;
                    }

                    namespace remove_automation
                    {
                        static constexpr CommandId Id = 0x01;
                    }

                    namespace set_automations
                    {
                        static constexpr CommandId Id = 0x02;
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
                uint16_t cluster_revision;
                char automations[MAX_AUTOMATIONS_LEN];
                config_t() : cluster_revision(1), automations{0} {}
            } config_t;

            cluster_t *create(endpoint_t *endpoint, config_t *config_, uint8_t flags);
            // static esp_err_t add_command_callback(const ConcreteCommandPath &command_path, TLVReader &tlv_data, void *opaque_ptr)

        }
    }
}

#endif // CUSTOM_CLUSTER_HPP
