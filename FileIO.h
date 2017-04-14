//
// Created by Jerome Jahnke on 4/12/17.
//

#ifndef JEDIT_FILEIO_H
#define JEDIT_FILEIO_H

#include <vector>
#include <sys/_types/_ssize_t.h>

class EditorRow {
private:
    int size;
    char *chars;
    int num_rows;

public:
    void addRow(int size, const char *chars);

    int getSize() const;

    void setSize(int size);

    char *getChars() const;

    void setChars(char *chars);

    int getNum_rows() const;

    void setNum_rows(int num_rows);
};

class FileIO {
public:
    std::vector<EditorRow> * open(const char *filename);
};


#endif //JEDIT_FILEIO_H
