#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#define SOUL_UART_TX_BUF_SIZE 1024
#define SOUL_UART_RX_BUF_SIZE 8192

class uart_ctrl
{
public:
    uart_ctrl(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud = 115200, uart_port_t _port = UART_NUM_1);

    virtual esp_err_t init();

protected:
    virtual esp_err_t on_uart_incoming_data(size_t len) = 0;
    esp_err_t uart_send(uint8_t *buf, size_t len) const;
    esp_err_t uart_recv(uint8_t *buf, size_t len) const;

private:
    static void uart_event_handler(void *_ctx);

private:
    uart_port_t port = 0;
    uint32_t default_baud = 9600;
    gpio_num_t rx_pin = GPIO_NUM_MAX;
    gpio_num_t tx_pin = GPIO_NUM_MAX;
    QueueHandle_t evt_queue = nullptr;

    static const constexpr char *TAG = "uart_broker";
};
