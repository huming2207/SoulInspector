#pragma once

#include <cstdint>
#include "NimBLECharacteristic.h"

#ifndef PROJECT_VER
#define PROJECT_VER "v0.0.0-unknown"
#endif

namespace dev_info
{
    struct __attribute__((packed)) device_info {
        uint8_t mac_addr[6];
        uint8_t flash_id[8];
        char esp_idf_ver[32];
        char dev_model[32];
        char dev_build[32];
    };
}

class dev_info_handler : public NimBLECharacteristicCallbacks
{
private:
    void onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
};
