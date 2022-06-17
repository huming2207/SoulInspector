#pragma once

#include <esp_err.h>
#include <memory>
#include <nvs_handle.hpp>

#include "eternal.hpp"

#define CFG_MGR_NVS_NAMESPACE "soulinspector"

namespace config
{
    enum entry_type : uint8_t
    {
        ENTRY_INVALID = 0,
        ENTRY_U32 = 0x01,
        ENTRY_I32 = 0x02,
        ENTRY_STR = 0x03,
        ENTRY_BLOB = 0x04,
    };

    enum entry_policy : uint8_t
    {
        POLICY_USB_UNREADABLE = BIT0,
        POLICY_BLE_UNREADABLE = BIT1,
        POLICY_CLOUD_UNREADABLE = BIT2, // Reserved for Guanlan Cloud platform?
        POLICY_EXT_UNREADABLE = (BIT0 | BIT1 | BIT2),
    };
}

class config_manager
{
public:
    static config_manager& instance()
    {
        static config_manager instance;
        return instance;
    }

    config_manager(config_manager const &) = delete;
    void operator=(config_manager const &) = delete;

private:
    config_manager() = default;
    static const constexpr char *TAG = "cfg_mgr";
    std::shared_ptr<nvs::NVSHandle> nvs_handle = nullptr;
    static constexpr const auto key_attr = mapbox::eternal::map<mapbox::eternal::string, uint8_t>(
        {
                {"NET_WIFI_SSID", ((config::ENTRY_STR & 0xf) | (config::POLICY_EXT_UNREADABLE << 4))},
                {"NET_WIFI_PASS", ((config::ENTRY_STR & 0xf) | (config::POLICY_EXT_UNREADABLE << 4))},
                {"NET_API_ENDPT", (config::ENTRY_STR & 0xf)},
                {"CDC_USB_TMP_OFF", (config::ENTRY_U32 & 0xf)},
                {"UART_1_BAUD", (config::ENTRY_U32 & 0xf)},
                {"UART_1_STOP", (config::ENTRY_U32 & 0xf)},
                {"UART_1_DATA", (config::ENTRY_U32 & 0xf)},
                {"UART_1_PARITY", (config::ENTRY_U32 & 0xf)},
                {"UART_2_BAUD", (config::ENTRY_U32 & 0xf)},
                {"UART_2_STOP", (config::ENTRY_U32 & 0xf)},
                {"UART_2_DATA", (config::ENTRY_U32 & 0xf)},
                {"UART_2_PARITY", (config::ENTRY_U32 & 0xf)},
        }
    );

public:
    esp_err_t init();
    esp_err_t get_u32(const char *key, uint32_t &out);
    esp_err_t get_i32(const char *key, int32_t &out);
    esp_err_t set_u32(const char *key, uint32_t in);
    esp_err_t set_i32(const char *key, int32_t in);
    esp_err_t get_str(const char *key, char *out, size_t len);
    esp_err_t get_blob(const char *key, uint8_t *out, size_t len);
    esp_err_t set_str(const char *key, const char *in);
    esp_err_t set_blob(const char *key, const uint8_t *in, size_t len);
    esp_err_t erase(const char *key);
    esp_err_t nuke();
    esp_err_t flush();
    size_t used_count();
    static uint8_t get_key_attr(const char *key);
    static bool is_key_valid(const char *key);
};

