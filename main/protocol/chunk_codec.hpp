#pragma once

#include <cstdint>
#include <NimBLECharacteristic.h>


namespace pkt
{
    struct __attribute__((packed)) chunk_metadata {
        uint32_t len;
        uint32_t crc;
    };

    struct __attribute__((packed)) chunk_content {
        uint32_t chunk_offset;
        uint8_t chunk_len;
        uint8_t buf[255];
    };
}

class chunk_codec
{

};

