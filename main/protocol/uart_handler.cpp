#include <esp_log.h>

#include "uart_handler.hpp"
#include "uart_ctrl.hpp"

void uart_rx_handler::onSubscribe(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc, uint16_t subValue)
{
    NimBLECharacteristicCallbacks::onSubscribe(pCharacteristic, desc, subValue);
    auto &uart = uart_ctrl::instance();

    if (subValue == 0 && uart.inited()) {
        uart.deinit();
    } else {
        uart.init();
        characteristic = pCharacteristic;
        uart.set_incoming_data_cb([&](size_t len){
            on_uart_data_available(len);
        });
    }
}

void uart_rx_handler::onRead(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onRead(pCharacteristic, desc);

    uint8_t read_buf[512] = {};

    auto &uart = uart_ctrl::instance();
    if (uart.get_rx_buf_len() == 0) {
        ESP_LOGW(TAG, "Read requested but nothing to read");
        return;
    }

    size_t mtu = pCharacteristic->getService()->getServer()->getPeerMTU(desc->conn_handle) - 2;
    size_t read_len = std::min(std::min(sizeof(read_buf), mtu), uart.get_rx_buf_len());
    if (uart.uart_recv(read_buf, read_len) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read from UART, len %d", read_len);
        return;
    }

    ESP_LOGI(TAG, "Write value, len %d", read_len);
    ESP_LOG_BUFFER_HEX(TAG, read_buf, read_len);
    pCharacteristic->setValue(read_buf, read_len);
}

void uart_rx_handler::on_uart_data_available(size_t len)
{
    if (characteristic == nullptr) return;
    if (len < 1) return;

    // Usually for a phone the maximum BLE notification is around 20 bytes,
    // so if the incoming UART data is less than or equal to 18 bytes, send it through;
    // otherwise, we send the length and tell the phone to read it.
    auto &uart = uart_ctrl::instance();
    if (len <= 18) {
        uart_handler::data_pkt pkt = {};
        pkt.type = uart_handler::UART_PKT_DATA;
        pkt.len = len;
        uart.uart_recv(pkt.data, len);
        characteristic->notify((uint8_t *)&pkt, std::min((size_t)(len + 2), (size_t)20)); // 2 bytes of packet type + length
    } else {
        uart_handler::length_pkt pkt = {};
        pkt.type = uart_handler::UART_PKT_LENGTH;
        pkt.increment_len = len;
        pkt.total_len = uart.get_rx_buf_len();
        characteristic->notify((uint8_t *)&pkt, sizeof(uart_handler::length_pkt));
    }
}

void uart_tx_handler::onSubscribe(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc, uint16_t subValue)
{
    NimBLECharacteristicCallbacks::onSubscribe(pCharacteristic, desc, subValue);
}

void uart_tx_handler::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);
}
