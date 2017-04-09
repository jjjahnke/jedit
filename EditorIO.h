//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_EDITORINPUT_H
#define JEDIT_EDITORINPUT_H

#include "Terminal.h"
#include "IOBuffer.h"

#define CTRL_KEY(k) ((k) & 0x1f)

class EditorInput {
private:
    Terminal *terminal;

public:
    EditorInput(Terminal *terminal);

    void processKeyPress();
    void moveCursor(char key);
};

class EditorOutput {
private:
    Terminal *terminal;

public:
    EditorOutput(Terminal *terminal);

    void drawAbout(IOBuffer *iobuf, int width);
    void placeCursor(IOBuffer *iobuf);

    void drawRows(IOBuffer *iobuf);
    void refreshScreen();
};

#endif //JEDIT_EDITORINPUT_H
