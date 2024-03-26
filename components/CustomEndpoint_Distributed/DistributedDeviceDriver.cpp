
#include "DistributedDeviceDriver.hpp"

distributed_device_driver_config_t distributed_device_get_config() {
    distributed_device_driver_config_t config;
    config.device_type_id = DISTRIBUTED_DEVICE_TYPE_ID;
    config.device_type_version = DISTRIBUTED_DEVICE_TYPE_VERSION;
    return config;
}

distributed_device_driver_handle_t distributed_device_init(distributed_device_driver_config_t &config) {

}

app_driver_handle_t app_driver_distributed_device_init()
{
    /* Initialize distributed device */
    distributed_device_driver_config_t config = distributed_device_get_config();
    distributed_device_driver_handle_t handle = distributed_device_init(&config);
    return (app_driver_handle_t)handle;
}