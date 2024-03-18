#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<GL/glew.h>
#include <GL/glut.h>
#include<GL/freeglut.h>
#define vbol 2
#define cbol 2

int l[4] = {0,0,0,0}, error;
float angle = 0;
GLuint program;
GLint color_attribute, coord3d_attribute;
GLuint vbo[vbol], cbo[cbol];

int* getvert(GLuint* vbo);
double b_spline_basis_fun(int i, int p, const double* knot_vector, double u);
void b_spline_curve(int p, double u, const double* knot_vector, const double* control_points, int n, double* points, const int* j);

void free_resources();
void key_cb (unsigned char key, int x, int y);
void onIdle();
void onTimer();
void onDisplay();
void reshape(int x, int y);
int init_resources();

int main(int argc, char* argv[])
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA);
     glutInitWindowSize(400, 400);
     glutInitWindowPosition(0, 0);
  
     glutCreateWindow("BSpline Curve");
     GLenum glew_status = glewInit();
     if (glew_status != GLEW_OK)
     {
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	  return 1;
     }

     int* ll = getvert(vbo);
     if(ll == NULL || error == 2)
     {
	  return 0;
     }
     
     if (init_resources())
     {
	  glutKeyboardFunc(key_cb);
	  glutDisplayFunc(onDisplay);
	  //  glutIdleFunc(onIdle);
	  //  glutTimerFunc(1000.0/60.0, onTimer, 0);
	  glutReshapeFunc(reshape);
	  glEnable(GL_DEPTH_TEST);
	  glutMainLoop();
     }
     free_resources();
     return 0;
}

int init_resources()
{
     glEnable(GL_PROGRAM_POINT_SIZE);
     if(!glIsEnabled(GL_PROGRAM_POINT_SIZE))
     {
	  return 0;
     }

     GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

     GLuint vs = glCreateShader(GL_VERTEX_SHADER);
     const char *vs_source =
	  "#version 460\n                    "  // OpenGL ES 4.6
	  "in vec3 coord3d;                  " 
	  "in vec3 in_color;                 "
	  "out vec4 out_color;               " 
	  "void main(void)                   "
	  "{                                 "
	  "	gl_Position = vec4(coord3d, 1.0);             "
	  "	gl_PointSize = 1.0f;                               "  
	  "  	out_color=vec4(in_color,1.0);                      "
	  "}                                                       ";
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
	  "#version 460\n"                   // OpenGL ES 4.6
	  "out vec4 olor;                                      "
	  "in vec4 out_color;                                  " 
	  "void main(void)                                     "
	  "{                                                   "
	  "     olor=out_color;                                "
	  "}                                                   ";
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
          
     char* attribute_name = "coord3d";
     coord3d_attribute = glGetAttribLocation(program, (const char*)attribute_name);
     if (coord3d_attribute == -1 )
     {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
     }
        
     attribute_name = "in_color";
     color_attribute = glGetAttribLocation(program, attribute_name);

     if (color_attribute == -1 )
     {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);  
     }
 
     if(coord3d_attribute== -1 || color_attribute == -1)
     {
	  return 0;
     }
     return 1;
}

void onDisplay()
{
  
     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
     glClear(GL_COLOR_BUFFER_BIT);

     glUseProgram(program);
     //begin
     glMatrixMode(GL_MODELVIEW); /* the following will affect the modelview matrix */
     glLoadIdentity();        /* reset the transformation matrix to identity */
     glTranslatef(0, 0, -5);  /* translate 5 units towards the -z axis */
     glRotatef(angle, 0, 1, 0);  /* rotate by "angle" around the y axis */
     //end
     glEnableVertexAttribArray(coord3d_attribute);
     glEnableVertexAttribArray(color_attribute);
     //Drawing the convex hull is the control polygon
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glVertexAttribPointer(coord3d_attribute, 3, GL_DOUBLE, GL_FALSE, 0, 0);     
      
     glBindBuffer(GL_ARRAY_BUFFER, cbo[0]); 
     glVertexAttribPointer(color_attribute, 3, GL_DOUBLE, GL_FALSE, 0, 0);
      
     glDrawArrays(GL_QUADS, 0,l[0]/3); 
     
     //Drawing the bspline curve
     glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
     glVertexAttribPointer(coord3d_attribute, 3, GL_DOUBLE, GL_FALSE, 0, 0);

     glBindBuffer(GL_ARRAY_BUFFER, cbo[1]);
     glVertexAttribPointer(color_attribute, 3, GL_DOUBLE, GL_FALSE, 0, 0);
     glDrawArrays(GL_POINTS, 0,l[1]/3);

     glutSwapBuffers();

     glDisableVertexAttribArray(coord3d_attribute);
     glDisableVertexAttribArray(color_attribute);     
}
/* the reshape callback is called when our window is resized
 * (X11 ConfigureNotify event)
 */
void reshape(int x, int y)
{
     glViewport(0, 0, x, y);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(45.0, (float)x / (float)y, 1.0, 1000.0);
}
void key_cb ( unsigned char key, int x, int y )
{
     switch ( key )
     {
     case 27 :                      /* Escape key */
	  glutLeaveMainLoop () ;
	  break ;
     
     case ',':
	  angle -= 45;
	  glutPostRedisplay();
	  break;
       
     case '.':
	  angle += 90;
	  glutPostRedisplay();
	  break;
     }
}

void onIdle()
{
     /*can be used for getting data about window*/
     /*when g[u is idle*/
     /**/
}

void onTimer()
{
     /*can Used to create animation*/
     /* glutPostRedisplay(); */
     /* glutTimerFunc(1000.0/60.0, onTimer, 0); */
}
void free_resources()
{
     glDeleteBuffers(2,vbo);
     glDeleteBuffers(2,cbo);
     glDeleteProgram(program);  
}

double b_spline_basis_fun(int i, int p, const double* knot_vector, double u)
{
     double nipu=0,num1=0,num2=0,den1=0,den2=0,a=0,b=0;
     if(knot_vector[i]<=u && u<knot_vector[i+p+1])
     {
	  if(p == 0)
	  {
	       nipu = 1;
	  }
	  else
	  {
	       num1 = (u-knot_vector[i]);
	       den1 = (knot_vector[i+p]-knot_vector[i]);
	       if(num1!=0 && den1!=0)
		    a = ((b_spline_basis_fun(i,p-1,knot_vector,u))*num1)/den1;
	       	  
	       num2 = (knot_vector[i+p+1]-u);
	       den2 = (knot_vector[i+p+1]-knot_vector[i+1]);
	       if(num2!=0 && den2!=0)
		    b = ((b_spline_basis_fun(i+1,p-1,knot_vector,u))*num2)/den2;
	       
	       nipu = a + b;
	  }
     }
     return nipu;
}

void b_spline_curve(int p, double u, const double* knot_vector,const double* control_points, int n, double* points, const int* j)
{
     int index = *j;
     double x = 0, y = 0, z  = 0, nipu = 0;
     for(int k=0,i=0; i<n; i++,k+=2)
     {
	  nipu = b_spline_basis_fun(i, p, knot_vector, u);
	  x = x + nipu*control_points[k];
	  y =  y + nipu*control_points[k+1]; 
	  z = z + nipu*control_points[k+2];
     }
     points[index] = x;
     points[index+1] = y;
     points[index+2] = z;     
}


int* getvert(GLuint* vbo)
{
     /*
       type = 0 open
       type = 1 clamped
       type = 2 closed
     */
     /* 
	p : degree of bspline curve
	n : number of control points
	m : number of control points
	N : n-1
	M : M-1
     */
     int curve_type;
     scanf("%d",&curve_type);
     
     int k_v_m, c_p_n, p;
     scanf("%d%d",&p,&c_p_n);
     int i=0, k = 1, j = 0;
     l[0] = (c_p_n)*3;							//length of array of control points
     l[2] = (c_p_n)*3;
     double* c_p = (double*)malloc(sizeof(double)*l[0]);
     double* c_p_col = (double*)malloc(sizeof(double)*l[2]);
     
     while(i<l[0])
     {
	  scanf("%lf%lf%lf",&c_p[i],&c_p[i+1],&c_p[i+2]);		//Control points
	  c_p_col[j]=0.5; c_p_col[j+1]=0.5; c_p_col[j+2]=0.8;		//color for control points.
	  j = j+3;
	  i = i+3;
     }

     scanf("%d",&k_v_m);
     
     double* k_v = (double*)malloc(sizeof(double)*k_v_m);
     i = 0;
     while(i<k_v_m)
     {
	  scanf("%lf",&k_v[i]);
	  i++;
     }
     
     double e = 0.0001f, u = k_v[p], um = k_v[k_v_m-p-1];  // Range of BSpline Curve
  
     int abc = (int)((um-u)/e);
     l[1] = 3*abc;               
     l[3] = 3*abc;
     double* points     = (double*)malloc(sizeof(double)*l[1]);
     double* points_col = (double*)malloc(sizeof(double)*l[3]);
     i = 0, j = 0;
     while(u<um && i+1<l[1])
     {
	  b_spline_curve(p, u, k_v, c_p, c_p_n, points,&i);
	  u = u + e;
	  points_col[j]=1.0; 	  points_col[j+1]=1.0; 	  points_col[j+2]=0.0;  
	  i = i+3;
	  j = j+3;
     }
  
     glGenBuffers(vbol, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, (sizeof(c_p)*l[0]), c_p, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
     glBufferData(GL_ARRAY_BUFFER, (sizeof(points)*l[1]), points, GL_STATIC_DRAW);
     
     glGenBuffers(cbol, cbo); 
     glBindBuffer(GL_ARRAY_BUFFER, cbo[0]); 
     glBufferData(GL_ARRAY_BUFFER, (sizeof(c_p_col)*l[2]), c_p_col, GL_STATIC_READ);
     glBindBuffer(GL_ARRAY_BUFFER, cbo[1]);  
     glBufferData(GL_ARRAY_BUFFER, sizeof(points_col)*l[3], points_col, GL_STATIC_READ);
     
     free(k_v);
     free(c_p);
     free(c_p_col);
     free(points);
     free(points_col);
     return l;
}
