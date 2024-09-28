// Experience.cpp
#include "Experience.h"
#include <iostream>

Experience::Experience():log("experience.log")
{

}

void Experience::update() {
    // Update 3D world logic
    log.write("Updating the 3D world...\n");
}


void Experience::render(HDC hdc) {
    // Render 3D world
    log.write("Rendering the 3D world...\n");
}
