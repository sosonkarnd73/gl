#include"rand.h"
//Returns a random number
int r_num(){
  int v;
  clock_t c;
  if(c%2==0){
    c = clock()%100;
    srand((unsigned int)c);
  }
    v = (int)roundl(rand())%10;
    //printf("%d ",v);
 return v;
}

int main(int args, char** argv){
  int a, b,x,c;
  if(args!=4){
    printf("Usage ./%s a b c",argv[0]);
    exit(0);
  }
  a = atoi(argv[1]);
  b = atoi(argv[2]);
  c = atoi(argv[3]);
  while(c>0){
    x = (r_num()+a)%b;
    printf("%d\t",x);
    c--;
  }
  printf("\n");
 return 0;
}
