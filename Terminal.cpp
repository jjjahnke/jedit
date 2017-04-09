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

char Terminal::editorReadKey() {
    int nread;
    char c;

    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1) exit_on_error("read");
    }
    return c;
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
    Terminal::init = 0;
    Terminal::initTerminal();
}

int Terminal::getCx() {
    return cx;
}

int Terminal::getCy() {
    return cy;
}

void Terminal::setCx(int cx) {
    Terminal::cx = cx;
}

void Terminal::setCy(int cy) {
    Terminal::cy = cy;
}

void Terminal::incrCx() {
    cx++;
    checkCx();
}

void Terminal::decrCx() {
    cx--;
    checkCx();
}

void Terminal::incrCy() {
    cy++;
    checkCy();
}

void Terminal::decrCy() {
    cy--;
    checkCy();
}

void Terminal::checkCx() {
    if (cx >= screencols) cx = screencols - 1;
    if( cx <= 1) cx = 1;
}

void Terminal::checkCy() {
    if (cy >= screenrows) cy = screenrows - 1;
    if( cy < 0) cy = 0;
}
