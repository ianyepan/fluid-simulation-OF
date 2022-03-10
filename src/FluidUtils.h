#pragma once

#include "ofAppRunner.h"
#include "ofMain.h"

const int N = 64;
const int ITER = 8;
const int SCALE = 4;

namespace FluidUtils {

template <typename T>
constexpr auto max(T a, T b) -> T {
  return a > b ? a : b;
}

template <typename T>
constexpr auto min(T a, T b) -> T {
  return a < b ? a : b;
}

template <typename T>
constexpr auto constrain(T x, T low, T high) -> T {
  x = FluidUtils::max(low, x);
  x = FluidUtils::min(high, x);
  return x;
}

// Get 2D location in 1D array
constexpr int IX(int x, int y) {
  x = constrain(x, 0, N - 1);
  y = constrain(y, 0, N - 1);
  return x + y * N;
}

inline void set_bnd(const int b, vector<float> &x) {
  for (int i = 1; i < N - 1; ++i) {
    x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
    x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
  }
  for (int j = 1; j < N - 1; ++j) {
    x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
    x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
  }

  x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
  x[IX(0, N - 1)] = 0.5f * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
  x[IX(N - 1, 0)] = 0.5f * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
  x[IX(N - 1, N - 1)] = 0.5f * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);
}

inline void lin_solve(const int b,
                      vector<float> &x,
                      vector<float> &x0,
                      const float a,
                      const float c) {
  float cRecip = 1.0 / c;
  for (int k = 0; k < ITER; ++k) {
    for (int j = 1; j < N - 1; ++j) {
      for (int i = 1; i < N - 1; ++i) {
        x[IX(i, j)] = cRecip * (x0[IX(i, j)] + a * (x[IX(i + 1, j)] + x[IX(i - 1, j)] +
                                                    x[IX(i, j + 1)] + x[IX(i, j - 1)]));
      }
    }
    set_bnd(b, x);
  }
}

inline void diffuse(const int b,
                    vector<float> &x,
                    vector<float> &x0,
                    const float diff,
                    const float dt) {
  auto a = dt * diff * (N - 2) * (N - 2);
  lin_solve(b, x, x0, a, 1 + 4 * a);
}

inline void project(vector<float> &velocX,
                    vector<float> &velocY,
                    vector<float> &p,
                    vector<float> &div) {
  for (int j = 1; j < N - 1; j++) {
    for (int i = 1; i < N - 1; i++) {
      div[IX(i, j)] = -0.5f *
                      (velocX[IX(i + 1, j)] - velocX[IX(i - 1, j)] + velocY[IX(i, j + 1)] -
                       velocY[IX(i, j - 1)]) /
                      N;
      p[IX(i, j)] = 0;
    }
  }

  set_bnd(0, div);
  set_bnd(0, p);
  lin_solve(0, p, div, 1, 4);

  for (int j = 1; j < N - 1; j++) {
    for (int i = 1; i < N - 1; i++) {
      velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) * N;
      velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) * N;
    }
  }
  set_bnd(1, velocX);
  set_bnd(2, velocY);
}

inline void advect(const int b,
                   vector<float> &d,
                   vector<float> &d0,
                   vector<float> &velocX,
                   vector<float> &velocY,
                   const float dt) {
  float i0, i1, j0, j1;

  float dtx = dt * (N - 2);
  float dty = dt * (N - 2);

  float s0, s1, t0, t1;
  float tmp1, tmp2, x, y;

  float Nfloat = N;
  float ifloat, jfloat;
  int i, j;

  for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
    for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
      tmp1 = dtx * velocX[IX(i, j)];
      tmp2 = dty * velocY[IX(i, j)];
      x = ifloat - tmp1;
      y = jfloat - tmp2;

      if (x < 0.5f)
        x = 0.5f;
      if (x > Nfloat + 0.5f)
        x = Nfloat + 0.5f;
      i0 = floor(x);
      i1 = i0 + 1.0f;
      if (y < 0.5f)
        y = 0.5f;
      if (y > Nfloat + 0.5f)
        y = Nfloat + 0.5f;
      j0 = floor(y);
      j1 = j0 + 1.0f;

      s1 = x - i0;
      s0 = 1.0f - s1;
      t1 = y - j0;
      t0 = 1.0f - t1;

      int i0i = int(i0);
      int i1i = int(i1);
      int j0i = int(j0);
      int j1i = int(j1);

      d[IX(i, j)] = s0 * (t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
                    s1 * (t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);
    }
  }

  set_bnd(b, d);
}

}  // namespace FluidUtils
