#include <esp_log.h>
#include <cstring>

#include "uart_broker.hpp"

uart_broker::uart_broker(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud, uart_port_t _port)
{
    rx_pin = _rx;
    tx_pin = _tx;
    port = _port;
    default_baud = _default_baud;
}

esp_err_t uart_broker::init()
{
    uart_config_t config = {
            .baud_rate = static_cast<int>(default_baud),
            .data_bits = UART_DATA_8_BITS,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
    };

    auto ret = uart_driver_install(port, SOUL_UART_RX_BUF_SIZE, SOUL_UART_TX_BUF_SIZE, 20, &evt_queue, 0);
    ret = ret ?: uart_param_config(port, &config);
    ret = ret ?: uart_set_pin(port, tx_pin, rx_pin, -1, -1);

    xTaskCreate(uart_event_handler, "si_uart_evt", 8192, nullptr, tskIDLE_PRIORITY + 5, nullptr);

    return ret;
}

void uart_broker::uart_event_handler(void *_ctx)
{
    auto *ctx = static_cast<uart_broker *>(_ctx);
    if (ctx == nullptr) {
        ESP_LOGE(TAG, "UART broker context pointer is null!");

        return;
    }

    while (true) {
        uint8_t rx_buf[SOUL_UART_RX_BUF_SIZE] = {};
        uart_event_t event = {};
        if (xQueueReceive(ctx->evt_queue, (void *)&event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_DATA: {
                    memset(rx_buf, 0, sizeof(rx_buf));
                    uart_read_bytes(ctx->port, rx_buf, event.size, portMAX_DELAY);
                    ctx->on_uart_receive(rx_buf, event.size);
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

esp_err_t uart_broker::uart_send(uint8_t *buf, size_t len) const
{
    return uart_write_bytes(port, (void *)buf, len);
}

esp_err_t uart_broker::uart_set_baud(uint32_t baud_rate) const
{
    return uart_set_baudrate(port, baud_rate);
}
