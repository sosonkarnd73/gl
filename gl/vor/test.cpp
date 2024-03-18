#include"vor.h"
void test(){
     point p;
     vector<point> s;
     int i;
     vector<region> r;
     for(i = 0; i<5; i++)  {
	  p =point(rand()%17,rand()%17);
	  s.push_back(p);
	  //cout<<"point :: ("<<p.x<<", "<<p.y<<") ";
     }
     cout<<endl;
     sort(s.begin(), s.end(),point::compare_y);
     sort(s.begin(), s.end(),point::compare_x);


     for(i =0;i<s.size();i++){
	  r.push_back(region(s,i));
     }

     for(i = 0; i<50; i++){
	  srand(clock());
	  p =point(rand()%19,rand()%19);
	  s.push_back(p);
 	  //cout<<"("<<p.x<<", "<<p.y<<") ";
     }

     sort(s.begin(), s.end(),point::compare_y);
     sort(s.begin(), s.end(),point::compare_x);
     for(i =0; i<r.size();i++){
	  r[i].print();
	  cout<<endl;
	  //cout<<"In region of \t("<<r[i].p.x<<", "<<r[i].p.y<<")\n";
	  // for(auto a:s){
	  //      if(r[i].in_same_region(a)){
	  // 	    cout<<"("<<a.x<<", "<<a.y<<")\t";
	  //      }
	  //}
     }
}

int main(){
     test();
     return 0;
}
