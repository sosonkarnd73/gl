#include"vor.h"
int get_vertices(GLuint vbo[], int opp)
{
     region r1;
     point p;
     vector<point> s;
     int i, k=0, length, a=0;
     vector<region> r;
     float e = 0,f = 0, m=0, n=0;

     for(i = 0; i<5; i++){
	  e = rand()%opp;
	  f = rand()%opp;
	  if(rand()%5 == 0){
	       f = (-1)*f;
	  }
	  if(rand()%5 == 0){
	       e = (-1)*e;
	  }
	  p = point(e,f);
	  s.push_back(p);
     }
     length = s.size();
     
     for(i =0; i<s.size(); i++){
	  r1=region(s,i);
	  r.push_back(r1);	 
	  length = length+(r1.vertices.size())*2;
     }
     s.clear();
     float vert[length];    
     float vl;
     i=0;
     int nayan = 0;
     for(auto ri:r){
	  if(ri.in_same_region(ri.p)){
	       nayan++;
	       vl = sqrt(((ri.p.x)*(ri.p.x)) + ((ri.p.y)*(ri.p.y)));
	       vert[i] = (ri.p.x)/vl;
	       vert[i+1] = (ri.p.y)/vl;
	       printf("\n%d (%f, %f)\n\t", nayan, vert[i], vert[i+1]);
	       i = i+2;
	  }
	  for(a = 0; a<ri.vertices.size(); a++){
	       if(ri.in_same_region(ri.vertices[a])){
		    vl = sqrt(((ri.vertices[a].x)*(ri.vertices[a].x)) + ((ri.vertices[a].y)*(ri.vertices[a].y)));
		    vert[i] = (ri.vertices[a].x)/vl;
		    vert[i+1] = (ri.vertices[a].y)/vl;
		    printf("\t(%f, %f)\n", vert[i], vert[i+1]);
		    nayan++;
		    i = i+2;
	       }
	  }
     }
     r.clear();    
     
     glGenBuffers(1, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, (2*nayan)*sizeof(float*), vert, GL_STATIC_DRAW);

     return nayan;
}
