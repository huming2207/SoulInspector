#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "uart_data_broker.hpp"
#include "NimBLECharacteristic.h"

class uart_data_characteristic : public uart_data_broker, NimBLECharacteristicCallbacks
{
public:
    uart_data_characteristic(gpio_num_t rx, gpio_num_t tx, uint32_t default_baud,
                             uart_port_t port, NimBLECharacteristic *characteristic);
    esp_err_t init() override;

protected:
    void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
    esp_err_t on_uart_incoming_data(size_t len) override;

private:
    static const constexpr char *TAG = "uart_data_char";
    NimBLECharacteristic *characteristic = nullptr;
    SemaphoreHandle_t rx_lock = nullptr;
};

