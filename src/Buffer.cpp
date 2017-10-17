#include "Buffer.h"

#include <algorithm>

char* Buffer::slide (uint32_t _seq_num) {
    bool allowed = false;
    char* p_addr = nullptr;
    
    if (win_size > 0) {
        uint32_t laf = cur_seq_num + win_size;

        if (cur_seq_num > laf) {
            allowed = (_seq_num >= cur_seq_num) || (_seq_num < laf);
        } else {
            allowed = (_seq_num >= cur_seq_num) && (_seq_num < laf);
        }
    }

    if (allowed) {
        size_t i_win = _seq_num - cur_seq_num;
        size_t i_buf = (win_begin + i_win) % size;

        size_t prev_begin = win_begin;

        // Accept frame.
        p_addr = &data[i_buf];
        win_flag |= 1 << i_win;

        // Shift if needed.
        while (win_flag.test (0)) {
            win_flag >>= 1;

            // Advance everything.
            cur_seq_num += 1;
            win_begin = (win_begin + 1) % size;

            data_size--;
        }

        win_size = std::min (data_size, pref_win_size);
    }

    return p_addr;
}
