#include <stdio.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define length 2000
#define e_log_size 1024
void free_resources();
void get_vertices();     
void onDisplay();
int init_resources();
void onIdle();
void onTimer();
void key_cb (unsigned char key, int x, int y);


GLuint program, vbo[2];
GLint attribute_coord2d;

int e_log_size_0;
char e_log[e_log_size];
float vertices1[2*length];

int main(int argc, char* argv[]) {
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA);
     glutInitWindowSize(600, 600);
     glutInitWindowPosition(600,50);
  
     glutCreateWindow("Points");
     GLenum glew_status = glewInit();

     if (glew_status != GLEW_OK) {
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	  return 1;
     }
  
     if (init_resources()) {
	  glutKeyboardFunc (key_cb);
	  glutDisplayFunc(onDisplay);
	  //glutIdleFunc(onIdle);
	  glutTimerFunc(1000.0/4.0, onTimer, 0);
	  glutMainLoop();
     }
     free_resources();
     return 0;
}

void onTimer(){
     int i = 0;
     int static cord = 1;
     if(cord==1){
	  while(i < length){
	       vertices1[i] = -1*vertices1[i];
	       i+=2;
	  }
	  cord = 2;
     }
     else if(cord == 2){
	  while(i < length){
	       vertices1[i+1] = -1*vertices1[i+1];
	       i+=2;
	  }
	  	  cord = 3;
     }
     else if(cord == 3){
	  while(i < length){
	       vertices1[i] = -1*vertices1[i];
	       i+=2;
	  }
	  cord = 4;
     }
     else if(cord == 4){
	  while(i < length){
	       vertices1[i+1] = -1*vertices1[i+1];
	       i+=2;
	  }
	  	  cord = 1;
     }
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
     glutPostRedisplay();
     glutTimerFunc(1000.0/4.0, onTimer, 0);
}


void onDisplay()
{
     glClearColor(0.0f,0.0f,0.0f,0.0f);
     glClear(GL_COLOR_BUFFER_BIT);     
     glUseProgram(program);
     glEnableVertexAttribArray(0);     
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
     glDrawArrays(GL_POINTS, 0,length); //length      
     glDisableVertexAttribArray(0);
     glutSwapBuffers();
}

void get_vertices(){    
     int i = 0;
     while(i<length){
	  vertices1[i]=(float)(random()%1000000)/1000000.0;
	  vertices1[i+1]=(float)(random()%1000000)/1000000.0;  
	  i+=2;
     }
     glGenBuffers(1, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);
}

void key_cb ( unsigned char key, int x, int y )
{
     switch ( key )
     {
     case 27 :  /* Escape key */
	  glutLeaveMainLoop () ;
	  break ;
     }
}


int init_resources()
{
     glEnable(GL_PROGRAM_POINT_SIZE);
     if(!glIsEnabled(GL_PROGRAM_POINT_SIZE)){
	  return 0;
     }
     get_vertices();
     GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

     GLuint vs = glCreateShader(GL_VERTEX_SHADER);
     const char *vs_source =
	  "#version 460\n"  // OpenGL ES 4.6
	  "in vec2 coord2d;                  "
	  "void main(void) {                        "
	  "  gl_Position = vec4(coord2d, 0.0, 1.0); "
	  "  gl_PointSize = 2.0f;                   "
	  "}";
     glShaderSource(vs, 1, &vs_source, NULL);
     glCompileShader(vs);
     glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok) {
 	  e_log[0]='\0';
	  glGetShaderInfoLog(vs, e_log_size, &e_log_size_0,e_log);
	  fprintf(stderr, "Error in vertex shader\nlog : %s\n",e_log);
	  return 0;
     }

     GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
     const char *fs_source =
	  "#version 460\n           "  // OpenGL ES 4.6
	  "out vec4 olor;           "
	  "void main(void) {        "
	  "  olor= vec4(1.0,1.0,0.0, 1); "
	  "}";
     glShaderSource(fs, 1, &fs_source, NULL);
     glCompileShader(fs);
     glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok) {
 	  e_log[0]='\0';
	  glGetShaderInfoLog(fs, e_log_size, &e_log_size_0,e_log);
	  fprintf(stderr, "Error in fragment shader\nlog : %s\n",e_log);
	  return 0;
     }

     program = glCreateProgram();
     glAttachShader(program, vs);
     glAttachShader(program, fs);
     glLinkProgram(program);
     glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
     if (!link_ok) {
	  e_log[0]='\0';
	  glGetProgramInfoLog(program, e_log_size, &e_log_size_0,e_log);
	  fprintf(stderr, "Error in Program Linking \nlog : %s\n",e_log); 
	  return 0;
     }

     const char* attribute_name = "coord2d";
     attribute_coord2d = glGetAttribLocation(program, attribute_name);
     if (attribute_coord2d == -1) {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  return 0;
     }
     return 1;
}

void free_resources()
{
     glDeleteProgram(program);  
}
