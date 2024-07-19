#pragma once

#include "ofMain.h"
#include <CL/cl.hpp>

struct Particle {
    float x, y;
    float vx, vy;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void setupOpenCL();
    void moveParticles();

    cl::Context context;
    cl::CommandQueue queue;
    cl::Program program;
    cl::Kernel kernel;
    std::vector<Particle> particles;
    cl::Buffer particleBuffer;
    const int numParticles = 10000;
};
