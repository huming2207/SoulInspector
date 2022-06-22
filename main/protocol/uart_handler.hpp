#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "NimBLECharacteristic.h"

namespace uart_handler
{
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

