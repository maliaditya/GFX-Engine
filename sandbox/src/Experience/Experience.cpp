// Experience.cpp
#include "Experience.h"
#include <iostream>

Experience::Experience(EventEmitter& eventEmitter) : eventEmitter(eventEmitter) {
    // Register event listeners
    eventEmitter.on("update", [this]() { Update(); });
    eventEmitter.on("render", [this]() { Render(); });
}

void Experience::Update() {
    // Update 3D world logic
    std::cout << "Updating the 3D world...\n";
}

void Experience::Render() {
    // Render 3D world
    std::cout << "Rendering the 3D world...\n";
}
