#include "uart_data_characteristic.hpp"

esp_err_t uart_data_characteristic::init()
{
    rx_lock = xSemaphoreCreateMutex();
    return uart_data_broker::init();
}

void uart_data_characteristic::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc* desc)
{
    uint16_t mtu = pCharacteristic->getService()->getServer()->getPeerMTU(desc->conn_handle);
    if (mtu < BLE_ATT_MTU_DFLT) {
        ESP_LOGW(TAG, "MTU too small: %u", mtu);
        return;
    }

    size_t rx_len = mtu - 3;
    auto *rx_buf = (uint8_t *)calloc(1, rx_len);
    if (uart_recv(rx_buf, rx_len) == ESP_OK) {
        pCharacteristic->setValue(rx_buf, rx_len);
    }
}

void uart_data_characteristic::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc* desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic);
}

esp_err_t uart_data_characteristic::on_uart_incoming_data(size_t len)
{
    if (characteristic == nullptr) {
        ESP_LOGE(TAG, "NimBLE characteristic pointer is null!");
        return ESP_ERR_INVALID_STATE;
    }

    characteristic->notify(false); // Indication only
    return ESP_OK;
}

uart_data_characteristic::uart_data_characteristic(gpio_num_t rx, gpio_num_t tx, uint32_t default_baud,
                                                   uart_port_t port, NimBLECharacteristic *_characteristic)
        : uart_data_broker(rx, tx, default_baud, port)
{
    characteristic = _characteristic;
}
