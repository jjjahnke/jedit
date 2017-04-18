//
// Created by Jerome Jahnke on 4/12/17.
//

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "FileIO.h"

using namespace std;

EditorRow::EditorRow(int size, const char *chars) : size(size) {
    this->size= size;
    this->chars = (char *)malloc(this->size + 1);
    memcpy((void *) this->chars, chars, this->size);
    this->chars[this->size] = '\0';
    this->render_chars = nullptr;
    render();
}

int EditorRow::getSize() const {
    return size;
}

void EditorRow::setSize(int size) {
    EditorRow::size = size;
}

char *EditorRow::getChars() const {
    return chars;
}

void EditorRow::render() {
    if(render_chars != nullptr) free(render_chars);
    int cur_size = getSize() + 1;
    render_chars = (char*)malloc(cur_size);
    render_map = (int*)malloc(cur_size * sizeof *render_map);
    char_map = (int*)malloc(cur_size * sizeof *render_map);

    int j;
    int idx = 0;
    for(j=0; j < getSize(); j++) {
        char_map[j] = idx;
        if(getChars()[j] == '\t') {
            int tab_spaces = computeTabStop(idx);
            cur_size += tab_spaces;
            render_chars = (char*)realloc(render_chars, cur_size);
            render_map = (int*)realloc(render_map, cur_size * sizeof *render_map);
            for( int x = 0; x < tab_spaces; x++) {
                render_map[idx] = j;
                render_chars[idx++] = '.';
            }
        } else {
            render_map[idx]=j;
            render_chars[idx++] = getChars()[j];
        }
    }
    render_map[idx]=j;
    char_map[j] = idx;
    render_chars[idx] = '\0';
    render_size = idx;
}

void EditorRow::setChars(char *chars) {
    EditorRow::chars = chars;
}

int EditorRow::getRender_size() const {
    return render_size;
}

char *EditorRow::getRender_chars() const {
    return render_chars;
}

int EditorRow::computeTabStop(const int start) {
    return tab_stop - start%tab_stop;
}

int *EditorRow::getRender_map() const {
    return render_map;
}

int *EditorRow::getChar_map() const {
    return char_map;
}

int EditorRow::mapCharToRender(const int curLoc) const {
    if (curLoc < size) {
        return char_map[curLoc];
    } else {
        return char_map[size];
    }
}

vector<EditorRow> * FileIO::open(const char *filename) {
    vector<EditorRow> *retVal = new vector<EditorRow>();

    ifstream file(filename);
    if( file.is_open() ) {
        string line;
        unsigned long linelen;
        while (getline(file, line)) {
            linelen = line.length();
            while (linelen > 0 && (line[linelen - 1] == '\n' ||
                                   line[linelen - 1] == '\r'))
                linelen--;
            retVal->push_back(EditorRow(linelen, line.c_str()));
        }
        file.close();
    }
    return retVal;
}
