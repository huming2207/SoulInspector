#pragma once

#include <driver/uart.h>
#include "uart_data_broker.hpp"
#include "NimBLECharacteristic.h"

// Config packet, 7 bytes (no need to worry about bitwise if we align them to 1 byte)
union __attribute__((packed)) uart_config_pkt {
    struct __attribute__((packed)) {
        uart_word_length_t word_length:8;
        uart_stop_bits_t stop_bits:8;
        uart_parity_t parity:8;
        uart_hw_flowcontrol_t hw_flowctrl:8;
        uint32_t baud_rate:24;
    };
    uint8_t data[7];
};

class uart_config_broker : public NimBLECharacteristicCallbacks
{
public:
    uart_config_broker(gpio_num_t _rx, gpio_num_t _tx, uint32_t _default_baud = 115200, uart_port_t _port = UART_NUM_1);

protected:
    void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;

private:
    gpio_num_t rx_pin = GPIO_NUM_MAX;
    gpio_num_t tx_pin = GPIO_NUM_MAX;
    uint32_t baud = 115200;
    uart_port_t port = 1;
};
