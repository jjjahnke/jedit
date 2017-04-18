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

    int render_size;
    char *render_chars;
    int *render_map;
    int *char_map;

    static const int tab_stop = 4;

    int computeTabStop(const int start);


public:
    EditorRow(int size, const char *chars);

    int getSize() const;
    void setSize(int size);

    char *getChars() const;
    void setChars(char *chars);

    void render();

    int getRender_size() const;
    char *getRender_chars() const;

    int *getRender_map() const;
    int *getChar_map() const;

    int mapCharToRender(const int curLoc) const;
};

class FileIO {
public:
    std::vector<EditorRow> * open(const char *filename);
};


#endif //JEDIT_FILEIO_H
