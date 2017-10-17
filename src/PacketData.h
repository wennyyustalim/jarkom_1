#pragma once

#include <algorithm>
#include <cstdint>

struct PacketData {
    uint8_t soh;
    uint32_t seq_num;
    uint8_t stx;
    uint8_t data;
    uint8_t etx;
    uint8_t checksum;

    uint8_t calculate_checksum () {        
        uint8_t* pb = (uint8_t*) this;
        uint8_t* pe = (uint8_t*) &this->checksum;

        uint8_t sum = std::accumulate (pb, pe, 0);
        return sum;
    }

    bool verify() {
        return checksum == calculate_checksum();
    }

    void prepare() {
        checksum = calculate_checksum();
    }
} __attribute__((__packed__));