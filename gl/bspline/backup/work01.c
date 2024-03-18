/*
  Wawe bsline curve
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#define vbol 1
#define cbol 1

int l[4] = {0,0}, error;
GLuint program;
GLint color_attribute, coord2d_attribute;
GLuint vbo[vbol], cbo[cbol];

int assertion(int clamped, int p, const double* k_v, int k_v_m, int c_p_n);
int* getvert(GLuint* vbo);
double b_spline_basis_fun(int i, int p, const double* knot_vector, double u);
void b_spline_curve(int p, double u, const double* knot_vector, const double* control_points, int n, double* points, const int* j);

void free_resources();
void key_cb (unsigned char key, int x, int y);
void onIdle();
void onTimer();
void onDisplay();
int init_resources();

int main(int argc, char* argv[])
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA);
     glutInitWindowSize(700, 500);
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
	  glutIdleFunc(onIdle);
	  glutTimerFunc(1000.0/1.0, onTimer, 0);

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
	  "in vec2 coord2d;                  " 
	  "in vec3 in_color;                 "
	  "out vec4 out_color;               " 
	  "void main(void)                   "
	  "{                                 "
	  "	gl_Position = vec4(coord2d, 1.0, 1.0);             "
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
          
     char* attribute_name = "coord2d";
     coord2d_attribute = glGetAttribLocation(program, (const char*)attribute_name);
     if (coord2d_attribute == -1 )
     {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
     }
     
     attribute_name = "in_color";
     color_attribute = glGetAttribLocation(program, attribute_name);

     if (color_attribute == -1 )
     {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);  
     }
     //     printf("coord2d_attribute: %d\tcolor_attribute: %d\n",coord2d_attribute,color_attribute);
     if(coord2d_attribute== -1 || color_attribute == -1)
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

     glEnableVertexAttribArray(coord2d_attribute);
     glEnableVertexAttribArray(color_attribute);

     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glVertexAttribPointer(coord2d_attribute, 2, GL_DOUBLE, GL_FALSE, 0, 0);

     glBindBuffer(GL_ARRAY_BUFFER, cbo[0]);
     glVertexAttribPointer(color_attribute, 3, GL_DOUBLE, GL_FALSE, 0, 0);
 
     int i = 0;
     while(i<l[0]/2)
     {
	  glDrawArrays(GL_POINTS, i,1);	  
	  glutSwapBuffers();
	  i++;
     }
     glDisableVertexAttribArray(coord2d_attribute);
     glDisableVertexAttribArray(color_attribute);     

}

void key_cb ( unsigned char key, int x, int y )
{
     switch ( key )
     {
     case 27 :                                 /* Escape key */
	  glutLeaveMainLoop () ;
	  break ;
     }
}

void onIdle()
{
     // glutPostRedisplay();
     // glutTimerFunc(1000.0/60.0, onIdle, 0);
}
void onTimer()
{
     glutPostRedisplay();
     sleep(1);
     glutTimerFunc(1000.0/1.0, onTimer, 0);
}
void free_resources()
{
     glDeleteBuffers(1,vbo);
     glDeleteBuffers(1,cbo);
     glDeleteProgram(program);  
}

int assertion(int clamped, int p, const double* k_v, int m,int n)
{
     int  k = 1, i = 0, j = (m-p-1), f1, f2;
     while(i<m-1 && k_v[i]<=k_v[i+1])
     {	  
	  i++;
     }
     if(i != m-1)
     {
	  error = 2;
     }
     i = 0;
     if(clamped != 0 && clamped!=1)
     {
	  error =2;
     }
     if(clamped==1)
     {
	  while(i<p && k!=0)
	  {
	       f1 = (k_v[i] == k_v[i+1]);
	       f2 = (k_v[j] == k_v[j+1]);
	       k = f1&f2;
	       i++;
	       j++;
	  }
     }
     
     if(n+p+1 != m || k==0 || error==2)
     {
	  fprintf(stderr,"Program is not defined correctly or Input is not consistent\n");
	  error = 2;
     }
     return error;
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
	       num1 = (u - knot_vector[i]);
	       den1 = (knot_vector[i+p] - knot_vector[i]);
	       if(num1!=0 && den1!=0)
		    a = ((b_spline_basis_fun(i,p-1,knot_vector,u))*num1)/den1;
	       
	       num2 = (knot_vector[i+p+1] - u);
	       den2 = (knot_vector[i+p+1] - knot_vector[i+1]);
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
     double x = 0, y = 0, nipu = 0;
     for(int k=0,i=0; i<n; i++,k+=2)
     {
	  nipu = b_spline_basis_fun(i, p, knot_vector, u);
	  x = x + nipu*control_points[k];
	  y =  y + nipu*control_points[k+1];
     }
     points[index] = x;
     points[index+1] = y;
}

int* getvert(GLuint* vbo)
{
     int clamped;
     scanf("%d",&clamped);     
     int k_v_m, c_p_n, p;
     scanf("%d%d",&c_p_n, &p);
     int i=0, j = 0;
     double* c_p = (double*)malloc(sizeof(double)*((c_p_n)*2));        
     while(j<c_p_n)
     {
	  scanf("%lf%lf",&c_p[i],&c_p[i+1]);                    //Control points
	  i = i+2;
	  j = j + 1;
     }
     i = 0; j = 0;
     scanf("%d",&k_v_m);
     double* k_v = (double*)malloc(sizeof(double)*k_v_m);
     while(i<k_v_m)
     {
	  scanf("%lf",&k_v[i]);
	  i++;
     }
     if(assertion(clamped, p, k_v, k_v_m, c_p_n)==2)
     {
	  return 0;
     }

     double u=k_v[p], um =k_v[k_v_m-p-1], e=0.0001f;
     int abc = (int)((um-u)/e);

     l[0] = 2*abc;
     l[1] = 3*abc;
     printf("l[0] %d\t[1] %d\n",l[0],l[1]);
     double* points     = (double*)malloc(sizeof(double)*l[0]);
     double* points_col = (double*)malloc(sizeof(double)*l[1]);
     i = 0, j = 0;

     while(u<um && i+1<l[0])
     {
	  b_spline_curve(p, u, k_v, c_p, c_p_n, points,&i);
	  u = u + e;
	  points_col[j]=1.0; 	  points_col[j+1]=1.0; 	  points_col[j+2]=0.0;  
	  i = i+2;
	  j = j+3;
     }
     glGenBuffers(vbol, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, (sizeof(points)*l[0]), points, GL_STATIC_DRAW);
     
     glGenBuffers(cbol, cbo);
     glBindBuffer(GL_ARRAY_BUFFER, cbo[0]); 
     glBufferData(GL_ARRAY_BUFFER, sizeof(points_col)*l[1], points_col, GL_STATIC_READ);
   
     free(k_v);
     free(c_p);
     free(points);
     free(points_col);
return l;
}
