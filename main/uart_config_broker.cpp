#include "uart_config_broker.hpp"

uart_config_broker::uart_config_broker(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud, uart_port_t _port)
{
    rx_pin = _rx;
    tx_pin = _tx;
    baud = _default_baud;
    port = _port;
}


esp_err_t uart_config_broker::uart_set_baud(uint32_t baud_rate)
{
    baud = baud_rate;
    return uart_set_baudrate(port, baud);
}