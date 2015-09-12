/*
 *  BoundingBoxes.cpp
 *
 *  Created on: 2014/10/19
 *      Author: Dimitri Kourkoulis
 *              http://dimitros.be
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "BoundingBoxes.hpp"
#include <fstream>
#include "Exception.hpp"
#include "GetTokens.hpp"
#include <stdlib.h>
#include "MathFunctions.hpp"
#include <dimitrikourk/glm/glm/glm.hpp>
#include <miguel/sdl2/include/SDL.h>
#include <iostream>

namespace small3d {

  /**
   * Constructor
   */
  BoundingBoxes::BoundingBoxes() {
    initLogger();
    vertices.clear();
    facesVertexIndexes = new vector<int *>();
    facesVertexIndexes->clear();
    numBoxes = 0;
  }

  BoundingBoxes::~BoundingBoxes() {

    if (facesVertexIndexes != NULL) {
      for (int i = 0; i != facesVertexIndexes->size(); ++i) {
        delete[] facesVertexIndexes->at(i);
      }
      facesVertexIndexes->clear();
      delete facesVertexIndexes;
      facesVertexIndexes = NULL;
    }
  }

  void BoundingBoxes::loadFromFile(string fileLocation) {
    if (vertices.size() != 0) {
      throw Exception(
          "Illegal attempt to reload bounding boxes. Please use another object.");
    }
    ifstream file((SDL_GetBasePath() + fileLocation).c_str());
    string line;
    if (file.is_open()) {
      while (getline(file, line)) {
        if (line[0] == 'v' || line[0] == 'f') {
          string *tokens = new string[5]; // Max 5 such tokens in the specific kind of
          // Wavefront file

          int numTokens = getTokens(line, ' ', tokens);

          int idx = 0;

          if (line[0] == 'v') {
            // get vertex
            vector<float> v;

            for (int tokenIdx = 0; tokenIdx < numTokens; ++tokenIdx) {
              string t = tokens[tokenIdx];
              if (idx > 0)   // The first token is the vertex indicator
              {
                v.push_back((float) atof(t.c_str()));
              }
              ++idx;
            }
            vertices.push_back(v);
          }
          else {
            // get vertex index
            int *v = new int[4];

            for (int tokenIdx = 0; tokenIdx < numTokens; ++tokenIdx) {
              string t = tokens[tokenIdx];
              if (idx > 0)   // The first token is face indicator
              {
                v[idx - 1] = atoi(t.c_str());
              }
              ++idx;
            }
            facesVertexIndexes->push_back(v);

          }

          if (tokens != NULL) {
            delete[] tokens;
          }
        }
      }
      file.close();
      numBoxes = facesVertexIndexes->size() / 6;
    }
    else
      throw Exception(
          "Could not open file " + string(SDL_GetBasePath())
          + fileLocation);

  }

  bool BoundingBoxes::pointIsWithin(const float &pointX, const float &pointY, const float &pointZ) {
    bool collides = false;
    glm::mat4 rotationMatrix = rotateZ(rotation.z) * rotateX(rotation.x) * rotateY(rotation.y);

    for (int idx = 0; idx < numBoxes; ++idx) {
      float minZ, maxZ, minX, maxX, minY, maxY;

      glm::vec4 coords(vertices[idx * 8][0], vertices[idx * 8][1], vertices[idx * 8][2], 1);
      glm::vec4 rotatedCoords(0.0f, 0.0f, 0.0f, 1.0f);

      rotatedCoords = rotationMatrix * coords;

      rotatedCoords.x += offset.x;
      rotatedCoords.y += offset.y;
      rotatedCoords.z += offset.z;

      minX = rotatedCoords.x;
      maxX = rotatedCoords.x;
      minY = rotatedCoords.y;
      maxY = rotatedCoords.y;
      minZ = rotatedCoords.z;
      maxZ = rotatedCoords.z;

      for (int checkidx = idx * 8; checkidx < (idx + 1) * 8; ++checkidx) {
        coords = glm::vec4(vertices[checkidx][0], vertices[checkidx][1], vertices[checkidx][2], 1);
        rotatedCoords = rotationMatrix * coords;

        rotatedCoords.x += offset.x;
        rotatedCoords.y += offset.y;
        rotatedCoords.z += offset.z;

        if (rotatedCoords.x < minX)
          minX = rotatedCoords.x;
        if (rotatedCoords.x > maxX)
          maxX = rotatedCoords.x;
        if (rotatedCoords.y < minY)
          minY = rotatedCoords.y;
        if (rotatedCoords.y > maxY)
          maxY = rotatedCoords.y;
        if (rotatedCoords.z < minZ)
          minZ = rotatedCoords.z;
        if (rotatedCoords.z > maxZ)
          maxZ = rotatedCoords.z;
      }

      /*cout<<"Checking "<<minX<<" "<<maxX<< " - " << pointX<<", "<<
    minY<<" "<<maxY<<" - "<<pointY << ", " << minZ <<" "<<pointZ<<" - "<<rotatedCoords.z<<endl;*/

      //cout<<"Checking " << minY<<" "<<maxY<<" - "<<pointY <<endl;

      if (pointX > minX && pointX < maxX &&
          pointY > minY && pointY < maxY &&
          pointZ > minZ && pointZ < maxZ) {
        //cout<<"NOT Checking " << minY<<" "<<maxY<<" - "<<pointY <<endl;
        collides = true;
        break;
      }
    }

    return collides;
  }

  bool BoundingBoxes::boxesAreWithin(shared_ptr<BoundingBoxes> otherBoxes) {
    bool collides = false;

    glm::mat4 rotationMatrix =
        rotateZ(otherBoxes->rotation.z) * rotateX(otherBoxes->rotation.x) * rotateY(otherBoxes->rotation.y);

    for (vector<vector<float> >::iterator vertex = otherBoxes->vertices.begin();
         vertex != otherBoxes->vertices.end(); ++vertex) {

      glm::vec4 otherCoords(vertex->at(0), vertex->at(1), vertex->at(2), 1.0f);
      glm::vec4 rotatedOtherCoords(0.0f, 0.0f, 0.0f, 1.0f);

      rotatedOtherCoords = rotationMatrix * otherCoords;

      rotatedOtherCoords.x += otherBoxes->offset.x;
      rotatedOtherCoords.y += otherBoxes->offset.y;
      rotatedOtherCoords.z += otherBoxes->offset.z;

      /*cout << "Checking " << rotatedOtherCoords.x << ", " << rotatedOtherCoords.y << ", " << rotatedOtherCoords.z <<
    " with " << boxesX << ", " << boxesY << ", " << boxesZ << " rotation " << boxesRotation << endl;*/
      if (pointIsWithin(rotatedOtherCoords.x, rotatedOtherCoords.y, rotatedOtherCoords.z)) {

        collides = true;
        break;
      }
    }

    return collides;
  }

}
