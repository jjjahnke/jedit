//
// Created by Jerome Jahnke on 4/8/17.
//

#include <cstdlib>
#include <iostream>
#include <zconf.h>
#include "EditorIO.h"
#include "Config.h"

using namespace std;

EditorInput::EditorInput(Terminal *terminal) : terminal(terminal) {
    this->setRow_offset(0);
    this->setCol_offset(0);
}

void EditorInput::processKeyPress() {
    int c = terminal->readKey();
    switch(c) {
        case CTRL_KEY('q'):
            terminal->clear_terminal();
            exit(0);
            break;
        case ARROW_LEFT:
        case ARROW_RIGHT:
        case ARROW_UP:
        case ARROW_DOWN:
        case PAGE_UP:
        case PAGE_DOWN:
        case HOME_KEY:
        case END_KEY:
            moveCursor(c);
            break;
        default:
            break;
    }
}

void EditorInput::moveCursor(int key) {
    switch (key) {
        case ARROW_LEFT:
            if(terminal->getCx() == 0) decrCol_offset();
            terminal->decrCx();
            break;
        case ARROW_RIGHT:
            if(terminal->getCx() == terminal->getScreencols() -1) incrCol_offset();
            terminal->incrCx();
            break;
        case ARROW_UP:
            if(terminal->getCy() == 0) decrRow_offset();
            terminal->decrCy();
            break;
        case ARROW_DOWN:
            if(terminal->getCy() == terminal->getScreenrows() - 1) incrRow_offset();
            terminal->incrCy();
            break;
        case PAGE_UP:
            if(terminal->getCy() == 0) decrRow_offset(terminal->getScreenrows());
            terminal->decrCy(terminal->getScreenrows());
            break;
        case PAGE_DOWN:
            if(terminal->getCy() == terminal->getScreenrows() - 1) incrRow_offset(terminal->getScreenrows());
            terminal->incrCy(terminal->getScreenrows());
            break;
        case HOME_KEY:
            if(terminal->getCx() == 0) decrCol_offset(terminal->getScreencols());
            terminal->setCx(0);
            break;
        case END_KEY:
            if(terminal->getCx() == terminal->getScreencols() -1) incrCol_offset(terminal->getScreencols());
            terminal->setCx(terminal->getScreencols());
            break;
        default:
            break;
    }
}

int EditorInput::getRow_offset() const {
    return row_offset;
}

void EditorInput::setRow_offset(int row_offset) {
    EditorInput::row_offset = row_offset;
}

void EditorInput::incrRow_offset(int move_rows) {
    row_offset += move_rows;
}

void EditorInput::decrRow_offset(int move_rows) {
    row_offset -= move_rows;
}

int EditorInput::getCol_offset() const {
    return col_offset;
}

void EditorInput::setCol_offset(int col_offset) {
    EditorInput::col_offset = col_offset;
}

void EditorInput::incrCol_offset(int move_cols) {
    col_offset += move_cols;
}

void EditorInput::decrCol_offset(int move_cols) {
    col_offset -= move_cols;
}

int EditorInput::mapCursorX() {
    return terminal->getCx() + getCol_offset();
}

int EditorInput::mapCursorY() {
    return terminal->getCy() + getRow_offset();
}


EditorOutput::EditorOutput(Terminal *terminal, EditorInput *editor_input, char *filename) : terminal(terminal), editor_input(editor_input) {
    this->editor_input = editor_input == nullptr ? new EditorInput(terminal) : editor_input;
    rows = FileIO().open(filename);
}

void EditorOutput::drawRows(IOBuffer *iobuf) {
    int y;
    for(y = 0; y < terminal->getScreenrows() -2; y++) {
        int file_row = y + editor_input->getRow_offset();
        if( file_row >= rows->size() ) {
            if (rows->size() == 0 && y == terminal->getScreenrows() / 3) {
                drawAbout(iobuf, terminal->getScreencols());
            } else {
                iobuf->append("~", 1);
            }
        } else {
            EditorRow row = rows->at(file_row);
            int len = row.getRender_size() - editor_input->getCol_offset();
            if (len > terminal->getScreencols()) len = terminal->getScreencols();
            if( len < 0)  len = 0;
            iobuf->append(&(row.getRender_chars()[editor_input->getCol_offset()]), len);
        }
        iobuf->append("\x1b[K", 3);
        if( y < terminal->getScreenrows() -1) {
            iobuf->append("\r\n", 2);
        }
    }
}

void EditorOutput::drawCursorInfo(IOBuffer *iobuf) {
    char buf[128];
    snprintf(buf, sizeof(buf), "--\r\nscr_loc:%d,%d map_loc:%d,%d offset:%d,%d size:%d|%d            ",
             terminal->getCx(),
             terminal->getCy(),
             editor_input->mapCursorX(),
             editor_input->mapCursorY(),
             editor_input->getRow_offset(),
             editor_input->getCol_offset(),
             currentRenderRowSize(),
             currentCharRowSize()
    );
    iobuf->append(buf, strlen(buf));
}

void EditorOutput::refreshScreen() {
    IOBuffer iobuf;

    scroll();
    iobuf.append("\x1b[?25l", 6);
    iobuf.append("\x1b[H", 3);
    drawRows(&iobuf);
    drawCursorInfo(&iobuf);
    placeCursor(&iobuf);
    iobuf.append("\x1b[?25h", 6);
    write(STDOUT_FILENO, iobuf.getBytes(), iobuf.getLen());
}

void EditorOutput::drawAbout(IOBuffer *iobuf, int width) {
    string version = JEDIT_VERSION;
    string welcome =  "JediT -- version " + version;

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
    int mapped_cursor = rows->at(terminal->getCy()).mapCharToRender(terminal->getCx());
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", terminal->getCy() + 1, mapped_cursor + 1);
    iobuf->append(buf, strlen(buf));
}

void EditorOutput::scroll() {
    if( editor_input->getRow_offset() < 0) {
        editor_input->setRow_offset(0);
    }
    if( editor_input->getRow_offset() >= rows->size()) {
        editor_input->setRow_offset(rows->size() - 1);
    }
    if( editor_input->getCol_offset() < 0) {
        editor_input->setCol_offset(0);
    }
    if(editor_input->mapCursorX() > currentCharRowSize()) {
        terminal->setCx(currentCharRowSize() - editor_input->getCol_offset());
    }
}

int EditorOutput::currentRenderRowSize() const {
    int cur_row_size = 0;
    if (editor_input->mapCursorY() < rows->size()) {
        cur_row_size = rows->at(editor_input->mapCursorY()).getRender_size();
    }
    return cur_row_size;
}

EditorInput *EditorOutput::getEditor_input() const {
    return editor_input;
}

int EditorOutput::currentCharRowSize() const {
    int cur_row_size = 0;
    if (editor_input->mapCursorY() < rows->size()) {
        cur_row_size = rows->at(editor_input->mapCursorY()).getSize();
    }
    return cur_row_size;
}
