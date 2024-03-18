#include"vor.h"
region::region(){}     
region::region(vector<point> vect, int i){
     int j =0;
     
     p = point(vect[i]);
     
     while(j < vect.size()){
	  if(i != j){
	       line l = line(p, vect[j]);
	       vertices.push_back(l.mid);
	       v.push_back(l);
	       b.push_back((v.back()).sign_delta_y(p));
	  }
	  j++;
     }
     int len = v.size();
     for(int i=0;i<len-1;i++){
	  for(int j=i+1;j<len;j++){
	       point* p1 = new point((v[i]).intersect(&v[j]));
	       point p2 = *p1;
	       if(in_same_region(p2)){       
		    vertices.push_back(p2);
	       }
	  }
     }
}
     
int region::in_same_region(point p1){
     int i =0, k = 1, phi, si;
     for(i=0; i<(v.size())&&k; i++){
	  phi = v[i].sign_delta_y(p1);
	  si  =   b[i];
	  k = phi == si || !phi;
      }
     return k;
}

void region::print(){
     int i =0;
     cout<<"point ::("<<p.x<<", "<<p.y<<")\n"<<"vertices :: \n\t";
     while(i<vertices.size()){
	  cout<<"("<<vertices[i].x<<", "<<vertices[i].y<<")\t";
	  i++;
     }
     cout<<endl;
}
