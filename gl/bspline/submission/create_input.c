#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int m,n;
void print(int a, double *f1, double *f2){
     if(a==0)
     {
	  srandom((m+n)*clock());
	  *f1 = (double)(random()%1000000)/1000000.0;
	  *f2 = (double)(random()%1000000)/1000000.0;
	  srandom((m+n)*clock());
	  if(random()%2==0)
	  {
	       *f1 = (-1)*(*f1);
	  }
	  if(random()%2==1)
	  {
	       *f2 = (-1)*(*f2);
	  }
     }
     printf("%lf\t%lf\n",*f1,*f2);
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
     M : M-1
  */
     int type = 0, p=0, i = 0, j = 0;
     double f1, f2;
     scanf("%d%d%d",&type,&n,&p);
     printf("%d\t%d\t%d\n", type, n, p);
     m = n+p+1;
     double factor = 1.0/(m-1);
     if(type != 2){
	  while(i<(n))
	  {
	       print(0,&f1,&f2);
	       i++;
	  }
     }
     else{
	  double cp[p*2];
	  for(i = 0; i<p;i++)
	  {
	       print(0,&f1,&f2);
	       cp[j]=f1;
	       cp[j+1]= f2;
	       j = j+2;
	  }
	  j = 0;

	  for(i; i<n-p; i++)
	  {
	       print(0,&f1,&f2);
	  }
	  for(i,j = 0; i<n;i++,j+=2)
	  {
	       print(1,&cp[j],&cp[j+1]);
	  }	  
     }
     printf("%d \n",m);
     if(type == 0 || type == 2)
     {
	  for(i = 0; i<m;i++){
	       printf("%lf ",i*factor);
	  }
     }
     if(type ==1 )
     {
	  for(i = 0; i<=p;i++)
	  {
	       printf("%lf ",0.0);
	  }
	  factor = 1.0/(n-p);
	  for(i=1; i<n-p; i++)
	  {
	       printf("%lf ",i*factor);
	  }
	  for(i = 0; i<=p;i++)
	  {
	       printf("%lf ",1.0);
	  }
     }
     
     printf("\n");
     f1 = (double)(random()%1000000)/1000000.0;
     f2 = (double)(random()%1000000)/1000000.0;
     if(f1<f2)
     {
	  printf("%lf\t%lf\n",f1,f2);
     }
     else
     {
	  printf("%lf\t%lf\n",f2,f1);
     }
     return 0;
}
