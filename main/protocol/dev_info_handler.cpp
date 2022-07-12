#include <esp_flash.h>
#include <esp_mac.h>
#include "dev_info_handler.hpp"

#ifndef SI_DEV_MODEL
#define SI_DEV_MODEL "Soul Inspector DEV"
#endif

void dev_info_handler::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onRead(pCharacteristic, desc);

    dev_info::device_info pkt = {};
    auto ret = esp_efuse_mac_get_default(pkt.mac_addr);
    ret = ret ?: esp_flash_read_unique_chip_id(esp_flash_default_chip, (uint64_t *)pkt.flash_id);
    if (ret != ESP_OK) return;

    const char *idf_ver = IDF_VER;
    const char *project_ver = PROJECT_VER;
    const char *dev_model = SI_DEV_MODEL;

    strncpy(pkt.esp_idf_ver, idf_ver, sizeof(IDF_VER));
    strncpy(pkt.dev_build, project_ver, sizeof(PROJECT_VER));
    strncpy(pkt.dev_model, dev_model, sizeof(SI_DEV_MODEL));

    pCharacteristic->setValue((uint8_t *)&pkt, sizeof(pkt));
}
