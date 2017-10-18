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

    char* slide (uint32_t _seq_num);

    const size_t size;

    size_t win_size = 0;

protected:
    virtual void buffer_flush (void) = 0;

    uint32_t cur_seq_num = 0;

    char* data;
    bool* data_flags;

    size_t data_size = 0;
    size_t win_begin = 0;
};
