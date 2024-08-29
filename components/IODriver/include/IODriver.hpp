#ifndef IODRIVER_HPP
#define IODRIVER_HPP

#include <esp_err.h>
#include <esp_matter.h>

#define DEBUG_DRIVER 1

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include "esp_openthread_types.h"
#endif

/** Standard max values (used for remapping attributes) */
#define STANDARD_BRIGHTNESS 100
#define STANDARD_HUE 360
#define STANDARD_SATURATION 100
#define STANDARD_TEMPERATURE_FACTOR 1000000

/** Matter max values (used for remapping attributes) */
#define MATTER_BRIGHTNESS 254
#define MATTER_HUE 254
#define MATTER_SATURATION 254
#define MATTER_TEMPERATURE_FACTOR 1000000

/** Default attribute values used during initialization */
#define DEFAULT_POWER false
#define DEFAULT_BRIGHTNESS 60
#define DEFAULT_HUE 140
#define DEFAULT_SATURATION 216
#define DEFAULT_TEMPERATURE 4000

const float XSM[6] = {
    3.2406255, -1.537208,
    -0.9689307, 1.8757561,
    0.0557101, -0.2040211,
};

typedef void *app_driver_handle_t;

esp_err_t convert_xy_srgb(uint16_t x, uint16_t y);

esp_err_t set_color_from_attribute(uint32_t attribute_id, esp_matter_attr_val_t *val);

/** Initialize the light driver
 *
 * This initializes the light driver associated with the selected board.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
app_driver_handle_t app_driver_light_init();

/** Initialize the button driver
 *
 * This initializes the button driver associated with the selected board.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
app_driver_handle_t app_driver_button_init();

/** Driver Update
 *
 * This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 *
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val);

/** Set defaults for light driver
 *
 * Set the attribute drivers to their default values from the created data model.
 *
 * @param[in] endpoint_id Endpoint ID of the driver.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_light_set_defaults(uint16_t endpoint_id);

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#define ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG()                                           \
    {                                                                                   \
        .radio_mode = RADIO_MODE_NATIVE,                                                \
    }

#define ESP_OPENTHREAD_DEFAULT_HOST_CONFIG()                                            \
    {                                                                                   \
        .host_connection_mode = HOST_CONNECTION_MODE_NONE,                              \
    }

#define ESP_OPENTHREAD_DEFAULT_PORT_CONFIG()                                            \
    {                                                                                   \
        .storage_partition_name = "nvs", .netif_queue_size = 10, .task_queue_size = 10, \
    }
#endif

#endif // IODRIVER_HPP