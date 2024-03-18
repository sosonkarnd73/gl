#include <time.h>
#include <stdio.h>
#include <unistd.h>
int main(){
  clock_t c;
  
  int k = 1000;
  while(k){
    if(k%13 == 0)
      {
	usleep(4);
	c = clock();
	printf("%ld\t",c%100);
      }
    k--;
  }
  printf("\n");
  return 0;
}
