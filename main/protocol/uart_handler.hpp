#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "NimBLECharacteristic.h"

namespace uart_handler
{
    enum pkt_type : uint8_t
    {
        UART_PKT_LENGTH = 0,
        UART_PKT_DATA = 1,
    };

    struct __attribute__((packed)) data_pkt
    {
        pkt_type type;
        uint8_t len;
        uint8_t data[500];
    };

    struct __attribute__((packed)) length_pkt
    {
        pkt_type type;
        uint32_t increment_len;
        uint32_t total_len;
    };
}

class uart_rx_handler : public NimBLECharacteristicCallbacks
{
public:
    void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue) override;
    void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;

private:
    void on_uart_data_available(size_t len);

private:
    static const constexpr char *TAG = "uart_rx_handler";
    NimBLECharacteristic* characteristic = nullptr;
};

class uart_tx_handler : public NimBLECharacteristicCallbacks
{
public:
    void onSubscribe(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc, uint16_t subValue) override;
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;

private:
    static const constexpr char *TAG = "uart_tx_handler";
    NimBLECharacteristic* characteristic = nullptr;
};
