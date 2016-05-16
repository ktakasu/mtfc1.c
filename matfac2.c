/*matrix factorization*/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "matrix.c"

#define K 2
 
#define STEP 100000
#define ALPHA 0.0002
#define BETA 0.02
#define THT 0.001

int M,N;

//(i,j)成分のRとP*Qの差を求める
double rating_er(double r,struct matrix *p,struct matrix *q,int i,int j){
  int k;
  double pq=0;
  for(k=0;k<K;k++){
    pq+= *(p->element+k+i*p->col) * *(q->element+j+k*q->col);
  }
  return(r-pq);
}

//RとP*Qの差を求める
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
  char filename[20]="matrix1.txt",
    filenamep[20]="matrixp.txt",filenameq[20]="matrixq.txt";
  struct matrix *p,*q,*r;
  double err;
  FILE *gp;
  srand((unsigned)time(NULL));
  r=mat_read(filename);
  M=r->row;N=r->col;
  p=mat_read(filenamep);
  q=mat_read(filenameq);

  printf("R\n");mat_print(r);
  printf("P\n");mat_print(p);
  printf("Q\n");mat_print(q);

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
      if(get_er(r,p,q)<THT){printf("step %d\n",step);break;}
    }
  }
  printf("========================\n");
  printf("error=%f\n",get_er(r,p,q));
  printf("P\n"); mat_print(p);
  printf("Q\n");mat_print(q);
  printf("P*Q\n");mat_print(mat_mul(p,q));

  return(0);
}
