#pragma once

#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#define SOUL_UART_TX_BUF_SIZE 1024
#define SOUL_UART_RX_BUF_SIZE 8192

class uart_ctrl
{
public:
    static uart_ctrl &instance()
    {
        static uart_ctrl _instance;
        return _instance;
    }

    uart_ctrl(uart_ctrl const &) = delete;
    void operator=(uart_ctrl const &) = delete;

public:
    esp_err_t init(gpio_num_t _rx, gpio_num_t _tx, gpio_num_t _rts = GPIO_NUM_NC,
                   gpio_num_t _cts = GPIO_NUM_NC, uint32_t _default_baud = 115200, uart_port_t _port = UART_NUM_1);
    [[nodiscard]] esp_err_t deinit() const;
    static esp_err_t load_uart_config(uart_config_t *cfg);

protected:
    esp_err_t uart_send(uint8_t *buf, size_t len) const;
    esp_err_t uart_recv(uint8_t *buf, size_t len) const;
    void set_incoming_data_cb(const std::function<void(size_t)> &cb);

private:
    uart_ctrl() = default;
    static void uart_event_handler(void *_ctx);

private:
    uart_port_t port = UART_NUM_1;
    uint32_t default_baud = 115200;
    gpio_num_t rx_pin = GPIO_NUM_NC;
    gpio_num_t tx_pin = GPIO_NUM_NC;
    gpio_num_t rts_pin = GPIO_NUM_NC;
    gpio_num_t cts_pin = GPIO_NUM_NC;
    QueueHandle_t evt_queue = nullptr;
    TaskHandle_t evt_task_handle = nullptr;
    std::function<void(size_t)> on_incoming_data{};

    static const constexpr char *TAG = "uart_broker";
};
