#include <nvs_flash.h>
#include <esp_log.h>
#include "config_manager.hpp"

esp_err_t config_manager::init()
{
    static_assert(key_attr.unique()); // Enforce unique key for the key attribute map
    auto ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ret = nvs_flash_erase();
        ret = ret ?: nvs_flash_init();
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "NVS init failed");
            return ret;
        }
    }

    nvs_handle = nvs::open_nvs_handle(CFG_MGR_NVS_NAMESPACE, NVS_READWRITE, &ret);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "NVS open failed");
        return ret;
    }

    return ESP_OK;
}

esp_err_t config_manager::get_u32(const char *key, uint32_t &out)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->get_item(key, out);
}

esp_err_t config_manager::get_i32(const char *key, int32_t &out)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->get_item(key, out);
}

esp_err_t config_manager::set_u32(const char *key, uint32_t in)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->set_item(key, in);
}

esp_err_t config_manager::set_i32(const char *key, int32_t in)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->set_item(key, in);
}

esp_err_t config_manager::get_str(const char *key, char *out, size_t len)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->get_string(key, out, len);
}

esp_err_t config_manager::get_blob(const char *key, uint8_t *out, size_t len)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->get_blob(key, out, len);
}

esp_err_t config_manager::set_str(const char *key, const char *in)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->set_string(key, in);
}

esp_err_t config_manager::set_blob(const char *key, const uint8_t *in, size_t len)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->set_blob(key, in, len);
}

esp_err_t config_manager::erase(const char *key)
{
    if (!is_key_valid(key)) return ESP_ERR_INVALID_ARG;
    return nvs_handle->erase_item(key);
}

esp_err_t config_manager::nuke()
{
    return nvs_handle->erase_all();
}

esp_err_t config_manager::flush()
{
    return nvs_handle->commit();
}

size_t config_manager::used_count()
{
    size_t cnt = 0;
    if (nvs_handle->get_used_entry_count(cnt) != ESP_OK) return -1;
    return cnt;
}

uint8_t config_manager::get_key_attr(const char *key)
{
    return key_attr.at(key);
}

bool config_manager::is_key_valid(const char *key)
{
    return key != nullptr && key_attr.contains(key);
}
