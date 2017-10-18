#pragma once

#include <cstdint>
#include <cstdlib>

class Buffer {
public:
    // Constructor.

    Buffer (size_t _size = 256);

    Buffer (const Buffer& _src) = delete;
    Buffer& operator= (const Buffer& _src) = delete;

    virtual ~Buffer (void);

    // Tools.

    bool accept (uint32_t _seq_num, size_t& _i_win);
    void shift (size_t _len, bool _flush = true);

    const size_t size;

    size_t win_size = 0;

protected:
    virtual void buffer_flush (void) = 0;

    uint32_t cur_seq_num = 0;

    char* data;

    size_t data_size = 0;
    size_t win_begin = 0;
};
