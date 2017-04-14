//
// Created by Jerome Jahnke on 4/12/17.
//

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "FileIO.h"

using namespace std;

void EditorRow::addRow(int size, const char *chars) {
    this->size= size;
    this->chars = (char *)malloc(this->size + 1);
    memcpy((void *) this->chars, chars, this->size);
    this->chars[this->size] = '\0';
    this->num_rows = 1;
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

void EditorRow::setChars(char *chars) {
    EditorRow::chars = chars;
}

int EditorRow::getNum_rows() const {
    return num_rows;
}

void EditorRow::setNum_rows(int num_rows) {
    EditorRow::num_rows = num_rows;
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
            EditorRow *eRow = new EditorRow();
            eRow->addRow(linelen, line.c_str());
            retVal->push_back(*eRow);
        }
        file.close();
    }
    return retVal;
}
