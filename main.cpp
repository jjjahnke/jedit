//
// Created by Jerome Jahnke on 4/8/17.
//

#include <iostream>
#include "Config.h"
#include "Terminal.h"
#include "EditorIO.h"

int main() {
    Terminal *t = new Terminal();
    EditorInput *input = new EditorInput(t);
    EditorOutput *output = new EditorOutput(t);

    while(1) {
        output->refreshScreen();
        input->processKeyPress();
    }

    return 0;
}