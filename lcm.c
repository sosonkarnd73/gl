#include<stdio.h>
/*
  Linear Cogruential Method
  
  m > 0
  0 <= a < m          a>=2 if x(n+k) is used
  0 <= c < m
  0 <= Xo < m

  X(n+1) = (a*X(n) + c) mod m;     n>=0
*/
int a,m,c,xo;

int lcm(int x){
  int k;
  k = (a*x + c)%m;
  return k;
}
int main(){
  printf("a m c xo");
  scanf("%d%d%d%d",&a,&m,&c,&xo);
  int i = 20;
  while(i){
    xo = lcm(xo);
    printf("%d\t",xo);
    i --;
  }
  printf("\n");
  return 0;
}
