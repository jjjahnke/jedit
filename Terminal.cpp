//
// Created by Jerome Jahnke on 4/8/17.
//

#include <iostream>
#include <zconf.h>
#include <sys/ioctl.h>
#include "Terminal.h"

int Terminal::cx;
int Terminal::cy;
int Terminal::init;
int Terminal::screenrows;
int Terminal::screencols;
struct termios Terminal::orig_termios;

void Terminal::clear_terminal() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void Terminal::exit_on_error(const std::string s) {
    clear_terminal();

    perror(s.c_str());
    exit(1);
}

void Terminal::disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &(orig_termios)) == -1)
        exit_on_error("tcsetattr");
}

void Terminal::enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &(orig_termios)) == -1) exit_on_error("tcgetattr");
    std::atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) exit_on_error("tcsetattr");
}

int Terminal::readKey() {
    int nread;
    char c;

    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1) exit_on_error("read");
    }

    if(c == '\x1b') {
        char seq[3];
        if(read(STDIN_FILENO, &seq[0],1) != 1) return '\x1b';
        if(read(STDIN_FILENO, &seq[1],1) != 1) return '\x1b';

        if(seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {

                if(read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';

                if (seq[2] == '~') {

                    switch (seq[1]) {
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        } else if( seq[0] == '0') {
            switch (seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}

int Terminal::getCursorPosition(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;

    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;


    while(i < sizeof(buf) -1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

    return 0;
}


int Terminal::getWindowSize(int *rows, int *cols) {
    struct winsize ws;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void Terminal::initTerminal() {
    if( init == 0 ) {
        cx = 0;
        cy = 0;

        enableRawMode();
        if (getWindowSize(&(screenrows), &(screencols)) == -1)
            exit_on_error("getWindowSize");
        init = 1;
    }
}

int Terminal::getScreenrows() {
    return screenrows;
}

int Terminal::getScreencols() {
    return screencols;
}

Terminal::Terminal() {
    init = 0;
    initTerminal();
}

int Terminal::getCx() {
    return cx;
}

int Terminal::getCy() {
    return cy;
}

void Terminal::setCx(int cx) {
    Terminal::cx = checkCx(cx);
}

void Terminal::setCy(int cy) {
    Terminal::cy = checkCy(cy);
}

void Terminal::incrCx(int move_cx) {
    cx = checkCx(cx + move_cx);
}

void Terminal::decrCx(int move_cx) {
    cx = checkCx(cx - move_cx);
}

void Terminal::incrCy(int move_cy) {
    cy = checkCy(cy + move_cy);
}

void Terminal::decrCy(int move_cy) {
    cy = checkCy(cy - move_cy);
}

int Terminal::checkCx(int new_cx) {
    if (new_cx >= screencols) return screencols - 1;
    if( new_cx <= 1) return 1;
    return new_cx;
}

int Terminal::checkCy(int new_cy) {
    if (new_cy >= screenrows) return screenrows - 1;
    if( new_cy < 0) return 0;
    return new_cy;
}