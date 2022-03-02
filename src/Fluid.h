#pragma once

#include "FluidUtils.h"
#include "ofGraphics.h"
#include "ofMain.h"

class Fluid {
 public:
  int sz{N};
  float dt;  // timestep
  float diff;
  float visc;
  vector<float> s;
  vector<float> density;
  vector<float> Vx;
  vector<float> Vy;
  vector<float> Vx0;
  vector<float> Vy0;

  void setup(float dt_, float diff_, float visc_) {
    dt = dt_;
    diff = diff_;
    visc = visc_;
    s.resize(sz * sz);
    density.resize(sz * sz);
    Vx.resize(sz * sz);
    Vy.resize(sz * sz);
    Vx0.resize(sz * sz);
    Vy0.resize(sz * sz);
  }

  void addDensity(int x, int y, float amount) {
    auto idx = FluidUtils::IX(x, y);
    density[idx] += amount;
    density[idx] = FluidUtils::min(density[idx], 255.0f);
  }

  void addVelocity(int x, int y, float amountX, float amountY) {
    int idx = FluidUtils::IX(x, y);
    Vx[idx] += amountX;
    Vy[idx] += amountY;
  }

  void step() {
    FluidUtils::diffuse(1, Vx0, Vx, visc, dt);
    FluidUtils::diffuse(2, Vy0, Vy, visc, dt);

    FluidUtils::project(Vx0, Vy0, Vx, Vy);

    FluidUtils::advect(1, Vx, Vx0, Vx0, Vy0, dt);
    FluidUtils::advect(2, Vy, Vy0, Vx0, Vy0, dt);

    FluidUtils::project(Vx, Vy, Vx0, Vy0);

    FluidUtils::diffuse(0, s, density, diff, dt);
    FluidUtils::advect(0, density, s, Vx, Vy, dt);
  }

  void renderD() const {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        auto d = density[FluidUtils::IX(i, j)];
        ofSetColor(d);
        ofDrawRectangle(i * SCALE, j * SCALE, SCALE, SCALE);
      }
    }
  }
};
