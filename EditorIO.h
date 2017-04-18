//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_EDITORINPUT_H
#define JEDIT_EDITORINPUT_H

#include "Terminal.h"
#include "IOBuffer.h"
#include "FileIO.h"

#define CTRL_KEY(k) ((k) & 0x1f)

class EditorInput {
private:
    Terminal *terminal;
    int row_offset;
    int col_offset;

public:
    EditorInput(Terminal *terminal);

    void processKeyPress();
    void moveCursor(int key);

    int getRow_offset() const;
    void setRow_offset(int row_offset);
    void incrRow_offset(int move_rows=1);
    void decrRow_offset(int move_rows=1);
    int getCol_offset() const;
    void setCol_offset(int col_offset);
    void incrCol_offset(int move_cols=1);
    void decrCol_offset(int move_cols=1);

    int mapCursorX();
    int mapCursorY();

};

class EditorOutput {
private:
    Terminal *terminal;
    EditorInput *editor_input;
    std::vector<EditorRow> *rows;

public:
    EditorOutput(Terminal *terminal, EditorInput *editor_input, char *filename);

    void drawAbout(IOBuffer *iobuf, int width);
    void drawCursorInfo(IOBuffer *iobuf);
    void placeCursor(IOBuffer *iobuf);
    void scroll();

    void drawRows(IOBuffer *iobuf);
    void refreshScreen();

    EditorInput *getEditor_input() const;

    int currentRenderRowSize() const;
    int currentCharRowSize() const;
};

#endif //JEDIT_EDITORINPUT_H
