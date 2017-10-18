#include <algorithm>

#include "Buffer.h"

Buffer::Buffer (size_t _size)
    : size (_size) {
    data = new char[size] ();
}

Buffer::~Buffer (void) {
    delete data;
}

bool Buffer::accept (uint32_t _seq_num, size_t& _i) {
    bool allowed = false;

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

        _i = i_buf;
    }

    return allowed;
}

void Buffer::shift (size_t _len, bool _flush) {
    cur_seq_num += _len;
    win_begin = (win_begin + _len) % size;

    data_size -= _len;

    if (_flush) {
        buffer_flush ();
    }
}
