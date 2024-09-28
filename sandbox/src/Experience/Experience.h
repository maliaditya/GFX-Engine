// experience.h
#pragma once

#include "OpenGLContext.h"

#include "utils/Logger.h"  // Include path for the Logger class

class Experience {
public:
    Logger log;  // Logger instance for logging messages

    // Constructor that initializes the logger with a log file name
    Experience();

    // Method declarations for updating and rendering
    void update();
    void render(HDC hdc);
};
