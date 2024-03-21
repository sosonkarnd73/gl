#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int m,n;
void print(int a, double *f1, double *f2, double *f3){
     if(a==0)
     {
	  srandom((m+n)*clock());
	  *f1 = (double)(random()%1000000)/1000000.0;
	  *f2 = (double)(random()%1000000)/1000000.0;
	  *f3 = (double)(random()%1000000)/1000000.0;
	  srandom((m+n)*clock());
	  if(random()%2==0)
	  {
	       *f1 = (-1)*(*f1);
	  }
	  if(random()%2==1)
	  {
	       *f2 = (-1)*(*f2);
	  }
	  if(random()%3==0){
	       *f3 = (-1)*(*f3);
	  }
     }
     printf("%lf\t%lf\t%lf\n",*f1,*f2,*f3);
}

int main()
{ /* type = 0 open
     type = 1 clamped
     type = 2 closed
  */
  /* 
     p : degree of bspline curve
     n : number of control points
     m : number of control points
     N : n-1
     M : m-1
  */
     int type = 0, p=0, i = 0, j = 0;
     double f1, f2;
     printf("Type of curve : ");
     scanf("%d",&type);
     printf("Degree of curve : ");
     scanf("%d",&p);
     printf("Number of control points : ");
     scanf("%d",&n);
     
     m = n+p+1;

     //Control points
     if(type != 2){
	  while(i < n)
	  {
	       print(0, &f1, &f2, &f3);
	       i++;
	  }
     }
     else{
	  double cp[p*3];
	  for(i = 0; i < p; i++)
	  {
	       print(0, &f1, &f2, &f3);
	       cp[j]= f1;
	       cp[j+1]= f2;
	       cp[j+2]= f3;
	       j = j+3;
	  }
	  j = 0;

	  for(i; i < n-p; i++)
	  {
	       print(0,&f1,&f2, &f2);
	  }
	  for(i,j = 0; i < n; i++,j+=3)
	  {
	       print(1, &cp[j], &cp[j+1], &cp[j+2]);
	  }
     }


	  
     printf("%d \n",m);   
     //Knot vector 
     double factor = 1.0/(m-1);
     printf("%d \n",m);
     if(type == 0 || type == 2)
     {
	  for(i = 0; i < m; i++){
	       printf("%lf ",i*factor);
	  }
     }
     if(type == 1)
     {
	  for(i = 0; i<=p;i++)
	  {
	       printf("%lf ",0.0f);
	  }
	  factor = 1.0/(n-p);
	  for(i=1; i<n-p; i++)
	  {
	       printf("%lf ",i*factor);
	  }
	  for(i = 0; i<=p;i++)
	  {
	       printf("%lf ",1.0f);
	  }
     }
     
     printf("\n");
     return 0;
}
