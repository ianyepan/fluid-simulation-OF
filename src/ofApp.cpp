#include "ofApp.h"
#include "Fluid.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofGraphics.h"
#include "ofMath.h"
#include "vector_float2.hpp"

void ofApp::setup() {
  ofSetFrameRate(24);
  ofBackground(0);
  fluid.setup(0.25, 0, 0.0000001);
}

void ofApp::update() {}

void ofApp::draw() {
const int cx = (int)(0.25 * ofGetWindowWidth() / SCALE);
const int cy = (int)(0.25 * ofGetWindowHeight() / SCALE);
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      fluid.addDensity(cx + i, cy + j, ofRandom(50, 150));
    }
  }
  for (int i = 0; i < 2; i++) {
    auto angle = ofRandom(ofDegToRad(180.0));
    glm::vec2 v(angle);
    v *= ofRandom(1.0);
    fluid.addVelocity(cx, cy, v.x, v.y);
    // fluid.addVelocity(cx, cy, 0.5, 0.5);
  }

  fluid.step();
  fluid.renderD();
}

void ofApp::keyPressed(int key) {}

void ofApp::keyReleased(int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::mouseEntered(int x, int y) {}

void ofApp::mouseExited(int x, int y) {}

void ofApp::windowResized(int w, int h) {}

void ofApp::gotMessage(ofMessage msg) {}

void ofApp::dragEvent(ofDragInfo dragInfo) {}
