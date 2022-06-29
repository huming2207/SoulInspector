#include <esp_log.h>
#include "time_sync_handler.hpp"

void time_sync_handler::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);

    auto ts_sec = pCharacteristic->getValue<uint32_t>();

    struct timeval tv = {};
    struct timezone tz = {};
    tv.tv_sec = ts_sec;
    tv.tv_usec = 0; // This is BLE based, not gonna to be that accurate anyway...

    settimeofday(&tv, &tz);

    time_t ts = time(nullptr);
    ESP_LOGI(TAG, "Got timestamp: %u %lld", ts_sec, ts);
    pCharacteristic->notify((uint8_t *)&ts, sizeof(ts));
}
