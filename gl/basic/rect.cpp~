#include<iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

void setup(){
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);
  glRectf(-0.75f,0.75f, 0.75f, -0.75f);
  // glutSwapBuffers();
glColor3f(1.0f, 0.0f, 1.0f);

/* Draws four points */
  glBegin(GL_POINTS);
  glVertex2f(0.55f, 0.5f); 
  glVertex2f(0.55f, -0.5f); 
  glVertex2f(-0.55f, 0.5f); 
  glVertex2f(-0.55f, -0.5f); 
glEnd();

 /* Draws two horizontal lines */
 glColor3f(1.0f, 0.9f, 0.1f);
 glBegin(GL_LINES);
  glVertex2f(0.5f, 0.5f); 
  glVertex2f(-0.5f, 0.5f); 
  glVertex2f(-0.5f, -0.5f); 
  glVertex2f(0.5f, -0.5f); 
glEnd();


/* Draws a square */ 
 glColor3f(0.0f, 1.0f, 1.0f);
 glBegin(GL_LINE_LOOP);
  glVertex2f(0.6f, 0.6f); 
  glVertex2f(-0.6f, 0.6f); 
  glVertex2f(-0.6f, -0.6f); 
  glVertex2f(0.6f, -0.6f); 
glEnd();

/* Draws a 'C' */
 glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  glVertex2f(0.4f, 0.4f); 
  glVertex2f(-0.4f, 0.4f); 
  glVertex2f(-0.4f, -0.4f); 
  glVertex2f(0.4f, -0.4f); 
glEnd();
   glutSwapBuffers();

}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800,800);
  glutCreateWindow("First Rectangle");
  setup();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
