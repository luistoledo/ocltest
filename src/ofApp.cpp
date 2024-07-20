#include "ofApp.h"
#include <fstream>
#include <sstream>

std::string loadKernel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open kernel file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void ofApp::setup() {
    //ofSetVerticalSync(true);
    ofSetFrameRate(30);

    setupOpenCL();

    // Initialize particles
    particles.resize(numParticles);
    for (auto& p : particles) {
        p.x = ofRandomWidth();
        p.y = ofRandomHeight();
        p.vx = ofRandom(-1, 1);
        p.vy = ofRandom(-1, 1);
    }

    // Create buffer for particles
    particleBuffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(Particle) * numParticles, particles.data());
}

void ofApp::setupOpenCL() {
    try {
        // Get all platforms (drivers), e.g. NVIDIA, Intel, AMD
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        if (platforms.empty()) {
            throw std::runtime_error("No OpenCL platforms found.");
        }

        // Get default device of the first platform
        cl_context_properties properties[] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
            0
        };
        context = cl::Context(CL_DEVICE_TYPE_GPU, properties);

        // Get a list of devices on this platform
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

        if (devices.empty()) {
            throw std::runtime_error("No OpenCL devices found.");
        }

        // Create a command queue and use the first device
        queue = cl::CommandQueue(context, devices[0]);

        // Load kernel code from file
        std::string kernelCode = loadKernel("moveParticles.cl");

        // Build the kernel code
        program = cl::Program(context, kernelCode);
        program.build(devices);

        // Create kernel
        kernel = cl::Kernel(program, "moveParticles");

        // Log build info
        std::string buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
        ofLog() << "Build Log: " << buildLog;

    }
    catch (std::exception& ex) {
        ofLog() << "Exception: " << ex.what();
    }
}

void ofApp::moveParticles() {
    try {
        // Set kernel arguments
        kernel.setArg(0, particleBuffer);

        // Enqueue kernel execution
        cl::NDRange global(numParticles);
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NullRange);
        queue.finish();

        int* w;
        //w[p]

        // Read the result back to host
        queue.enqueueReadBuffer(particleBuffer, CL_FALSE, 0, sizeof(Particle) * numParticles, particles.data());

        // Debug: Print first particle's position to verify movement
        //ofLog() << "First particle position: (" << particles[0].x << ", " << particles[0].y << ")";
    }
    catch (std::exception& ex) {
        ofLog() << "Exception: " << ex.what();
    }
}

void ofApp::update() {
    moveParticles();
}

void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    for (auto& p : particles) {
        ofDrawCircle(p.x, p.y, 2);
    }
    ofDrawBitmapString(ofGetFrameRate(), 10, 10);
}
