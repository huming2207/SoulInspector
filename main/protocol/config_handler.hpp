#pragma once

#include <cstdint>
#include <NimBLECharacteristic.h>

namespace pkt
{
    enum config_ops : uint8_t {
        CONFIG_U32 = 0,
        CONFIG_POS_I32 = 1,
        CONFIG_NEG_I32 = 2,
        CONFIG_STR = 3,
        CONFIG_BLOB = 4,
        CONFIG_DELETE = 0xa0,
        CONFIG_NUKE = 0xa1,
        CONFIG_OK = 0xf0,
        CONFIG_ERROR_NOT_FOUND = 0xf1,
        CONFIG_ERROR_INVALID_SIZE = 0xf2,
        CONFIG_ERROR_UNKNOWN_TYPE = 0xf3,
        CONFIG_ERROR_GENERIC = 0xff,
    };

    struct __attribute__((packed)) num_cfg_pkt {
        config_ops type;
        char key[16];
        uint32_t data;
    };

    struct __attribute__((packed)) blob_str_cfg_pkt {
        config_ops type;
        uint8_t len;
        char key[16];
        uint8_t data[128];
    };

    struct __attribute__((packed)) cfg_key_pkt {
        config_ops type;
        char key[16];
    };
}

class config_writer : public NimBLECharacteristicCallbacks
{
private:
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;

private:
    static const constexpr char SIMPLE_ACK_MAGIC[] = "JingHouJiaYin";
    static const constexpr char *TAG = "config_writer";
};

class config_reader : public NimBLECharacteristicCallbacks
{
private:
    void onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) override;

private:
    static const constexpr char SIMPLE_ACK_MAGIC[] = "JingHouJiaYin";
    static const constexpr char *TAG = "config_writer";
};

