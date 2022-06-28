#include "time_sync_handler.hpp"

void time_sync_handler::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);

    auto data = pCharacteristic->getValue().data();
    auto len = pCharacteristic->getDataLength();

    if (len != 8 || data == nullptr) return;

    uint32_t ts_sec = ((data[0]) | (data[1] << 8u) | (data[2] << 16u) | (data[3] << 24u));

    struct timeval tv = {};
    struct timezone tz = {};
    tv.tv_sec = ts_sec;
    tv.tv_usec = 0; // This is BLE based, not gonna to be that accurate anyway...

    settimeofday(&tv, &tz);

    // Read it back...
    gettimeofday(&tv, &tz);

    pCharacteristic->notify((uint8_t *)&tv.tv_sec, sizeof(tv.tv_sec));
}
