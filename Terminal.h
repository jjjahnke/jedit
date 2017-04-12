//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_TERMINAL_H
#define JEDIT_TERMINAL_H

#include <string>
#include <termios.h>

enum editorKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};

class Terminal {
private:
    static int init;
    static int screenrows;
    static int screencols;
    static int cx;
    static int cy;
    static termios orig_termios;

public:
    Terminal();

    static void clear_terminal();
    static void exit_on_error(const std::string s);
    static void disableRawMode();
    static void enableRawMode();
    static int readKey();
    static int getWindowSize(int *rows, int *cols);
    static int getCursorPosition(int *rows, int *cols);
    static void initTerminal();
    static int getScreenrows();
    static int getScreencols();
    static int getCx();
    static void setCx(int cx);
    static void incrCx(int move_cx=1);
    static void decrCx(int move_cx=1);
    static int checkCx(int new_cx);
    static int getCy();
    static void setCy(int cy);
    static void incrCy(int move_cy=1);
    static void decrCy(int move_cy=1);
    static int checkCy(int new_cy);

};

#endif //JEDIT_TERMINAL_H
