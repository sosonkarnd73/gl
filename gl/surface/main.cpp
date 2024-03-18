//Author: Posuer(Wang Gengyu)
//E-Mail: posuer000@gmail.com
//First released on www.wanggengyu.com
#include "surface.h"
float deBoor(int i, int p, float u, float *knots)
{
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

void bSpline()

{

     for (int u = 0; u < numSegment; u++){ //for each point on the surface, 20*20 points for this surface
	  for (int v = 0; v < numSegment; v++){ 
	       for (int i = 0; i <= numContrU; i++) { //for each control points
		    for (int j = 0; j <= numContrV; j++){
			 P[u][v] = P[u][v] + ctlPoints[i][j] * double(deBoor(i, degreeU, u*piece, knotU) * deBoor(j, degreeV, v*piece, knotV));
		    }
	       }
	  }

	  Vec3 faceNormal(Vec3 a, Vec3 b,	Vec3 c)
	  {
	       Vec3 ab = a - b;
	       Vec3 ac = a - c;
	       Vec3 cross = ab.crossProduct(ac);
	       return cross.normalize();
	  }


	  void vertexNormal()
	  {
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
		    vertexnormal[i][0] = (facenormal[i - 1][0] + facenormal[i][0]) / 2;
		    vertexnormal[0][i] = (facenormal[0][i-1] + facenormal[0][i]) / 2;
		    vertexnormal[i][19] = (facenormal[i - 1][18] + facenormal[i][18]) / 2;
		    vertexnormal[19][i] = (facenormal[18][i-1] + facenormal[18][i]) / 2;
	       }
	       //others
	       for (int i = 1; i < numSegment-1; i++)
		    for (int j = 1; j < numSegment-1; j++){
			 vertexnormal[i][j] = (facenormal[i - 1][j - 1] + facenormal[i - 1][j] + facenormal[i][j - 1] + facenormal[i][j]) / 4;
		    }
	  }

	  void lightInit()
	  {
		
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


	       GLfloat lightColor0[] = { 0.0f, 0.4560f, 0.70f, 1.0f }; //Color (1.0f, 1.0f, 1.0f)
	       GLfloat lightPos0[] = { 1.0f, 20.0f, 1.0f, 1.0f }; //Positioned at (-10.0f, 50.0f, 1.0f
	       glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	       glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
	       glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	       glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0); //for two sides lighting


	  }

	  void renderScene(void)
	  {
	       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	       glLoadIdentity(); 		// Reset (Modelview) transformations

	       // Set the camera: Note! gluLookAt is called in Modelview mode.
	       // Reset transformations
	       glMatrixMode(GL_MODELVIEW);


	       glLoadIdentity();         //Reset the drawing perspective
	
	       // Set the camera
	       gluLookAt(0, 0.0f, 80.0f, 0, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	       // draw something
	       glPushMatrix(); //Save the transformations performed thus far

	       //change the visualization view
	       glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	       glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	       glColor3f(0.9f, 0.9f, 0.9f);
	
	       if (flatSmooth){/******************* Smooth shading **********************/
		    glShadeModel(GL_SMOOTH);
		    glBegin(GL_POINTS);
		
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
		    Vec3 facenormal1, facenormal2;
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


	  void changeSize(int ww, int hh)
	  {

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

	  void processSpecialKeys(int key, int x, int y)
	  {
	       switch (key) {
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
	       case GLUT_KEY_LEFT:
		    yAngle += 5;
		    if (yAngle == 360) yAngle = 0;
		    break;
	       case GLUT_KEY_RIGHT:
		    yAngle -= 5;
		    if (yAngle == -360) yAngle = 0;
		    break;
	       case GLUT_KEY_UP:
		    xAngle += 5;
		    if (xAngle == 360) xAngle = 0;
		    break;
	       case GLUT_KEY_DOWN:
		    xAngle -= 5;
		    if (xAngle == -360) xAngle = 0;
		    break;
	       }
	       glutPostRedisplay();
	  }


	  int main(int argc, char **argv) {

	       fstream inputfile("input2.txt");
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

	
	       ctlPoints = new Vec3*[numContrU];
	       for (int i = 0; i < numContrU; i++){
		    ctlPoints[i] = new Vec3[numContrV];
	       }

	       for (int i = 0; i < numContrU; i++)
		    for (int j = 0; j < numContrV; j++){
			 inputfile >> ctlPoints[i][j].x >> ctlPoints[i][j].y >> ctlPoints[i][j].z;
		    }

	       inputfile.close();

	       numContrU--; numContrV--; //The 

	       bSpline();

	
	       // init GLUT and create window
	       glutInit(&argc, argv);
	       glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	       glutInitWindowPosition(100, 100);
	       glutInitWindowSize(320, 320);
	       glutCreateWindow("B-Spline Surface");
	       lightInit();
	       vertexNormal();
	       // register callbacks
	       glutDisplayFunc(renderScene);
	       glutReshapeFunc(changeSize);      // register changeSize function
	       glutIdleFunc(renderScene);
	       //glutKeyboardFunc(processNormalKeys);
	       glutSpecialFunc(processSpecialKeys);

	       glEnable(GL_DEPTH_TEST);

	       // enter GLUT event processing loop
	       glutMainLoop();

	       return 0;
	  }

