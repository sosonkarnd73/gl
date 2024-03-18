#include "vor.h"
int l = -1;
GLuint vbo[1];
GLuint program;
GLint attribute_coord2d;

int main(int argc, char* argv[]) {
     int m=19;
     if(argc==2){
	  m = atoi(argv[1]);
     }
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA);
     glutInitWindowSize(800, 800);
     glutInitWindowPosition(50,300);
  
     glutCreateWindow("My First Regioon");
     GLenum glew_status = glewInit();

     if (glew_status != GLEW_OK){
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	  return 1;
     }
  
     if (init_resources(m)) {
	  glutDisplayFunc(onDisplay);
	  glutMainLoop();
     }
  
     free_resources();
     return 0;
}

void onDisplay()
{
     glClearColor(1.0f,1.0f,1.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT);
     glUseProgram(program);
   
     glEnableVertexAttribArray(attribute_coord2d);

     /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
     glVertexAttribPointer(
	  attribute_coord2d,                 // 
	  2,                 // number of elements per vertex, here (x,y)
	  GL_FLOAT,          // the type of each element
	  GL_FALSE,          // take our values as-is
	  0,                 // no extra data between each position
	  0  // pointer to the C array   
	  );

     /* Push each element in buffer_vertices to the vertex shader */
     glDrawArrays(GL_POINTS, 0,l);
     glDisableVertexAttribArray(attribute_coord2d);                  
     glutSwapBuffers();
}

int init_resources(int m)
{
     glEnable(GL_PROGRAM_POINT_SIZE);
     if(!glIsEnabled(GL_PROGRAM_POINT_SIZE)){
	  return 0;
     }

     l = get_vertices(vbo, m);
     if(l < 0){
	  return 0;
     }
     
     GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

     GLuint vs = glCreateShader(GL_VERTEX_SHADER);
     const char *vs_source =
	  "#version 460\n"  // OpenGL ES 4.6
	  "in vec2 coord2d;                  "
	  "void main(void) {                        "
	  "  gl_Position = vec4(coord2d, 1.0, 1.0); "
	  "  gl_PointSize = 3.0f;                   "	  
	  "}";
     glShaderSource(vs, 1, &vs_source, NULL);
     glCompileShader(vs);
     glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok) {
	  fprintf(stderr, "Error in vertex shader\n");
	  return 0;
     }

     GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
     const char *fs_source =
	  "#version 460\n"  // OpenGL ES 4.6
	  "out vec4 olor;"
	  "void main(void) {        "
	  "   olor= vec4(0.7,0.4,0.2,1.0); "
	  "}";
     glShaderSource(fs, 1, &fs_source, NULL);
     glCompileShader(fs);
     glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
     if (!compile_ok) {
	  fprintf(stderr, "Error in fragment shader\n");
	  return 0;
     }

     program = glCreateProgram();
     glAttachShader(program, vs);
     glAttachShader(program, fs);
     glLinkProgram(program);
     glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
     if (!link_ok) {
	  fprintf(stderr, "glLinkProgram:");
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
