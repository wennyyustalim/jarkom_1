#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

template<typename T>
class Buffer {
public:
    struct Item {
        T data;
        bool fin;
    };

    // Constructor.

    Buffer (size_t _size = 256)
        : size (_size) {
        data = new Item[size] ();
    }

    Buffer (const Buffer& _src) = delete;
    Buffer& operator= (const Buffer& _src) = delete;

    virtual ~Buffer (void) {
        delete data;
    }

    // Slide.

    T* slide (uint32_t _seq_num) {
        bool allowed = false;
        T* data_ptr = nullptr;
        
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
            Item& item = data[i_buf];
    
            data_ptr = &item.data;
            item.fin = true;
    
            // Shift if needed.
            while (data[win_begin].fin) {
                data[win_begin].fin = false;
    
                // Advance everything.
                cur_seq_num += 1;
                win_begin = (win_begin + 1) % size;
    
                data_size--;
            }
        }
    
        return data_ptr;
    }

    const size_t size;

    uint32_t cur_seq_num = 0;

    Item* data;

    size_t data_size = 0;

    size_t win_begin = 0;
    size_t win_size = 0;
};
