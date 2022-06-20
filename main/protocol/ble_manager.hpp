#pragma once

#include <esp_err.h>
#include "NimBLEDevice.h"

class ble_manager
{
public:
    static ble_manager &instance()
    {
        static ble_manager _instance;
        return _instance;
    }

    ble_manager(ble_manager const &) = delete;
    void operator=(ble_manager const &) = delete;

public:
    static const constexpr char *SERVICE_UUID = "6ae00001-efc9-11ec-8ea0-0242ac120002";
    static const constexpr char *CONFIG_READER_CHARACTERISTIC_UUID = "6ae00002-efc9-11ec-8ea0-0242ac120002";
    static const constexpr char *CONFIG_WRITER_CHARACTERISTIC_UUID = "6ae00003-efc9-11ec-8ea0-0242ac120002";

public:
    esp_err_t init();

private:
    static const constexpr char *TAG = "ble_manager";
    ble_manager() = default;
    NimBLEServer *ble_server = nullptr;
    NimBLEService *ble_service = nullptr;
};

