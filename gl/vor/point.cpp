#include"vor.h"
point::point(){
	  x=0;
	  y=0;
     }
point::point(float x, float y){
	  this->x=x;
	  this->y=y;
     }
point::point(const point *p){
	  this->x=p->x;
	  this->y=p->y;
     }
bool point::operator==(point p){
  bool out = false;
  if(this->x == p.x && this->y == p.y){
    out = true;
  }
  return out;
}
void point::print(){
    cout<<"x : "<<x<<"\ty : "<<y<<endl;
  }
bool point::compare_x(point p1, point p2){
	  bool f=true;
	  if(p2.x - p1.x<=0){
	       f = false;
	  }
	  return f;
     }

bool point::compare_y(point p1, point p2){
	  bool f=true;
	  if(p2.y - p1.y<=0){
	       f = false;
	  }
	  return f;
     }
