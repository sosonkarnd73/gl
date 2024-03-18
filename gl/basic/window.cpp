#include <stdio.h>
#include<unistd.h>
#include <GL/freeglut.h>


#define PROGRAM "window"
static void RenderSceneCB(){
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  char *version = NULL;
  char *vendor = NULL;
  char *renderer = NULL;
  char *extensions = NULL;
  GLuint idWindow = 0;
  int	glutVersion;

  glutInit(&argc, argv);
  glutInitWindowSize(1000,600);
  glutInitDisplayMode(GLUT_RGBA);
  idWindow = glutCreateWindow(PROGRAM);
  glutVersion = glutGet(0x01FC);
  version =     (char*)glGetString(GL_VERSION);
  vendor =      (char*)glGetString(GL_VENDOR);
  renderer =    (char*)glGetString(GL_RENDERER);
  extensions =  (char*)glGetString(GL_EXTENSIONS);

  printf("GLUT=%d\nVERSION=%s\nVENDOR=%s\nRENDERER=%s\nEXTENSIONS=%s\n",glutVersion,version,vendor,renderer,extensions);
  sleep(1);
  glClearColor(1.0f,1.0f,1.0f,0.0f);
  glutDisplayFunc(RenderSceneCB);
  glutMainLoop();
  glutHideWindow();
   
  glutDestroyWindow(idWindow);
  return(0);
}
