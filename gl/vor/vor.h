#ifndef __VOR_H__
#define __VOR_H__
#include<stdio.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include<GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include<GL/freeglut.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<set>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
using namespace std;

void free_resources();
void onDisplay();
int init_resources(int m);
int get_vertices(GLuint *vbo, int opp);

class point{
public:
     float x;
     float y;
     point();
     point(float x, float y);
     point(const point *p);
     bool operator==(point p);
     void print();
     static bool compare_x(point p1, point p2);
     static bool compare_y(point p1, point p2);
};

class line{
public:
     point mid;
     float slope;
     float intercept;
     int flag;                // flag = 0 -> line is perpendicular to x axis and x-intercept will used as intercept
     line(point a, point b);
     void print();
     int sign_delta_y(point p);
     point* intersect(const line *l);  
};

class region{
public:
     point p;
     vector<point> vertices;
     vector<line> v;
     vector<int> b;
     region();
     region(vector<point> vect, int i);
     int in_same_region(point p1);
     void print();
};
#endif
