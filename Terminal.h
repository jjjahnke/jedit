//
// Created by Jerome Jahnke on 4/8/17.
//

#ifndef JEDIT_TERMINAL_H
#define JEDIT_TERMINAL_H

#include <string>
#include <termios.h>

class Terminal {
private:
    static int init;
    static int screenrows;
    static int screencols;
    static termios orig_termios;

public:
    Terminal();

    static void clear_terminal();
    static void exit_on_error(const std::string s);
    static void disableRawMode();
    static void enableRawMode();
    static char editorReadKey();
    static int getWindowSize(int *rows, int *cols);
    static void initTerminal();

    static int getScreenrows();

    static int getScreencols();
};

#endif //JEDIT_TERMINAL_H
