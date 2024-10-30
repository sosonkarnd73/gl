#ifndef SURFACE_H
#define SURFACE_H

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Vec3.h"
using namespace std;


//variables for Flat/Smooth, Control Polygon and Wireframe
Vec3 vertexnormal[21][21];
Vec3 facenormal[20][20];
bool flatSmooth = 0, showControlPolygon = 0, showWireframe = 0;

//variables for B-Spline function
Vec3 P[21][21];
Vec3 **ctlPoints;
float *knotU, *knotV;
int numContrU, numContrV, degreeU, degreeV;
float piece = 0.05;
int numSegment = 1 / piece;

//variables for rotate
float xAngle = 0, yAngle = 0;

float deBoor(int i, int p, float u, float *knots);
void bSpline();
Vec3 faceNormal(Vec3 a, Vec3 b,	Vec3 c);
void vertexNormal();
void lightInit();
void renderScene(void);
void changeSize(int ww, int hh);
void processSpecialKeys(int key, int x, int y);

#endif
