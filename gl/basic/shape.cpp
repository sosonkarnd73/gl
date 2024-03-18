#include<iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
 
void init(){ 
  glClearColor(0,0,0,0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0,100,0,200);
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);//_FAN
  glColor3f(1,1,0);
  glVertex2f(0,0.5);
  glVertex2f(-0.4,0);
  glVertex2f(0.4,0);
  glColor3f(0,1,0);
  glVertex2f(0,-0.5);
  glEnd();
  //  glFlush();
  glutSwapBuffers();
} 

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800,800);
  glutCreateWindow("First Rectangle");
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
