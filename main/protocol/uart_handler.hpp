#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "NimBLECharacteristic.h"

class uart_rx_reader : public NimBLECharacteristicCallbacks
{
public:
    void onSubscribe(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc, uint16_t subValue) override;
    void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
};

