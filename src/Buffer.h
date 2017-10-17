#pragma once

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>

class Buffer {
public:
    Buffer (void) {
        memset (data, 0, sizeof (data));
    }

    static const size_t size = 256;

protected:
    char* slide (uint32_t _seq_num);

    uint32_t cur_seq_num = 0;

    char data[size];
    size_t data_size = 0;

    size_t pref_win_size = 0;

    size_t win_begin = 0;
    size_t win_size = 0;
    std::bitset<size> win_flag;
};
