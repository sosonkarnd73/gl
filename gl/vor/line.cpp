#include"vor.h"

line::line(point a, point b){
     mid.x = (a.x+b.x)/2;
     mid.y = (a.y+b.y)/2;
	       
     flag = 0;
     slope = 0;
     if(b.y != a.y){
	  flag = 1;            
	  slope = (a.x-b.x)/(b.y-a.y);
	  intercept = mid.y-(slope*mid.x);
     }else{
	  intercept = mid.x;        // in this case intercept will be x-intercept of line.
     }
}
  
int line::sign_delta_y(point p){
     int k;
     float delta;
     if(this->flag == 0){   // flag = 0 -> line is perpendicular to x axis
	  delta = p.x - this->intercept;
     }
     else{
	  delta = p.y-((this->slope * p.x) + this->intercept);
     }
     if(delta<0){
	  k = -1;
     }
     else if(delta>0){
	  k =1;
     }
     else{
	  k =0;
     }
     return k;
}

point* line::intersect(const line *l){
     point* p=NULL;
     if(l != NULL && this->flag + l->flag > 0){
	  p = new point;
	  if(l->slope != this->slope){
	       p->x = (l->intercept - this->intercept)/(this->slope - l->slope);
	       p->y = this->slope * p->x + this->intercept;
	  }
	  else if(this->flag == 0){
	       p->x = this->intercept;
	       p->y = l->slope * p->x + l->intercept;
	  }
	  else if(l->flag == 0){
	       p->x = l->intercept;
	       p->y = this->slope * p->x + this->intercept;
	  }
     }
     return p;
}
void line::print(){
     cout<<"point on line ::\n\t mid.x :"<<mid.x<<", mid.y : "<<mid.y<<"\nflag :"<<flag<<"\nSlope :: "<<slope<<"\t intercept :: "<<intercept<<endl;
}
