#pragma once

#include "uart_data_broker.hpp"

class uart_config_broker
{
public:
    uart_config_broker(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud = 115200, uart_port_t _port = UART_NUM_1);

protected:
    esp_err_t uart_set_baud(uint32_t baud_rate);

private:
    gpio_num_t rx_pin = GPIO_NUM_MAX;
    gpio_num_t tx_pin = GPIO_NUM_MAX;
    uint32_t baud = 115200;
    uart_port_t port = 1;
};
