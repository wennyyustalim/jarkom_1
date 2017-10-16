#pragma once

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>

class Buffer {
public:
    Buffer (bool _empty = true)
        : end(0), empty (_empty), win_pref_size (0), win_begin (0), win_size (0) {
        memset (data, 0, sizeof (data));
    }

    bool full (void) {
        return (win_begin == end) && !empty;
    }

    static const size_t size = 256;

protected:
    size_t slide (uint32_t _seq_num);

    uint32_t cur_seq_num;

    char data[size];
    size_t end;
    bool empty;

    size_t win_pref_size;

    size_t win_begin;
    size_t win_size;
    std::bitset<size> win_flag;
};
