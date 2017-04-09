//
// Created by Jerome Jahnke on 4/8/17.
//

#include <cstdlib>
#include <cstring>
#include "IOBuffer.h"

IOBuffer::IOBuffer() {
    b = nullptr;
    len = 0;
}

void IOBuffer::append(const char *s, size_t s_len) {
    b = (char*)std::realloc(b, len + s_len);

    if( b == nullptr) return;

    std::memcpy(&b[len], s, s_len);
    len += s_len;
}

void IOBuffer::free() {
    std::free(b);
    b = nullptr;
    len = 0;
}

char *IOBuffer::getBytes() {
    return b;
}

int IOBuffer::getLen() {
    return len;
}

IOBuffer::~IOBuffer() {
    this->free();
}
