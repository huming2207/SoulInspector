#include "dev_info_handler.hpp"

void dev_info_handler::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onRead(pCharacteristic, desc);


}
