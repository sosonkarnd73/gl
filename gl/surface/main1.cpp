#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

const float piece = 0.05;
const int numSegment = 1 / piece;

//variables for B-Spline function
glm::vec3 P[numSegment+1][numSegment+1];


//variables for Flat/Smooth, Control Polygon and Wireframe
glm::vec3 vertexnormal[numSegment+1][numSegment+1];
glm::vec3 facenormal[numSegment][numSegment];
bool flatSmooth = 0, showControlPolygon = 0, showWireframe = 0;

glm::vec3 **ctlPoints;
float *knotU, *knotV;
int numContrU, numContrV, degreeU, degreeV;

//Flag to allow euler angle rotations

int euler_rot = 1;

//variables for rotate
float xAngle = 0, yAngle = 0, zAngle = 0;
glm::vec3 rotation_axis;
float qAngle = 0, theta = 0;

float deBoor(int i, int p, float u, float *knots){
     float equationR, equationL;//right coefficient and left coefficient
     if (p == 0){
	  if (u >= knots[i] && (u < knots[i + 1]))
	       return 1;
	  else return 0;
     }
     else{
	  if ((knots[i + p] - knots[i]) == 0) equationR = 0; //Prevent divide by zero
	  else equationR = ((u - knots[i]) / (knots[i + p] - knots[i]));
	  if ((knots[i + p + 1] - knots[i + 1]) == 0) equationL = 0; // Prevent divide by zero
	  else equationL = ((knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1]));
	  return equationR * deBoor(i, p - 1, u, knots) + equationL * deBoor(i + 1, p - 1, u, knots);
     }
}

void bSpline(){

     for (int u = 0; u < numSegment; u++) //for each point on the surface, 20*20 points for this surface
	  for (int v = 0; v < numSegment; v++){ 
	       for (int i = 0; i <= numContrU; i++)//for each control points
		    for (int j = 0; j <= numContrV; j++){
			 P[u][v] = P[u][v] + ctlPoints[i][j] * float(deBoor(i, degreeU, u*piece, knotU) * deBoor(j, degreeV, v*piece, knotV));
		    }
	  }
}

glm::vec3 faceNormal(glm::vec3 a, glm::vec3 b,	glm::vec3 c){
     glm::vec3 ab = a - b;
     glm::vec3 ac = a - c;
     glm::vec3 cp = glm::cross(ab, ac);
     return glm::normalize(cp);
}


void vertexNormal(){
     for (int i = 0; i < numSegment-1; i++)
	  for (int j = 0; j < numSegment-1; j++){
	       facenormal[i][j] = faceNormal(P[i][j + 1], P[i + 1][j], P[i][j]);
	  }
     //four corners
     vertexnormal[0][0] = facenormal[0][0];
     vertexnormal[numSegment][numSegment] = facenormal[numSegment - 1][numSegment - 1];
     vertexnormal[0][numSegment] = facenormal[0][numSegment - 1];
     vertexnormal[numSegment][0] = facenormal[numSegment - 1][0];
     //four edge
     for (int i = 1; i < numSegment; i++){
	  vertexnormal[i][0] = (facenormal[i - 1][0] + facenormal[i][0]) * 0.5f;
	  vertexnormal[0][i] = (facenormal[0][i-1] + facenormal[0][i]) * 0.5f;
	  vertexnormal[i][numSegment-1] = (facenormal[i - 1][numSegment-2] + facenormal[i][numSegment-2]) * 0.5f;
	  vertexnormal[numSegment-1][i] = (facenormal[numSegment-2][i-1] + facenormal[numSegment-2][i]) * 0.5f;
     }
     //others
     for (int i = 1; i < numSegment-1; i++)
	  for (int j = 1; j < numSegment-1; j++){
	       vertexnormal[i][j] = ((facenormal[i - 1][j - 1] + facenormal[i - 1][j]) + (facenormal[i][j - 1] + facenormal[i][j])) * 0.25f;
	  }
}

void lightInit(){
     GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
     GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat mat_shininess[] = { 100.0 };

     glClearColor(0.0, 0.0, 0.0, 0.0);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_AUTO_NORMAL);
     glEnable(GL_NORMALIZE);

     GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };     //Color(0.2, 0.2, 0.2)
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);


     GLfloat lightColor0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Color (1.0f, 1.0f, 1.0f)
     GLfloat lightPos0[] = { 1.0f, 20.0f, 1.0f, 1.0f }; //Positioned at (-10.0f, 50.0f, 1.0f)
     glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
     glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
     glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0); //for two sides lighting
}

void renderScene(void) {    
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity(); 		// Reset (Modelview) transformations

     // Set the camera: Note! gluLookAt is called in Modelview mode.
     // Reset transformations
     glMatrixMode(GL_MODELVIEW);

     glLoadIdentity();         //Reset the drawing perspective
     glTranslatef(0.0f, -10.0f, -70.0f); 	    //Move forward  (for all objects)
     // Set the camera
     gluLookAt(0.0f, 0.0f, 80.0f,
	       0.0f, 0.0f, 0.0f,
	       0.0f, 1.0f, 0.0f);
     
     // draw something
     glPushMatrix(); //Save the transformations performed thus far
     if(euler_rot == 1){
	  //change the visualization view
	  glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	  glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	  glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
     }
     else
     {
	  glm::fquat qaxis = glm::angleAxis(glm::radians(qAngle), glm::normalize(rotation_axis));
	  glm::mat4 rotationmatrix = glm::toMat4(qaxis);
	  glMultMatrixf(glm::value_ptr(rotationmatrix));
     }
     
     Glcolor3f(0.9f, 0.9f, 0.9f);
     //glTranslatef(0.0f, -50.0f, 0.0f);

     if(!euler_rot)
     {
	  glDisable(GL_LIGHTING);
	  glColor3f(1.0, 1.0, 1.0);
	  glBegin(GL_LINES);
	  glVertex3fv(glm::value_ptr(glm::vec3((80.0f)*rotation_axis)));
	  glVertex3fv(glm::value_ptr(glm::vec3((-80.0f)*rotation_axis)));
	  glEnd();
	  glEnable(GL_LIGHTING);
     }
     if (flatSmooth){/******************* Smooth shading **********************/
	  glShadeModel(GL_SMOOTH);
	  glBegin(GL_TRIANGLES);
	  int k = 0;
	  for (int i = 0; i < numSegment-1; i++){
	       for (int j = 0; j < numSegment-1; j++){
		    
		    glNormal3f(vertexnormal[i][j + 1].x, vertexnormal[i][j + 1].y, vertexnormal[i][j + 1].z);
		    glVertex3f(P[i][j + 1].x, P[i][j + 1].y, P[i][j + 1].z);
			  
		    glNormal3f(vertexnormal[i + 1][j].x, vertexnormal[i + 1][j].y, vertexnormal[i + 1][j].z);
		    glVertex3f(P[i + 1][j].x, P[i + 1][j].y, P[i + 1][j].z);

		    glNormal3f(vertexnormal[i][j].x, vertexnormal[i][j].y, vertexnormal[i][j].z);
		    glVertex3f(P[i][j].x, P[i][j].y, P[i][j].z);

		    glNormal3f(vertexnormal[i + 1][j].x, vertexnormal[i + 1][j].y, vertexnormal[i + 1][j].z);
		    glVertex3f(P[i + 1][j].x, P[i + 1][j].y, P[i + 1][j].z);
		    glNormal3f(vertexnormal[i][j + 1].x, vertexnormal[i][j + 1].y, vertexnormal[i][j + 1].z);
		    glVertex3f(P[i][j + 1].x, P[i][j + 1].y, P[i][j + 1].z);
		    glNormal3f(vertexnormal[i + 1][j + 1].x, vertexnormal[i + 1][j + 1].y, vertexnormal[i + 1][j + 1].z);
		    glVertex3f(P[i + 1][j + 1].x, P[i + 1][j + 1].y, P[i + 1][j + 1].z);
	       }
	  }
	  glEnd();
     }
     else {//**************flat shading****************
	  glm::vec3 facenormal1, facenormal2;
	  glShadeModel(GL_FLAT);
	  glBegin(GL_TRIANGLES);
	  for (int i = 0; i < numSegment-1; i++){
	       for (int j = 0; j < numSegment-1; j++){
		    facenormal1 = faceNormal(P[i][j + 1], P[i + 1][j], P[i][j]);
		    facenormal2 = faceNormal(P[i + 1][j + 1], P[i + 1][j], P[i][j + 1]);
		
		    glNormal3f(facenormal1.x, facenormal1.y, facenormal1.z);
		    glVertex3f(P[i][j + 1].x, P[i][j + 1].y, P[i][j + 1].z);
		    glVertex3f(P[i + 1][j].x, P[i + 1][j].y, P[i + 1][j].z);
		    glVertex3f(P[i][j].x, P[i][j].y, P[i][j].z);
	
		    glNormal3f(facenormal2.x, facenormal2.y, facenormal2.z);
		    glVertex3f(P[i + 1][j].x, P[i + 1][j].y, P[i + 1][j].z);
		    glVertex3f(P[i][j + 1].x, P[i][j + 1].y, P[i][j + 1].z);
		    glVertex3f(P[i+1][j+1].x, P[i+1][j+1].y, P[i+1][j+1].z);
	       }
	  }
	  glEnd();
     }
	
     if (showControlPolygon) {
	  glDisable(GL_LIGHTING);
	  glColor3f(1.0, 1.0, 0.0);
	  for (int i = 0; i <= numContrU; i++) {
	       glBegin(GL_LINE_STRIP);
	       for (int j = 0; j <= numContrV; j++) {
		    glVertex3f(ctlPoints[i][j].x, ctlPoints[i][j].y, ctlPoints[i][j].z);
	       }
	       glEnd();
	  }
	  for (int i = 0; i <= numContrV; i++) {
	       glBegin(GL_LINE_STRIP);
	       for (int j = 0; j <= numContrU; j++) {
		    glVertex3f(ctlPoints[j][i].x, ctlPoints[j][i].y, ctlPoints[j][i].z);
	       }
	       glEnd();
	  }
	  glEnable(GL_LIGHTING);
     }

     if (showWireframe) {
	  glDisable(GL_LIGHTING);
	  glColor3f(1.0, 1.0, 1.0);
	  for (int i = 0; i < numSegment; i++) {
	       glBegin(GL_LINE_STRIP);
	       for (int j = 0; j < numSegment; j++) {
		    glVertex3f(P[i][j].x, P[i][j].y, P[i][j].z);
	       }
	       glEnd();
	       glBegin(GL_LINE_STRIP);
	       for (int j = 0; j < numSegment; j++) {
		    glVertex3f(P[j][i].x, P[j][i].y, P[j][i].z);
	       }
	       glEnd();
	  }
	  glEnable(GL_LIGHTING);
     }
     glPopMatrix(); //Undo the move	
     glutSwapBuffers();
}

void changeSize(int ww, int hh) {
     int h = hh;
     int w = ww;
     if (h == 0) h = 1; 		// Prevent divide by zero 
     float ratio = (float)w / h;

     glMatrixMode(GL_PROJECTION); 	// Use the Projection Matrix
     glLoadIdentity(); 		// Reset Matrix
     glViewport(0, 0, w, h); 		// Set the viewport to be the entire window

     gluPerspective(45, ratio, 1, 200);	// Set the correct perspective

     glMatrixMode(GL_MODELVIEW);	// Get Back to the Modelview
     glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{
     switch (key) {
     case 27 :
	  glutLeaveMainLoop();
	  break;
     }
}
void processSpecialKeys(int key, int x, int y)
{
     switch (key) {
	  // Reset to default.
     case  GLUT_KEY_END:
	  euler_rot  = 0;
	  flatSmooth = 0;
	  showControlPolygon = 0;
	  showWireframe = 0;
	  xAngle = 0; yAngle = 0; zAngle = 0, qAngle = 0;
	  cout<<"Axis of rotation : ";
	  cin>>rotation_axis.x>>rotation_axis.y>>rotation_axis.z>>theta;
	  rotation_axis = glm::normalize(rotation_axis);
	  break;
     case GLUT_KEY_F1:
	  euler_rot = !euler_rot;
	  break;
     case GLUT_KEY_F2:
	  flatSmooth = !flatSmooth;
	  break;
     case GLUT_KEY_F3:
	  showControlPolygon = !showControlPolygon;
	  break;
     case GLUT_KEY_F4:
	  showWireframe = !showWireframe;
	  glutPostRedisplay();
	  break;
	  //change the visualization view	  
     case GLUT_KEY_F5:
	  yAngle += 5;
	  if (yAngle == 360) yAngle = 0;
	  break;
     case GLUT_KEY_F6:
	  yAngle -= 5;
	  if (yAngle == -360) yAngle = 0;
	  break;
     case GLUT_KEY_F7:
	  xAngle += 5;
	  if (xAngle == 360) xAngle = 0;
	  break;
     case GLUT_KEY_F8:
	  xAngle -= 5;
	  if (xAngle == -360) xAngle = 0;
	  break;
     case GLUT_KEY_F9:
	  zAngle += 5;
	  if (zAngle == 360) zAngle = 0;
	  break;
     case GLUT_KEY_F10:
	  zAngle -= 5;
	  if (zAngle == -360) zAngle = 0;
	  break;
     case GLUT_KEY_F11:
	  qAngle += theta;
	  if (qAngle == 360) qAngle = 0;
	  break;
     case GLUT_KEY_F12:
	  qAngle -= theta;
	  if (qAngle == -360) qAngle = 0;
	  break;       
     }
     glutPostRedisplay();
}

void createObject(char* objfile)
{
     fstream  outputfile(objfile);
     outputfile<<"id "<<endl;
     outputfile<<"c "<<endl;
     outputfile<<"d "<<endl;
     outputfile<<"p "<<endl;
     
     for (int i = 0; i < numSegment-1; i++){
	  for (int j = 0; j < numSegment-1; j++){
	       outputfile<<"v "<<P[i][j].x<<" "<<P[i][j].y<<" "<<P[i][j].z<<endl;
	       outputfile<<"n "<<vertexnormal[i][j].x <<" "<< vertexnormal[i][j].y <<" "<< vertexnormal[i][j].z<<endl;
	  }
     }
     for (int i = 0; i < numSegment-1; i++){
	  for (int j = 0; j < numSegment-1; j++){


	       outputfile<<"e "<<(i*(numSegment-1) + (j + 1))<<" "<<((i+1)*(numSegment-1) + (j))<<" "<<(i*(numSegment-1) + (j))<<endl;
	 
	       outputfile<<"e "<<((i+1)*(numSegment-1) + (j))<<" "<<((i)*(numSegment-1) + (j+1))<<" "<<((i+1)*(numSegment-1) + (j+1))<<endl;
	  }
     }
     outputfile.close();
}

int main(int argc, char **argv) {
     if(argc != 3)
     {
	  cout<<"Usage : ./"<<argv[0]<<" bspline_input obj_output"<<endl; 
	  return 1;
     }
     fstream inputfile(argv[1]);
     inputfile >> numContrU >> numContrV;
     inputfile >> degreeU >> degreeV;

     knotU = new float[numContrU + degreeU + 1];
     knotV = new float[numContrV + degreeV + 1];

     for (int i = 0; i < numContrU + degreeU + 1; i++)
     {
	  inputfile >> knotU[i];
     }
     for (int i = 0; i < numContrV + degreeV + 1; i++)
     {
	  inputfile >> knotV[i];
     }

	
     ctlPoints = new glm::vec3*[numContrU];
     for (int i = 0; i < numContrU; i++){
	  ctlPoints[i] = new glm::vec3[numContrV];
     }

     for (int i = 0; i < numContrU; i++)
	  for (int j = 0; j < numContrV; j++){
	       inputfile >> ctlPoints[i][j].x >> ctlPoints[i][j].y >> ctlPoints[i][j].z;
	  }
     
     inputfile.close();

     numContrU--; numContrV--; //The 

     bSpline();

     vertexNormal();
     createObject(argv[2]);
     int k = 0;
     cout<<"Axis of rotation : ";
     cin>>rotation_axis.x>>rotation_axis.y>>rotation_axis.z>>theta;
     rotation_axis = glm::normalize(rotation_axis);         
	
     // init GLUT and create window
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
     glutInitWindowPosition(100, 100);
     glutInitWindowSize(320, 320);
     glutCreateWindow("B-Spline Surface");
     lightInit();
     // register callbacks
     glutDisplayFunc(renderScene);
     glutReshapeFunc(changeSize);      // register changeSize function

     glutKeyboardFunc(processNormalKeys);
     glutSpecialFunc(processSpecialKeys);

     glEnable(GL_DEPTH_TEST);

     // enter GLUT event processing loop
     glutMainLoop();

     return 0;
}
