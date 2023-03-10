// Copyright 2019, Savvy Sine, Aline Normoyle
#include "agl/mesh/capsule.h"

#include <vector>
#include <cstdio>
#include <cmath>
#include <glm/gtc/constants.hpp>

namespace agl {

Capsule::Capsule(float radius, float length, GLuint nSlices, GLuint nStacks) {
  _radius = radius;
  _length = length;
  _nSlices = nSlices;
  _nStacks = nStacks;
}

void Capsule::init() {
  int nVerts = (_nSlices+1) * (_nStacks + 1);
  int elements = (_nSlices * 2 * (_nStacks-1) ) * 3;

  // Verts
  std::vector<GLfloat> p(3 * nVerts);
  // Normals
  std::vector<GLfloat> n(3 * nVerts);
  // Tex coords
  std::vector<GLfloat> tex(2 * nVerts);
  // Elements
  std::vector<GLuint> el(elements);

  // Generate positions and normals
  GLfloat theta, phi;
  GLfloat thetaFac = glm::two_pi<float>() / _nSlices;
  GLfloat phiFac = glm::pi<float>() / _nStacks;
  GLfloat nx, ny, nz, s, t, height;
  GLuint idx = 0, tIdx = 0;
  for (GLuint i = 0; i <= _nSlices; i++) {
    theta = i * thetaFac;
    s = (GLfloat)i / _nSlices;

    for (GLuint j = 0; j <= _nStacks; j++) {
      if (j < _nSlices * 0.5) {
        height = _length;
      } else {
        height = 0.0;
      }

      phi = j * phiFac;
      t = (GLfloat)j / _nStacks;
      nx = sinf(phi) * cosf(theta);
      ny = sinf(phi) * sinf(theta);
      nz = cosf(phi);

      p[idx] = _radius * nx;
      p[idx+1] = _radius * ny;
      p[idx+2] = _radius * nz + height;

      n[idx] = nx; n[idx+1] = ny; n[idx+2] = nz;
      idx += 3;

      tex[tIdx] = s;
      tex[tIdx+1] = t;
      tIdx += 2;
    }
  }

  // Generate the element list
  idx = 0;
  for (GLuint i = 0; i < _nSlices; i++) {
    GLuint stackStart = i * (_nStacks + 1);
    GLuint nextStackStart = (i+1) * (_nStacks+1);
    for (GLuint j = 0; j < _nStacks; j++) {
      if (j == 0) {
        el[idx] = stackStart;
        el[idx+1] = stackStart + 1;
        el[idx+2] = nextStackStart + 1;
        idx += 3;

      } else if (j == _nStacks - 1) {
        el[idx] = stackStart + j;
        el[idx+1] = stackStart + j + 1;
        el[idx+2] = nextStackStart + j;
        idx += 3;

      } else {
        el[idx] = stackStart + j;
        el[idx+1] = stackStart + j + 1;
        el[idx+2] = nextStackStart + j + 1;
        el[idx+3] = nextStackStart + j;
        el[idx+4] = stackStart + j;
        el[idx+5] = nextStackStart + j + 1;
        idx += 6;
      }
    }
  }

  initBuffers(&el, &p, &n, &tex);
}

}  // namespace agl
