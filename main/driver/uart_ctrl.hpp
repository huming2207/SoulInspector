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
    esp_err_t init(gpio_num_t _rx = GPIO_NUM_1, gpio_num_t _tx = GPIO_NUM_2, gpio_num_t _rts = GPIO_NUM_NC,
                   gpio_num_t _cts = GPIO_NUM_NC, uint32_t _default_baud = 115200, uart_port_t _port = UART_NUM_1);
    esp_err_t deinit();
    [[nodiscard]] bool inited() const;
    static esp_err_t load_uart_config(uart_config_t *cfg);
    void set_incoming_data_cb(const std::function<void(size_t)> &cb);
    size_t send(uint8_t *buf, size_t len) const;
    esp_err_t receive(uint8_t *buf, size_t len) const;
    [[nodiscard]] size_t get_rx_buf_len() const;

private:
    uart_ctrl() = default;
    static void uart_event_handler(void *_ctx);

private:
    bool has_inited = false;
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
