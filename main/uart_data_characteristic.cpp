#include "uart_data_characteristic.hpp"

esp_err_t uart_data_characteristic::init()
{
    rx_lock = xSemaphoreCreateMutex();
    return uart_data_broker::init();
}

void uart_data_characteristic::onRead(NimBLECharacteristic *pCharacteristic)
{
    static uint8_t rx_buf[SOUL_UART_RX_BUF_SIZE] = {};
    memset(rx_buf, 0, sizeof(rx_buf));



    pCharacteristic->setValue(rx_buf, )
}

void uart_data_characteristic::onWrite(NimBLECharacteristic *pCharacteristic)
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
