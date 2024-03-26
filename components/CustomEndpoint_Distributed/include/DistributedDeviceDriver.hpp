#ifndef DISTRIBUTED_DEVICE_DRIVER_HPP
#define DISTRIBUTED_DEVICE_DRIVER_HPP

#include <esp_matter.h>
#include "DistributedDevice.hpp"

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t light_endpoint_id;

/* Do any conversions/remapping for the actual value here */
distributed_device_driver_config_t distributed_device_get_config();

distributed_device_driver_handle_t distributed_device_init();

app_driver_handle_t app_driver_distributed_device_init();

#endif // DISTRIBUTED_DEVICE_DRIVER_HPP
