//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_IOBUFFER_H
#define JEDIT_IOBUFFER_H

class IOBuffer {
private:
    char *b;
    int len;

public:
    IOBuffer();
    virtual ~IOBuffer();
    void append(const char *s, size_t s_len);
    void free();
    char *getBytes();
    int getLen();
};

#endif //JEDIT_IOBUFFER_H
