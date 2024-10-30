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

//variables for B-Spline function
glm::vec3 **ctlPoints;
float *knotU, *knotV;
int numContrU, numContrV, degreeU, degreeV;
const float piece = 0.05;
const int numSegment = 1 / piece;

//variables for Flat/Smooth, Control Polygon and Wireframe
glm::vec3 vertexnormal[numSegment+1][numSegment+1];
glm::vec3 facenormal[numSegment][numSegment];

glm::vec3 P[numSegment+1][numSegment+1];


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

void readInput(char* in)
{
     fstream inputfile(in);
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
}

void createObject(char* objfile, char* id, char* idparent)
{
     fstream  outputfile(objfile);
     outputfile<<"id "<<id<<endl;
     outputfile<<"a "<<idparent<<endl;
     outputfile<<"d "<<" 5.0 "<<endl;
     
     outputfile<<"p "<<"0.0    0.0    0.0"<<endl;

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
     if(argc != 5)
     {
	  cout<<"Usage : ./"<<argv[0]<<" bspline_input obj_output id parentid "<<endl; 
	  return 1;
     }
     readInput(argv[1]);
     numContrU--; numContrV--;

     bSpline();

     vertexNormal();
     createObject(argv[2], argv[3], argv[4]);
     for(int i = 0; i < numSegment+1; i++){
       for(int j = 0; j < numSegment+1; j++){
	 cout<<"v i j : "<<i<<","<<j<<": "<<P[i][j].x<<", "<<P[i][j].y<<", "<<P[i][j].z<<endl;
       }
     }
     
     return 0;
}
