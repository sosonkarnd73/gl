#include"vor.h"
int get_vertices(GLuint vbo[], int opp)
{
     cout<<"01 \n"<<endl;
     region r1;
     point p;
     vector<point> s;
     set<point> vert1;
     int i, k=0, length;
    
     float e,f,m=0,n=0, max_x = 0.0f, max_y = 0.0f,x,y;

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
     cout<<"02 \n"<<endl;
     i =0;
     for(int j=0;j<s.size();j++){
	  r1=region(s,j);
	  if(r1.in_same_region(r1.p)){
	       vert1.insert(r1.p);
	  }
	  for(int a=0; a<r1.vertices.size(); a++){
	       if(r1.in_same_region(r1.vertices[a])){
		    vert1.insert(r1.vertices[a]);
	       }
	  }
     }
     cout<<"03 \n"<<endl;
     s.clear();
     float vert[length];         
     int a=0;
     for(a=0;a<1;a+=2){      
	  vert[a] = 0.7;  
	  vert[a+1] =0.4;
     }
      
     cout<<"05 \n"<<endl;
     a = (a)/2;   //Number of points to be drawn

     
     glGenBuffers(1, vbo);
     glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
     cout<<"06 \n"<<endl;
     return a;
}

/*
//float vert2[] = {0.1, 0.1, 0.2, 0.2, 0.3, 0.3, 0.4, 0.4,  0.5, 0.5, 0.6, 0.6};
cout<<" ("<<vert[a]<<","<<vert[a+1]<<")\t";

     
*/
