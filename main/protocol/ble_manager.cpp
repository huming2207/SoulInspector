#include <esp_mac.h>
#include "ble_manager.hpp"
#include "NimBLEDevice.h"
#include "config_handler.hpp"

esp_err_t ble_manager::init()
{
    uint8_t mac_addr[6] = {};
    esp_efuse_mac_get_default(mac_addr);

    char device_name[64] = {};
    snprintf(device_name, sizeof(device_name), "SoulInspector-%x%x%x", mac_addr[3], mac_addr[4], mac_addr[5]);

    NimBLEDevice::init("soul-inspector");
    ble_server = BLEDevice::createServer();
    if (ble_server == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    ble_service = ble_server->createService(SERVICE_UUID);
    if (ble_service == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    auto config_read_char = ble_service->createCharacteristic(CONFIG_READER_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    config_read_char->setCallbacks(new config_reader());

    auto config_write_char = ble_service->createCharacteristic(CONFIG_WRITER_CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);
    config_write_char->setCallbacks(new config_writer);

    if (!ble_service->start()) {
        ESP_LOGE(TAG, "Failed to start BLE GATT service");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "GATT service started");

    if (!ble_server->getAdvertising()->start()) {
        ESP_LOGE(TAG, "Failed to start advertising");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Advertising started");
    return ESP_OK;
}
