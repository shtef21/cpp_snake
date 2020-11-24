#include <iostream>

// Remove file
#include <stdio.h>

#include "App"

int main(int argc, char *argv[])
{
    App::run();

    // Clean directory
    remove("main.obj");
    return 0;
}