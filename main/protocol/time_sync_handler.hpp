#pragma once

#include "NimBLECharacteristic.h"

class time_sync_handler : public NimBLECharacteristicCallbacks
{
public:
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;
};
