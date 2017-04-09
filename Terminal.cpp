//
// Created by Jerome Jahnke on 4/8/17.
//

#include <iostream>
#include <zconf.h>
#include <sys/ioctl.h>
#include "Terminal.h"

int Terminal::init;
int Terminal::screenrows;
int Terminal::screencols;
struct termios Terminal::orig_termios;

void Terminal::clear_terminal() {
    std::cout << "\x1b[2J";
    std::cout << "\x1b[H";
}

void Terminal::exit_on_error(const std::string s) {
    Terminal::clear_terminal();

    perror(s.c_str());
    exit(1);
}

void Terminal::disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &(Terminal::orig_termios)) == -1)
        Terminal::exit_on_error("tcsetattr");
}

void Terminal::enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &(Terminal::orig_termios)) == -1) Terminal::exit_on_error("tcgetattr");
    std::atexit(Terminal::disableRawMode);

    struct termios raw = Terminal::orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) Terminal::exit_on_error("tcsetattr");
}

char Terminal::editorReadKey() {
    int nread;
    char c;

    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1) Terminal::exit_on_error("read");
    }
    return c;
}

int Terminal::getWindowSize(int *rows, int *cols) {
    struct winsize ws;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        editorReadKey();
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void Terminal::initTerminal() {
    if( Terminal::init == 0 ) {
        Terminal::enableRawMode();
        if (Terminal::getWindowSize(&(Terminal::screenrows), &(Terminal::screencols)) == -1)
            Terminal::exit_on_error("getWindowSize");
        Terminal::init = 1;
    }
}

int Terminal::getScreenrows() {
    return screenrows;
}

int Terminal::getScreencols() {
    return screencols;
}

Terminal::Terminal() {
    Terminal::init = 0;
    Terminal::initTerminal();
}
