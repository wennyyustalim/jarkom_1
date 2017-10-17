#include "Buffer.h"

#include <algorithm>

size_t Buffer::slide (uint32_t _seq_num) {
    bool allowed = false;
    size_t count = 0;
    
    if (win_size > 0) {
        uint32_t laf = cur_seq_num + win_size;

        if (cur_seq_num > laf) {
            allowed = (_seq_num >= cur_seq_num) || (_seq_num < laf);
        } else {
            allowed = (_seq_num >= cur_seq_num) && (_seq_num < laf);
        }
    }

    if (allowed) {
        size_t n = _seq_num - cur_seq_num;

        // Accept frame.
        win_flag |= 1 << n;

        // Shift if needed.
        while (win_flag.test (0)) {
            count++;
            win_flag >>= 1;

            // Advance everything.
            cur_seq_num += 1;
            win_begin = (win_begin + 1) % size;

            data_size--;
        }

        win_size = std::min (data_size, pref_win_size);
    }

    return count;
}
