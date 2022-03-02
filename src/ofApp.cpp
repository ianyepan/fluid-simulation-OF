#include "ofApp.h"
#include "Fluid.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofGraphics.h"
#include "ofMath.h"
#include "vector_float2.hpp"

void ofApp::setup() {
  ofSetFrameRate(60);
  ofBackground(0);
  fluid.setup(0.2, 0, 0.0000001);
}

void ofApp::update() {}

void ofApp::draw() {
  const int centerX = (int)(0.5 * ofGetWindowWidth() / SCALE);
  const int centerY = (int)(0.5 * ofGetWindowHeight() / SCALE);
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      fluid.addDensity(centerX + i, centerY + j, ofRandom(50, 150));
    }
  }
  for (int i = 0; i < 2; i++) {
    auto noise = ofNoise(ofGetElapsedTimef());
    auto angle = noise * ofDegToRad(360.0f);
    fluid.addVelocity(centerX, centerY, cosf(angle), sinf(angle));
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
