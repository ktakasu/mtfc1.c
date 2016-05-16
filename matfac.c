/*matfac*/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "matrix.c"

//#define M 5
//#define N 4
#define K 2
 
#define STEP 5000
#define ALPHA 0.0002
#define BETA 0.02
#define THT 0.001
#define GNU 0

int M,N;

double rating_er(double r,struct matrix *p,struct matrix *q,int i,int j){
  int k;
  double pq=0;
  for(k=0;k<K;k++){
    pq+= *(p->element+k+i*p->col) * *(q->element+j+k*q->col);
  }
  return(r-pq);
}

double get_er(struct matrix *r,struct matrix *p,struct matrix *q){
  int i,j;
  double error=0;
  for(i=0;i<M;i++){
    for(j=0;j<N;j++){
      if(*(r->element+j+i*r->col)==0)continue;
      error+= pow(rating_er(*(r->element+j+i*r->col),p,q,i,j),2);
    }
  }
  for(i=0;i<p->row*p->col;i++)error+=BETA/2*pow(*(p->element+i),2);
  for(i=0;i<q->row*q->col;i++)error+=BETA/2*pow(*(q->element+i),2);
  return(error);
}

int main(void){
  int i,j,k,step;
  char filename[20]="mataaa.txt";
  struct matrix *p,*q,*r;
  double err;
  FILE *gp;
  srand((unsigned)time(NULL));
  r=mat_read(filename);
  M=r->row;N=r->col;
  p=mat_test(mat_alloc(M,K),0,1,10000);
  q=mat_test(mat_alloc(K,N),0,1,10000);

  mat_print(r); mat_print(p); mat_print(q);

  if(GNU==1){
    gp = popen("gnuplot -persist","w");
    fprintf(gp,"set xrange[0:50000]\n");
    fprintf(gp,"set yrange[0:0.1]\n");
    fprintf(gp,"plot '-' \n");
  }

  for(step=0;step<STEP;step++){
    for(i=0;i<M;i++){
      for(j=0;j<N;j++){
	if(*(r->element+j+i*r->col)==0)continue;
	err=rating_er(*(r->element+j+i*r->col),p,q,i,j);
	for(k=0;k<K;k++){
	  *(p->element+k+i*p->col)
	    +=ALPHA*(2*err* *(q->element+j+k*q->col)-BETA* *(p->element+k+i*p->col));
	  *(q->element+j+k*q->col)
	    +=ALPHA*(2*err* *(p->element+k+i*p->col)-BETA* *(q->element+j+k*q->col));
	}
      }
    }
    if((step%100)==0){
      //printf("%f \n",get_er(r,p,q));
      if(GNU==1)fprintf(gp,"%d %f\n",step,get_er(r,p,q));
      if(get_er(r,p,q)<THT){printf("step %d\n",step);break;}
    }
  }
  printf("%f\n",get_er(r,p,q));
  mat_print(p);mat_print(q);
  mat_print(mat_mul(p,q));

  if(GNU==1){
    fprintf(gp,"e\n");
    fprintf(gp,"quit\n");
    pclose(gp);
  }
  return(0);
}
