#include"vor.h"
int get_vertices(GLuint vbo[], int opp)
{
     region r1;
     point p;
     vector<point> s;
     int i, k=0, length;
     vector<region> r;
     float e,f,m=0,n=0, max_x = 0.0f, max_y = 0.0f;

     for(i = 0; i<15; i++){
	  e = rand()%opp;
	  f = rand()%opp;
	  if(rand()%5 ==0){
	       f = (-1)*f;
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
     
     i=0;
     for(auto ri:r){
	  if(ri.in_same_region(ri.p)){
	       x = ri.p.x;
	       y = ri.p.y;
	       vert[i] = x;
	       vert[i+1] = y;
	       if(abs(vert[i]) > max_x){
		 max_x = vert[i];
	       }
	       if(abs(vert[i+1]) > max_y){
		 max_y = vert[i+1];
	       }     
	       i = i+2;
	  }
	  for(int a=0; a<ri.vertices.size(); a++){
	       if(ri.in_same_region(ri.vertices[a])){
		    vert[i] = ri.vertices[a].x;
		    vert[i+1] = ri.vertices[a].y;
		     
		    if(abs(vert[i]) > max_x){
			 max_x = vert[i];
		    }
		    if(abs(vert[i+1]) > max_y){
		      max_y = vert[i+1];
		    }

		    i = i+2;
	       }
	  }
    }
     r.clear();
     int a=0;
      for(a=0;a<i;a+=2){      
      	  vert[a] = vert[a]/max_x;	  
      	  vert[a+1] = vert[a+1]/max_y;
      }
      
     a = (a)/2;   //Number of points to be drawn

     
     glGenBuffers(1, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

     return a;
}

/*
          //float vert2[] = {0.1, 0.1, 0.2, 0.2, 0.3, 0.3, 0.4, 0.4,  0.5, 0.5, 0.6, 0.6};
	  cout<<" ("<<vert[a]<<","<<vert[a+1]<<")\t";

     
*/
