#include <stdio.h>
#include<unistd.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>

int l=0;
GLuint vbo_LINE_STRIP;
GLuint program;
GLint attribute_coord2d;

void free_resources();
void onDisplay();
int init_resources();

int main(int argc, char* argv[])
{
     glutInit(&argc, argv);
     glutInitContextVersion(2,0);
     glutInitDisplayMode(GLUT_RGBA);//|GLUT_DOUBLE|GLUT_DEPTH);
     glutInitWindowSize(540, 540);
     glutInitWindowPosition(0,0);
  
     glutCreateWindow("My First LINE_STRIP");
     GLenum glew_status = glewInit();

     if (glew_status != GLEW_OK)
     {
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	  return 1;
     }


     if (init_resources())
      {
     	  glutDisplayFunc(onDisplay);
	  glutMainLoop();
      }
  
     free_resources();
     return 0;
}

void onDisplay()
{
     glClearColor(0.0f,0.0f,0.0f,0.0f);
     glClear(GL_COLOR_BUFFER_BIT);

     glUseProgram(program);
   
     glEnableVertexAttribArray(attribute_coord2d);
   
     /* Describe our vertices array to OpenGL (it can't guess its format automatically) */

     glVertexAttribPointer(
	  attribute_coord2d,                 // attribute    
	  2,                 // number of elements per vertex, here (x,y)
	  GL_FLOAT,          // the type of each element
	  GL_FALSE,          // take our values as-is
	  0,                 // no extra data between each position
	  0                  // offset of first element   
	  );

     /* Push each element in buffer_vertices to the vertex shader */
     //   glDrawArrays(GL_LINES, 0,l);
     glDrawArrays(GL_LINE_STRIP, 0,l);
     glDisableVertexAttribArray(attribute_coord2d);                  
     glutSwapBuffers();
}

int get_vertices()
{
     int k=0;
     GLfloat LINE_STRIP_vertices[]={0.0,  0.0, 0.1, 0.1, 0.2, 0.1,  0.3, 0.0};
     l = 4;
     glGenBuffers(1, &vbo_LINE_STRIP);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_LINE_STRIP);
     glBufferData(GL_ARRAY_BUFFER, sizeof(LINE_STRIP_vertices), LINE_STRIP_vertices, GL_STATIC_DRAW);
     k =1;
     return k;
}
int init_resources()
{
     if(!get_vertices())
     {
	  return 0;
     }
     
     GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

     GLuint vs = glCreateShader(GL_VERTEX_SHADER);
     const char *vs_source =
       "#version 460\n"  // OpenGL ES 4.6
       "in vec2 coord2d;                  "
       "void main(void)  {                       "
       "  gl_Position = vec4(coord2d, 0.0, 1.0); "
       "}";
     glShaderSource(vs, 1, &vs_source, NULL);
     glCompileShader(vs);
     glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok)
     {
	  fprintf(stderr, "Error in vertex shader\n");
	  return 0;
     }

     GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
     const char *fs_source =
     	  "#version 460\n"  // OpenGL ES 2.0
     	   "out vec4 olor;"
	   "void main(void) {        "
	   "olor = vec4(1,1,0,1);"
	   "}";     
     glShaderSource(fs, 1, &fs_source, NULL);
     glCompileShader(fs);
     glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok)
     {
	  fprintf(stderr, "Error in fragment shader\n");
	  return 0;
     }
     
     program = glCreateProgram();
     glAttachShader(program, vs);
     glAttachShader(program, fs);
     glLinkProgram(program);
     glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
     if (!link_ok)
     {
	  fprintf(stderr, "glLinkProgram:");
	  return 0;
     }

     const char* attribute_name = "coord2d";
     attribute_coord2d = glGetAttribLocation(program, attribute_name);
     if (attribute_coord2d == -1)
     {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  return 0;
     }
     return 1;
}

void free_resources()
{
     glDeleteProgram(program);
     glDeleteBuffers(1,&vbo_LINE_STRIP);
}
