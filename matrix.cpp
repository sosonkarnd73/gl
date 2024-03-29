#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>

using namespace std;
void prfloat(const vector<vector<float>> &v);
float in_p(const vector<float> &v1, const vector<float> &v2);
vector<vector<float>> transpose(const vector<vector<float>> &v);
vector<vector<float>> product(const vector<vector<float>> &v1, const vector<vector<float>> &v2);

void print(const vector<vector<float>> &v)
{
  for(auto a:v)
    {
      for(auto b:a)
	{
	  cout<<""<<b<<" \t";
	}
      cout<<endl;
    }
  cout<<endl; 
}

float in_p(const vector<float> &v1,const vector<float> &v2)
{
   float s=0;
  if(v1.size()==v2.size()) {
    for(int i=0;i<v1.size();i++){
      s = s+v1[i]*v2[i];
    }
  }
  return s;
}

vector<vector<float>> product(const vector<vector<float>> &v1, const vector<vector<float>> &v2)
{
  vector<vector<float>> v3, v2t=transpose(v2);
  vector<float> v;
  vector<vector<float>>::iterator itr;
  cout<<"v1.size "<<v1.size()<<endl;
  cout<<"v2.size "<<v2.size()<<endl;
  cout<<"v1[0].size "<<v1[0].size()<<endl;
  if(v1.size()!=0 && v1[0].size() == v2.size())
    {

      for(vector<float> x:v1)
	{ 
	  itr= v2t.begin();
	  while(itr!=v2t.end()){
	    v.push_back(in_p(x,*itr));
	    ++itr;
	  }
	  v3.push_back(v);
	  v.clear();
	}
    }
  return v3;
}

vector<vector<float>> transpose(const vector<vector<float>> &v)
{
  vector<vector<float>> tr;
  vector<float> v1;
  for(int a=0;a<(v[0].size());a++)
    {
      for(int b=0;b<(v.size());b++)
	{
	  v1.push_back(v[b][a]);
	}
      tr.push_back(v1);
      v1.clear();
    }
  return tr;
}

int main()
{
  vector<vector<float>> a= {{0,      0,       0,	 0,	  0,	  0,	   0,	   0, 0, 0, 0},
     {0.57735,0,       0,	 0,	  0,	  0,	   0,	   0, 0, 0, 0},
     {0.57735,0,       0,        0,       0,      0,       0,      0, 0, 0, 0},
     {0.57735,0,       0,        0,       0,      0,       0,      0, 0, 0, 0},
     {0,      0.707106,0.707106, 0.707106,0,      0,       0,      0, 0, 0, 0},
     {0,      0.707106,0.707106, 0.707106,0,      0,       0,      0, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0,       1,      0, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0,       0,      1, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0.57735, 0,      0, 1, 0, 0},
     {0,       0,       0,        0,      0,      0,       0.57735,0, 0, 1, 0},
     {0,       0,       0,        0,      0,      0,       0.57735,0, 0, 0, 1}
  };
  vector<vector<float>> b={{0,      0,       0,	 0,	  0,	  0,	   0,	   0, 0, 0, 0},
     {0.57735,0,       0,	 0,	  0,	  0,	   0,	   0, 0, 0, 0},
     {0.57735,0,       0,        0,       0,      0,       0,      0, 0, 0, 0},
     {0.57735,0,       0,        0,       0,      0,       0,      0, 0, 0, 0},
     {0,      0.707106,0.707106, 0.707106,0,      0,       0,      0, 0, 0, 0},
     {0,      0.707106,0.707106, 0.707106,0,      0,       0,      0, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0,       1,      0, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0,       0,      1, 0, 0, 0},
     {0,      0,       0,        0,       0.57735,0.57735, 0,      0, 1, 0, 0},
     {0,       0,       0,        0,      0,      0,       0.57735,0, 0, 1, 0},
     {0,       0,       0,        0,      0,      0,       0.57735,0, 0, 0, 1}
  };


  // for(float i =0;i<n;i++){
  //   for(float j =0;j<n;j++){
  //     cin>>x;
  //     v1.push_back(x);
  //     v2.push_back(rand()%10);
  //   }
  //   a.push_back(v1); 
  //   v1.clear();
  //   b.push_back(v2);
  //   v2.clear();
  // }

  print(a);
  vector<vector<float>> p = product(product(product(a,b),a),a);

  print(p);
  return 0;
}
