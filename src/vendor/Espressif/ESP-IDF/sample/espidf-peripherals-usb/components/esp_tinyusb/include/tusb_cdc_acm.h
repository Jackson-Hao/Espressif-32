/*
 * SPDX-FileCopyrightText: 2020-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sdkconfig.h"
#include "esp_err.h"

#include "tinyusb_types.h"
#include "class/cdc/cdc.h"

#if (CONFIG_TINYUSB_CDC_ENABLED != 1)
#error "TinyUSB CDC driver must be enabled in menuconfig"
#endif


/**
 * @brief CDC ports available to setup
 */
typedef enum {
    TINYUSB_CDC_ACM_0 = 0x0,
    TINYUSB_CDC_ACM_1,
    TINYUSB_CDC_ACM_MAX
} tinyusb_cdcacm_itf_t;

/* Callbacks and events
   ********************************************************************* */

/**
 * @brief Data provided to the input of the `callback_rx_wanted_char` callback
 */
typedef struct {
    char wanted_char; /*!< Wanted character */
} cdcacm_event_rx_wanted_char_data_t;

/**
 * @brief Data provided to the input of the `callback_line_state_changed` callback
 */
typedef struct {
    bool dtr; /*!< Data Terminal Ready (DTR) line state */
    bool rts; /*!< Request To Send (RTS) line state */
} cdcacm_event_line_state_changed_data_t;

/**
 * @brief Data provided to the input of the `line_coding_changed` callback
 */
typedef struct {
    cdc_line_coding_t const *p_line_coding; /*!< New line coding value */
} cdcacm_event_line_coding_changed_data_t;

/**
 * @brief Types of CDC ACM events
 */
typedef enum {
    CDC_EVENT_RX,
    CDC_EVENT_RX_WANTED_CHAR,
    CDC_EVENT_LINE_STATE_CHANGED,
    CDC_EVENT_LINE_CODING_CHANGED
} cdcacm_event_type_t;

/**
 * @brief Describes an event passing to the input of a callbacks
 */
typedef struct {
    cdcacm_event_type_t type; /*!< Event type */
    union {
        cdcacm_event_rx_wanted_char_data_t rx_wanted_char_data; /*!< Data input of the `callback_rx_wanted_char` callback */
        cdcacm_event_line_state_changed_data_t line_state_changed_data; /*!< Data input of the `callback_line_state_changed` callback */
        cdcacm_event_line_coding_changed_data_t line_coding_changed_data; /*!< Data input of the `line_coding_changed` callback */
    };
} cdcacm_event_t;

/**
 * @brief CDC-ACM callback type
 */
typedef void(*tusb_cdcacm_callback_t)(int itf, cdcacm_event_t *event);

/*********************************************************************** Callbacks and events*/
/* Other structs
   ********************************************************************* */

/**
 * @brief Configuration structure for CDC-ACM
 */
typedef struct {
    tinyusb_usbdev_t usb_dev; /*!< Usb device to set up */
    tinyusb_cdcacm_itf_t cdc_port;  /*!< CDC port */
    size_t rx_unread_buf_sz __attribute__((deprecated("This parameter is not used any more. Configure RX buffer in menuconfig.")));
    tusb_cdcacm_callback_t callback_rx;  /*!< Pointer to the function with the `tusb_cdcacm_callback_t` type that will be handled as a callback */
    tusb_cdcacm_callback_t callback_rx_wanted_char; /*!< Pointer to the function with the `tusb_cdcacm_callback_t` type that will be handled as a callback */
    tusb_cdcacm_callback_t callback_line_state_changed; /*!< Pointer to the function with the `tusb_cdcacm_callback_t` type that will be handled as a callback */
    tusb_cdcacm_callback_t callback_line_coding_changed; /*!< Pointer to the function with the `tusb_cdcacm_callback_t` type that will be handled as a callback */
} tinyusb_config_cdcacm_t;

/*********************************************************************** Other structs*/
/* Public functions
   ********************************************************************* */
/**
 * @brief Initialize CDC ACM. Initialization will be finished with
 *          the `tud_cdc_line_state_cb` callback
 *
 * @param[in] cfg Configuration structure
 * @return esp_err_t
 */
esp_err_t tusb_cdc_acm_init(const tinyusb_config_cdcacm_t *cfg);

/**
 * @brief De-initialize CDC ACM.
 *
 * @param[in] itf Index of CDC interface
 * @return esp_err_t
 */
esp_err_t tusb_cdc_acm_deinit(int itf);

/**
 * @brief Register a callback invoking on CDC event. If the callback had been
 *        already registered, it will be overwritten
 *
 * @param[in] itf        Index of CDC interface
 * @param[in] event_type Type of registered event for a callback
 * @param[in] callback   Callback function
 * @return esp_err_t - ESP_OK or ESP_ERR_INVALID_ARG
 */
esp_err_t tinyusb_cdcacm_register_callback(tinyusb_cdcacm_itf_t itf,
        cdcacm_event_type_t event_type,
        tusb_cdcacm_callback_t callback);

/**
 * @brief Unregister a callback invoking on CDC event
 *
 * @param[in] itf        Index of CDC interface
 * @param[in] event_type Type of registered event for a callback
 * @return esp_err_t - ESP_OK or ESP_ERR_INVALID_ARG
 */
esp_err_t tinyusb_cdcacm_unregister_callback(tinyusb_cdcacm_itf_t itf, cdcacm_event_type_t event_type);

/**
 * @brief Sent one character to a write buffer
 *
 * @param[in] itf Index of CDC interface
 * @param[in] ch  Character to send
 * @return size_t - amount of queued bytes
 */
size_t tinyusb_cdcacm_write_queue_char(tinyusb_cdcacm_itf_t itf, char ch);

/**
 * @brief Write data to write buffer
 *
 * @param[in] itf     Index of CDC interface
 * @param[in] in_buf  Data
 * @param[in] in_size Data size in bytes
 * @return size_t - amount of queued bytes
 */
size_t tinyusb_cdcacm_write_queue(tinyusb_cdcacm_itf_t itf, const uint8_t *in_buf, size_t in_size);

/**
 * @brief Flush data in write buffer of CDC interface
 *
 * Use `tinyusb_cdcacm_write_queue` to add data to the buffer
 *
 *        WARNING! TinyUSB can block output Endpoint for several RX callbacks, after will do additional flush
 *        after the each transfer. That can leads to the situation when you requested a flush, but it will fail until
 *        one of the next callbacks ends.
 *        SO USING OF THE FLUSH WITH TIMEOUTS IN CALLBACKS IS NOT RECOMMENDED - YOU CAN GET A LOCK FOR THE TIMEOUT
 *
 * @param[in] itf             Index of CDC interface
 * @param[in] timeout_ticks   Transfer timeout. Set to zero for non-blocking mode
 * @return - ESP_OK           All data flushed
 *         - ESP_ERR_TIMEOUT  Time out occurred in blocking mode
 *         - ESP_NOT_FINISHED The transfer is still in progress in non-blocking mode
 */
esp_err_t tinyusb_cdcacm_write_flush(tinyusb_cdcacm_itf_t itf, uint32_t timeout_ticks);

/**
 * @brief Receive data from CDC interface
 *
 * @param[in] itf           Index of CDC interface
 * @param[out] out_buf      Data buffer
 * @param[in] out_buf_sz    Data buffer size in bytes
 * @param[out] rx_data_size Number of bytes written to out_buf
 * @return esp_err_t ESP_OK, ESP_FAIL or ESP_ERR_INVALID_STATE
 */
esp_err_t tinyusb_cdcacm_read(tinyusb_cdcacm_itf_t itf, uint8_t *out_buf, size_t out_buf_sz, size_t *rx_data_size);

/**
 * @brief Check if the CDC interface is initialized
 *
 * @param[in] itf  Index of CDC interface
 * @return - true  Initialized
 *         - false Not Initialized
 */
bool tusb_cdc_acm_initialized(tinyusb_cdcacm_itf_t itf);

/*********************************************************************** Public functions*/

#ifdef __cplusplus
}
#endif
