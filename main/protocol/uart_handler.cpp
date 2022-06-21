#include "uart_handler.hpp"

void uart_rx_reader::onSubscribe(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc, uint16_t subValue)
{
    NimBLECharacteristicCallbacks::onSubscribe(pCharacteristic, desc, subValue);

    if (subValue == 0) {

    }
}

void uart_rx_reader::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onRead(pCharacteristic, desc);
}
