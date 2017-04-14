//
// Created by Jerome Jahnke on 4/8/17.
//

#include <iostream>
#include "Config.h"
#include "Terminal.h"
#include "EditorIO.h"

int main(int argc, char *argv[]) {
    Terminal *t = new Terminal();
    EditorOutput *output = new EditorOutput(t, nullptr, argv[1]);

    while(1) {
        output->refreshScreen();
        output->getEditor_input()->processKeyPress();
    }

    return 0;
}