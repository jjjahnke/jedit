//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_EDITORINPUT_H
#define JEDIT_EDITORINPUT_H

#include "Terminal.h"

#define CTRL_KEY(k) ((k) & 0x1f)

class EditorInput {
private:
    Terminal *terminal;

public:
    EditorInput(Terminal *terminal);

    void processKeyPress();
};

class EditorOutput {
private:
    Terminal *terminal;

public:
    EditorOutput(Terminal *terminal);

    void drawRows();
    void refreshScreen();
};

#endif //JEDIT_EDITORINPUT_H
