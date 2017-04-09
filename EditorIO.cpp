//
// Created by Jerome Jahnke on 4/8/17.
//

#include <cstdlib>
#include <iostream>
#include <zconf.h>
#include "EditorIO.h"
#include "Config.h"

using namespace std;

EditorInput::EditorInput(Terminal *terminal) : terminal(terminal) {}

void EditorInput::processKeyPress() {
    char c = terminal->editorReadKey();
    switch(c) {
        case CTRL_KEY('q'):
            terminal->clear_terminal();
            exit(0);
            break;

        case 'a':
        case 'd':
        case 'w':
        case 's':
            moveCursor(c);
            break;
    }
}

void EditorInput::moveCursor(char key) {
    switch (key) {
        case 'a':
            terminal->decrCx();
            break;
        case 'd':
            terminal->incrCx();
            break;
        case 'w':
            terminal->decrCy();
            break;
        case 's':
            terminal->incrCy();
            break;
    }
}

EditorOutput::EditorOutput(Terminal *terminal) : terminal(terminal) {}

void EditorOutput::drawRows(IOBuffer *iobuf) {
    int y;
    for(y = 0; y < terminal->getScreenrows(); y++) {
        if(y == terminal->getScreenrows()/3) {
            drawAbout(iobuf, terminal->getScreencols());
        } else {
            iobuf->append("~", 1);
        }
        iobuf->append("\x1b[K", 3);
        if( y < terminal->getScreenrows() -1) {
            iobuf->append("\r\n", 2);
        }
    }
}

void EditorOutput::refreshScreen() {
    IOBuffer iobuf;
    iobuf.append("\x1b[?25l", 6);
    iobuf.append("\x1b[H", 3);
    drawRows(&iobuf);
    placeCursor(&iobuf);
    iobuf.append("\x1b[?25h", 6);
    write(STDOUT_FILENO, iobuf.getBytes(), iobuf.getLen());
}

void EditorOutput::drawAbout(IOBuffer *iobuf, int width) {
    string version = JEDIT_VERSION;
    string welcome =  "JEdit -- version " + version;

    int welcome_len = welcome.length();
    if(welcome_len > width) welcome_len = width;
    int padding = (width - welcome_len)/2;
    if(padding) {
        iobuf->append("~", 1);
        padding--;
    }
    while(padding--) iobuf->append(" ", 1);

    iobuf->append(welcome.c_str(), welcome_len);
}

void EditorOutput::placeCursor(IOBuffer *iobuf) {
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", terminal->getCy() + 1, terminal->getCx() + 1);
    iobuf->append(buf, strlen(buf));
}


