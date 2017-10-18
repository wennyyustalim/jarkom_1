#include <algorithm>

#include "Buffer.h"

Buffer::Buffer (size_t _size) : size (_size) {
    data = new char[size] ();
    data_flags = new bool[size] ();
}

Buffer::~Buffer (void) {
    delete data;
    delete data_flags;
}

char* Buffer::slide (uint32_t _seq_num) {
    bool allowed = false;
    char* data_ptr = nullptr;
    
    // Fix window size if invalid.
    win_size = std::min (win_size, data_size);

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

        // Accept frame.
        data_ptr = &data[i_buf];
        data_flags[i_buf] = true;

        // Shift if needed.
        while (data_flags[win_begin]) {
            data_flags[win_begin] = false;

            // Advance everything.
            cur_seq_num += 1;
            win_begin = (win_begin + 1) % size;

            data_size--;
        }
    }

    return data_ptr;
}
