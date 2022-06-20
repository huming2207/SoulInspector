#include "config_handler.hpp"
#include "config_manager.hpp"

void config_writer::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);

    auto &cfg_mgr = config_manager::instance();
    auto value = pCharacteristic->getValue(nullptr);
    auto *data = value.data();
    if (value.length() < 1) {
        ESP_LOGE(TAG, "Invalid size");
        pkt::cfg_key_pkt pkt = {};
        pkt.type = pkt::CONFIG_ERROR_INVALID_SIZE;
        pCharacteristic->notify((uint8_t *)&pkt, sizeof(pkt), true);
        return;
    }

    auto op = (pkt::config_ops)data[0];
    switch (op) {
        case pkt::CONFIG_U32: {
            if (value.length() < sizeof(pkt::num_cfg_pkt)) {
                pkt::cfg_key_pkt ack = {};
                ack.type = pkt::CONFIG_ERROR_INVALID_SIZE;
                pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
                return;
            }

            auto *pkt = (pkt::num_cfg_pkt *)data;
            auto ret = cfg_mgr.set_u32(pkt->key, pkt->data);

            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_POS_I32: {
            if (value.length() < sizeof(pkt::num_cfg_pkt)) {
                pkt::cfg_key_pkt ack = {};
                ack.type = pkt::CONFIG_ERROR_INVALID_SIZE;
                pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
                return;
            }

            auto *pkt = (pkt::num_cfg_pkt *)data;
            auto ret = cfg_mgr.set_i32(pkt->key, (int32_t)pkt->data);

            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_NEG_I32: {
            if (value.length() < sizeof(pkt::num_cfg_pkt)) {
                pkt::cfg_key_pkt ack = {};
                ack.type = pkt::CONFIG_ERROR_INVALID_SIZE;
                pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
                return;
            }

            auto *pkt = (pkt::num_cfg_pkt *)data;
            auto ret = cfg_mgr.set_i32(pkt->key, ((int32_t)pkt->data) * -1);

            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_BLOB: {
            if (value.length() < sizeof(pkt::blob_str_cfg_pkt)) {
                pkt::cfg_key_pkt ack = {};
                ack.type = pkt::CONFIG_ERROR_INVALID_SIZE;
                pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
                return;
            }

            auto *pkt = (pkt::blob_str_cfg_pkt *)data;
            auto ret = cfg_mgr.set_blob(pkt->key, pkt->data, pkt->len);

            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_STR: {
            if (value.length() < sizeof(pkt::blob_str_cfg_pkt)) {
                pkt::cfg_key_pkt ack = {};
                ack.type = pkt::CONFIG_ERROR_INVALID_SIZE;
                pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
                return;
            }

            auto *pkt = (pkt::blob_str_cfg_pkt *)data;
            auto ret = cfg_mgr.set_str(pkt->key, (const char *)pkt->data);

            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_OK: {
            pkt::cfg_key_pkt ack = {};
            ack.type = pkt::CONFIG_OK;
            memcpy(ack.key, SIMPLE_ACK_MAGIC, sizeof(SIMPLE_ACK_MAGIC));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_DELETE: {
            auto *pkt = (pkt::cfg_key_pkt *)data;
            auto ret = cfg_mgr.erase(pkt->key);
            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            memcpy(ack.key, pkt->key, sizeof(ack.key));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        case pkt::CONFIG_NUKE: {
            auto ret = cfg_mgr.nuke();
            pkt::cfg_key_pkt ack = {};
            ack.type = ret == ESP_OK ? pkt::CONFIG_OK : pkt::CONFIG_ERROR_GENERIC;
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
        default: {
            pkt::cfg_key_pkt ack = {};
            ack.type = pkt::CONFIG_ERROR_UNKNOWN_TYPE;
            memcpy(ack.key, SIMPLE_ACK_MAGIC, sizeof(SIMPLE_ACK_MAGIC));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
    }
}

void config_reader::onWrite(NimBLECharacteristic *pCharacteristic, ble_gap_conn_desc *desc)
{
    NimBLECharacteristicCallbacks::onWrite(pCharacteristic, desc);

    auto &cfg_mgr = config_manager::instance();
    auto value = pCharacteristic->getValue(nullptr);
    if (value.length() < sizeof(pkt::cfg_key_pkt)) {
        ESP_LOGE(TAG, "Invalid size");
        pkt::cfg_key_pkt pkt = {};
        pkt.type = pkt::CONFIG_ERROR_INVALID_SIZE;
        pCharacteristic->notify((uint8_t *)&pkt, sizeof(pkt), true);
        return;
    }

    auto *req = (pkt::cfg_key_pkt *)value.data();
    switch (req->type) {
        case pkt::CONFIG_U32: {
            uint32_t out = 0;
            auto ret = cfg_mgr.get_u32(req->key, out);

            pkt::num_cfg_pkt resp = {};
            if (ret != ESP_OK) {
                resp.type = pkt::CONFIG_ERROR_GENERIC;
            } else {
                resp.type = pkt::CONFIG_U32;
                memcpy(resp.key, req->key, sizeof(resp.key));
                resp.data = out;
                pCharacteristic->notify((uint8_t *)&resp, sizeof(resp), true);
            }
            break;
        }
        case pkt::CONFIG_POS_I32:
        case pkt::CONFIG_NEG_I32: {
            int32_t out = 0;
            auto ret = cfg_mgr.get_i32(req->key, out);

            pkt::num_cfg_pkt resp = {};
            if (ret != ESP_OK) {
                resp.type = pkt::CONFIG_ERROR_GENERIC;
            } else {
                resp.type = out < 0 ? pkt::CONFIG_NEG_I32 : pkt::CONFIG_POS_I32;
                memcpy(resp.key, req->key, sizeof(resp.key));
                resp.data = abs(out);
                pCharacteristic->notify((uint8_t *)&resp, sizeof(resp), true);
            }
            break;
        }
        case pkt::CONFIG_STR: {
            char buf_out[128] = {};
            auto ret = cfg_mgr.get_str(req->key, buf_out, sizeof(buf_out));

            pkt::blob_str_cfg_pkt resp = {};
            if (ret != ESP_OK) {
                resp.type = pkt::CONFIG_ERROR_GENERIC;
            } else {
                resp.type = pkt::CONFIG_STR;
                size_t out_len = strnlen(buf_out, sizeof(buf_out));
                memcpy(resp.key, req->key, sizeof(resp.key));
                memcpy(resp.data, buf_out, out_len);
                resp.len = (uint8_t)(out_len & 0xff);
                pCharacteristic->notify((uint8_t *)&resp, sizeof(resp), true);
            }
            break;
        }
        case pkt::CONFIG_BLOB: {
            uint8_t buf_out[128] = {};
            auto ret = cfg_mgr.get_blob(req->key, buf_out, sizeof(buf_out));

            pkt::blob_str_cfg_pkt resp = {};
            if (ret != ESP_OK) {
                resp.type = pkt::CONFIG_ERROR_GENERIC;
            } else {
                resp.type = pkt::CONFIG_STR;
                memcpy(resp.key, req->key, sizeof(resp.key));
                memcpy(resp.data, buf_out, sizeof(buf_out));
                resp.len = sizeof(resp.data);
                pCharacteristic->notify((uint8_t *)&resp, sizeof(resp), true);
            }
            break;
        }
        case pkt::CONFIG_OK: {
            pkt::cfg_key_pkt ack = {};
            ack.type = pkt::CONFIG_OK;
            memcpy(ack.key, SIMPLE_ACK_MAGIC, sizeof(SIMPLE_ACK_MAGIC));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }

        default: {
            pkt::cfg_key_pkt ack = {};
            ack.type = pkt::CONFIG_ERROR_UNKNOWN_TYPE;
            memcpy(ack.key, SIMPLE_ACK_MAGIC, sizeof(SIMPLE_ACK_MAGIC));
            pCharacteristic->notify((uint8_t *)&ack, sizeof(ack), true);
            break;
        }
    }
}
