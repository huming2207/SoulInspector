#include "uart_config_broker.hpp"

uart_config_broker::uart_config_broker(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud, uart_port_t _port)
{
    rx_pin = _rx;
    tx_pin = _tx;
    baud = _default_baud;
    port = _port;
}

void uart_config_broker::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{

}

void uart_config_broker::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);
}
