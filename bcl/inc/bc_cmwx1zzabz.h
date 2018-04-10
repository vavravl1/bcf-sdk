#ifndef _BC_CMWX1ZZABZ_H
#define _BC_CMWX1ZZABZ_H

#include <bc_scheduler.h>
#include <bc_gpio.h>
#include <bc_uart.h>

//! @addtogroup bc_cmwx1zzabz bc_cmwx1zzabz
//! @brief Driver for CMWX1ZZABZ muRata LoRa modem
//! @{

//! @cond

#define BC_CMWX1ZZABZ_TX_FIFO_BUFFER_SIZE 64
#define BC_CMWX1ZZABZ_RX_FIFO_BUFFER_SIZE 64

//! @endcond

//! @brief Callback events

typedef enum
{
    //! @brief Ready event
    BC_CMWX1ZZABZ_EVENT_READY = 0,

    //! @brief Error event
    BC_CMWX1ZZABZ_EVENT_ERROR = 1,

    //! @brief RF frame transmission started event
    BC_CMWX1ZZABZ_EVENT_SEND_RF_FRAME_START = 2,

    //! @brief RF frame transmission finished event
    BC_CMWX1ZZABZ_EVENT_SEND_RF_FRAME_DONE = 3,

    //! @brief Configuration save done
    BC_CMWX1ZZABZ_EVENT_CONFIG_SAVE_DONE = 4,

    //! @brief OTAA join success
    BC_CMWX1ZZABZ_EVENT_JOIN_SUCCESS = 5,

    //! @brief OTAA join error
    BC_CMWX1ZZABZ_EVENT_JOIN_ERROR = 6   

} bc_cmwx1zzabz_event_t;

//! @brief CMWX1ZZABZ instance

typedef struct bc_cmwx1zzabz_t bc_cmwx1zzabz_t;

//! @brief LoRa mode ABP/OTAA

typedef enum
{
    BC_CMWX1ZZABZ_CONFIG_MODE_ABP = 0,
    BC_CMWX1ZZABZ_CONFIG_MODE_OTAA = 1,

} bc_cmwx1zzabz_config_mode_t;

//! @brief Frequency modes and standards

typedef enum
{
    BC_CMWX1ZZABZ_CONFIG_BAND_AS923 = 0,
    BC_CMWX1ZZABZ_CONFIG_BAND_AU915 = 1,
    BC_CMWX1ZZABZ_CONFIG_BAND_EU868 = 5,
    BC_CMWX1ZZABZ_CONFIG_BAND_KR920 = 6,
    BC_CMWX1ZZABZ_CONFIG_BAND_IN865 = 7,
    BC_CMWX1ZZABZ_CONFIG_BAND_US915 = 8

} bc_cmwx1zzabz_config_band_t;

//! @cond

typedef enum
{
    BC_CMWX1ZZABZ_CONFIG_INDEX_DEVADDR = 0,
    BC_CMWX1ZZABZ_CONFIG_INDEX_DEVEUI = 1,
    BC_CMWX1ZZABZ_CONFIG_INDEX_APPEUI = 2,
    BC_CMWX1ZZABZ_CONFIG_INDEX_NWKSKEY = 3,
    BC_CMWX1ZZABZ_CONFIG_INDEX_APPSKEY = 4,
    BC_CMWX1ZZABZ_CONFIG_INDEX_APPKEY = 5,
    BC_CMWX1ZZABZ_CONFIG_INDEX_BAND = 6,
    BC_CMWX1ZZABZ_CONFIG_INDEX_MODE = 7,
    BC_CMWX1ZZABZ_CONFIG_INDEX_LAST_ITEM

} bc_cmwx1zzabz_config_index_t;

typedef enum
{
    BC_CMWX1ZZABZ_STATE_READY = 0,
    BC_CMWX1ZZABZ_STATE_ERROR = 1,
    BC_CMWX1ZZABZ_STATE_INITIALIZE = 2,
    BC_CMWX1ZZABZ_STATE_IDLE = 3,
    BC_CMWX1ZZABZ_STATE_INITIALIZE_COMMAND_SEND = 5,
    BC_CMWX1ZZABZ_STATE_INITIALIZE_COMMAND_RESPONSE = 6,

    BC_CMWX1ZZABZ_STATE_CONFIG_SAVE_SEND = 7,
    BC_CMWX1ZZABZ_STATE_CONFIG_SAVE_RESPONSE = 8,

    BC_CMWX1ZZABZ_STATE_SEND_MESSAGE_COMMAND = 9,
    BC_CMWX1ZZABZ_STATE_SEND_MESSAGE_RESPONSE = 10,

    BC_CMWX1ZZABZ_STATE_JOIN_SEND = 11,
    BC_CMWX1ZZABZ_STATE_JOIN_RESPONSE = 12

} bc_cmwx1zzabz_state_t;

typedef struct
{
    bc_cmwx1zzabz_config_band_t band;
    bc_cmwx1zzabz_config_mode_t mode;
    char devaddr[8+1];
    char deveui[16+1];
    char appeui[16+1];
    char nwkskey[32+1];
    char appskey[32+1];
    char appkey[32+1];
} bc_cmwx1zzabz_config;

struct bc_cmwx1zzabz_t
{
    bc_scheduler_task_id_t _task_id;
    bc_uart_channel_t _uart_channel;
    bc_cmwx1zzabz_state_t _state;
    bc_cmwx1zzabz_state_t _state_after_sleep;
    bc_fifo_t _tx_fifo;
    bc_fifo_t _rx_fifo;
    uint8_t _tx_fifo_buffer[BC_CMWX1ZZABZ_TX_FIFO_BUFFER_SIZE];
    uint8_t _rx_fifo_buffer[BC_CMWX1ZZABZ_RX_FIFO_BUFFER_SIZE];
    void (*_event_handler)(bc_cmwx1zzabz_t *, bc_cmwx1zzabz_event_t, void *);
    void *_event_param;
    char _command[BC_CMWX1ZZABZ_TX_FIFO_BUFFER_SIZE];
    char _response[BC_CMWX1ZZABZ_RX_FIFO_BUFFER_SIZE];
    uint8_t _message_buffer[12];
    size_t _message_length;
    uint8_t init_command_index;
    uint8_t _save_command_index;
    bool _save_flag;
    uint32_t _save_config_mask;
    bc_cmwx1zzabz_config _config;
    bool _join_command;
};

//! @endcond

//! @brief Initialize CMWX1ZZABZ
//! @param[in] self Instance
//! @param[in] uart_channel UART channel where TX and RX signals are connected

void bc_cmwx1zzabz_init(bc_cmwx1zzabz_t *self, bc_uart_channel_t uart_channel);

//! @brief Set callback function
//! @param[in] self Instance
//! @param[in] event_handler Function address
//! @param[in] event_param Optional event parameter (can be NULL)

void bc_cmwx1zzabz_set_event_handler(bc_cmwx1zzabz_t *self, void (*event_handler)(bc_cmwx1zzabz_t *, bc_cmwx1zzabz_event_t, void *), void *event_param);

//! @brief Check if modem is ready for commands
//! @param[in] self Instance
//! @return true If ready
//! @return false If not ready

bool bc_cmwx1zzabz_is_ready(bc_cmwx1zzabz_t *self);

//! @brief Send RF frame command
//! @param[in] self Instance
//! @param[in] buffer Pointer to data to be transmitted
//! @param[in] length Length of data to be transmitted in bytes (must be from 1 to 51 bytes)
//! @return true If command was accepted for processing
//! @return false If command was denied for processing

bool bc_cmwx1zzabz_send_message(bc_cmwx1zzabz_t *self, const void *buffer, size_t length);

//! @brief Set DEVADDR
//! @param[in] self Instance
//! @param[in] devaddr Pointer to 8 character string

void bc_cmwx1zzabz_set_devaddr(bc_cmwx1zzabz_t *self, char *devaddr);

//! @brief Get DEVADDR
//! @param[in] self Instance
//! @param[in] devaddr Pointer to at least 8+1 character string

void bc_cmwx1zzabz_get_devaddr(bc_cmwx1zzabz_t *self, char *devaddr);

//! @brief Set DEVEUI
//! @param[in] self Instance
//! @param[in] deveui Pointer to 16 character string. WARNING, this value should not be changed as it is unique number programmed in the factory!

void bc_cmwx1zzabz_set_deveui(bc_cmwx1zzabz_t *self, char *deveui);

//! @brief Get DEVEUI
//! @param[in] self Instance
//! @param[in] deveui Pointer to at least 16+1 character string.

void bc_cmwx1zzabz_get_deveui(bc_cmwx1zzabz_t *self, char *deveui);

//! @brief Set APPEUI
//! @param[in] self Instance
//! @param[in] appeui Pointer to 16 character string

void bc_cmwx1zzabz_set_appeui(bc_cmwx1zzabz_t *self, char *appeui);

//! @brief Get APPEUI
//! @param[in] self Instance
//! @param[in] appeui Pointer to at least 16+1 character string

void bc_cmwx1zzabz_get_appeui(bc_cmwx1zzabz_t *self, char *appeui);

//! @brief Set NWKSKEY
//! @param[in] self Instance
//! @param[in] nwkskey Pointer to 32 character string

void bc_cmwx1zzabz_set_nwkskey(bc_cmwx1zzabz_t *self, char *nwkskey);

//! @brief Set NWKSKEY
//! @param[in] self Instance
//! @param[in] nwkskey Pointer to at least 32+1 character string

void bc_cmwx1zzabz_get_nwkskey(bc_cmwx1zzabz_t *self, char *nwkskey);

//! @brief Set APPSKEY
//! @param[in] self Instance
//! @param[in] appskey Pointer to 32 character string

void bc_cmwx1zzabz_set_appskey(bc_cmwx1zzabz_t *self, char *appskey);

//! @brief Get APPSKEY
//! @param[in] self Instance
//! @param[in] appskey Pointer to at least 32+1 character string

void bc_cmwx1zzabz_get_appskey(bc_cmwx1zzabz_t *self, char *appskey);

//! @brief Set APPKEY
//! @param[in] self Instance
//! @param[in] appkey Pointer to 32 character string

void bc_cmwx1zzabz_set_appkey(bc_cmwx1zzabz_t *self, char *appkey);

//! @brief Get APPKEY
//! @param[in] self Instance
//! @param[in] appkey Pointer to at least 32+1 character string

void bc_cmwx1zzabz_get_appkey(bc_cmwx1zzabz_t *self, char *appkey);

//! @brief Set BAND
//! @param[in] self Instance
//! @param[in] band Set correct frequency and modulation for EU, US and other countries

void bc_cmwx1zzabz_set_band(bc_cmwx1zzabz_t *self, bc_cmwx1zzabz_config_band_t band);

//! @brief Get BAND
//! @param[in] self Instance
//! @return Band value

bc_cmwx1zzabz_config_band_t bc_cmwx1zzabz_get_band(bc_cmwx1zzabz_t *self);

//! @brief Set ABP/OTAA mode
//! @param[in] self Instance
//! @param[in] mode ABP or OTAA mode

void bc_cmwx1zzabz_set_mode(bc_cmwx1zzabz_t *self, bc_cmwx1zzabz_config_mode_t mode);

//! @brief Get ABP/OTAA mode
//! @param[in] self Instance
//! @return ABP or OTAA mode

bc_cmwx1zzabz_config_mode_t bc_cmwx1zzabz_get_mode(bc_cmwx1zzabz_t *self);

//! @brief Start LoRa OTAA join procedure
//! @param[in] self Instance
//! @note The output of the join is handled by callback events
//! @see bc_cmwx1zzabz_event_t

void bc_cmwx1zzabz_join(bc_cmwx1zzabz_t *self);

#endif // _BC_CMWX1ZZABZ_H
