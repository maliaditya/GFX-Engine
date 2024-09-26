
// Experience.h
#pragma once
#include "EventEmitter.h"

class Experience {
public:
    Experience(EventEmitter& eventEmitter);
    ~Experience() = default;

    void Update();
    void Render();

private:
    EventEmitter& eventEmitter;
};

