#include <esp_log.h>
#include <cstring>
#include <sys/param.h>

#include "uart_ctrl.hpp"
#include "config_manager.hpp"

esp_err_t uart_ctrl::init(gpio_num_t _rx, gpio_num_t _tx, gpio_num_t _rts, gpio_num_t _cts, uint32_t _default_baud, uart_port_t _port)
{
    rx_pin = _rx;
    tx_pin = _tx;
    rts_pin = _rts;
    cts_pin = _cts;
    default_baud = _default_baud;
    port = _port;

    uart_config_t config = {
            .baud_rate = static_cast<int>(default_baud),
            .data_bits = UART_DATA_8_BITS,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_RTC,
    };

    auto ret = load_uart_config(&config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to load config: 0x%x", ret);
        return ret;
    }

    ret = uart_driver_install(port, SOUL_UART_RX_BUF_SIZE, SOUL_UART_TX_BUF_SIZE, 20, &evt_queue, 0);
    ret = ret ?: uart_param_config(port, &config);
    ret = ret ?: uart_set_pin(port, tx_pin, rx_pin, rts_pin, cts_pin);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "UART init failed: 0x%x", ret);
        return ret;
    }

    auto task_ret = xTaskCreate(uart_event_handler, "si_uart_evt", 8192, this, tskIDLE_PRIORITY + 5, &evt_task_handle);
    if (task_ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create UART event handler task");
        return ESP_ERR_NO_MEM;
    }

    has_inited = true;
    return ret;
}

void uart_ctrl::uart_event_handler(void *_ctx)
{
    auto *ctx = static_cast<uart_ctrl *>(_ctx);
    if (ctx == nullptr) {
        ESP_LOGE(TAG, "UART broker context pointer is null!");
        return;
    }

    while (true) {
        uart_event_t event = {};
        if (xQueueReceive(ctx->evt_queue, (void *)&event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_DATA: {
                    if (ctx->on_incoming_data) ctx->on_incoming_data(event.size);
                    break;
                }

                case UART_BUFFER_FULL: {
                    ESP_LOGW(TAG, "Rx buffer full!!!");
                    uart_flush_input(ctx->port);
                    break;
                }

                case UART_FIFO_OVF: {
                    ESP_LOGW(TAG, "Rx FIFO overflow!!!");
                    uart_flush_input(ctx->port);
                    break;
                }

                case UART_FRAME_ERR: {
                    ESP_LOGE(TAG, "UART frame error detected!");
                    break;
                }
                case UART_PARITY_ERR: {
                    ESP_LOGE(TAG, "UART parity error detected!");
                    break;
                }

                default: break;
            }
        }
    }
}

esp_err_t uart_ctrl::uart_send(uint8_t *buf, size_t len) const
{
    return uart_write_bytes(port, (void *)buf, len);
}

esp_err_t uart_ctrl::uart_recv(uint8_t *buf, size_t len) const
{
    size_t buffed_len = 0;
    uart_get_buffered_data_len(port, &buffed_len);

    size_t read_len = MIN(buffed_len, len);
    if (read_len < 1) {
        ESP_LOGW(TAG, "Read length too short: %u %u %u", read_len, buffed_len, len);
        return ESP_ERR_INVALID_SIZE;
    }

    uart_read_bytes(port, buf, read_len, portMAX_DELAY);
    return ESP_OK;
}

void uart_ctrl::set_incoming_data_cb(const std::function<void(size_t)> &cb)
{
    on_incoming_data = cb;
}

esp_err_t uart_ctrl::load_uart_config(uart_config_t *cfg)
{
    if (cfg == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    auto &cfg_mgr = config_manager::instance();
    uint32_t baud = 0, stop_bit = 0, data_bit = 0, parity = 0;
    auto ret = cfg_mgr.get_u32(config::UART_BAUD, baud);
    ret = ret ?: cfg_mgr.get_u32(config::UART_STOP, stop_bit);
    ret = ret ?: cfg_mgr.get_u32(config::UART_DATA, data_bit);
    ret = ret ?: cfg_mgr.get_u32(config::UART_PARITY, parity);

    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Failed to load config, writing default config...");
        ret = cfg_mgr.set_u32(config::UART_BAUD, cfg->baud_rate);
        ret = ret ?: cfg_mgr.set_u32(config::UART_STOP, cfg->stop_bits);
        ret = ret ?: cfg_mgr.set_u32(config::UART_DATA, cfg->data_bits);
        ret = ret ?: cfg_mgr.set_u32(config::UART_PARITY, cfg->parity);

        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Init default setting failed");
            return ESP_ERR_INVALID_STATE;
        } else {
            ret = cfg_mgr.get_u32(config::UART_BAUD, baud);
            ret = ret ?: cfg_mgr.get_u32(config::UART_STOP, stop_bit);
            ret = ret ?: cfg_mgr.get_u32(config::UART_DATA, data_bit);
            ret = ret ?: cfg_mgr.get_u32(config::UART_PARITY, parity);
        }
    }

    return ret;
}

esp_err_t uart_ctrl::deinit()
{
    has_inited = false;
    return uart_driver_delete(port);
}

bool uart_ctrl::inited() const
{
    return has_inited;
}

size_t uart_ctrl::get_rx_buf_len() const
{
    size_t len = 0;
    if (uart_get_buffered_data_len(port, &len) != ESP_OK) {
        return 0;
    }

    return len;
}
