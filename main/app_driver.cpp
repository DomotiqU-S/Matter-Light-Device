#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <esp_matter.h>
#include "DistributedDevice.hpp"
#include <app_priv.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t light_endpoint_id;

/* Do any conversions/remapping for the actual value here */
// app_driver_handle_t app_driver_distributed_device_init()
// {
//     /* Initialize distributed device */
//     led_driver_config_t config = distributed_device_get_config();
//     led_driver_handle_t handle = distributed_device_init(&config);
//     return (app_driver_handle_t)handle;
// }
