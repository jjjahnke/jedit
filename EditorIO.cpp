//
// Created by Jerome Jahnke on 4/8/17.
//

#include <cstdlib>
#include <iostream>
#include "EditorIO.h"


EditorInput::EditorInput(Terminal *terminal) : terminal(terminal) {}

void EditorInput::processKeyPress() {
    char c = terminal->editorReadKey();
    switch(c) {
        case CTRL_KEY('q'):
            terminal->clear_terminal();
            exit(0);
            break;
    }
}

EditorOutput::EditorOutput(Terminal *terminal) : terminal(terminal) {}

void EditorOutput::drawRows() {
    int y;
    for(y = 0; y < terminal->getScreenrows(); y++) {
        std::cout << "~\r\n";
    }
}

void EditorOutput::refreshScreen() {
    terminal->clear_terminal();

    drawRows();

    std::cout << "\x1b[H";
}

