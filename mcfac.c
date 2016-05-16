#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "matrix.c"

#define K 2
 
#define STEP 100000
#define ALPHA 0.001
#define MAX 3
#define MIN -1
#define CUT 15

int M,N;
struct matrix *r;

struct mlist{
  struct matrix *mp;
  struct matrix *mq;
  int count;
  struct mlist *l;
  struct mlist *r;
};

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
  return(error);
}

double get_er2(struct matrix *r,struct matrix *p,struct matrix *q){
  int i,j;
  double error=0;
  for(i=0;i<M;i++){
    for(j=0;j<N;j++){
      if(*(r->element+j+i*r->col)==0)continue;
      error+= pow(rating_er(*(r->element+j+i*r->col),p,q,i,j),2);
    }
  }
  error=error/10;
  return(error*error*error*error);
}


//行列同士を比べる.同じなら0を返す
int matcmp(struct matrix *p,struct matrix *q){
  int i;

  if(q==NULL){printf("error in matcmp\n");return(0);}

  for(i=0;i<p->col*p->row;i++){
    if(*(p->element+i)>*(q->element+i))return 1;
    if(*(p->element+i)<*(q->element+i))return -1;
  }
  return 0;
}

//p,qのmlistを作る
struct mlist *tree2(struct matrix *p,struct matrix *q){
  struct mlist *tr;

  tr=(struct mlist *)malloc(sizeof(*tr));
  tr->mp=mat_copy(p);
  tr->mq=mat_copy(q);
  tr->count=1;
  tr->l=NULL;
  tr->r=NULL;
  return(tr);
}

//二分木を探索しp,qと一致するものが無いか探す.無ければ新たに作る
int treecheck(struct matrix *p,struct matrix *q,struct mlist *root){
  int i;
  struct mlist *tr;
  tr=root;
  while(tr!=NULL){
    if((i=matcmp(p,tr->mp))>0){
      if(tr->l!=NULL){tr=tr->l;continue;}
      else{tr->l=tree2(p,q);return(1);}
    }else if(i<0){
      if(tr->r!=NULL){tr=tr->r;continue;}
      else{tr->r=tree2(p,q);return(2);}
    }

    if((i=matcmp(q,tr->mq))>0){
      if(tr->l!=NULL){tr=tr->l;continue;}
      else{tr->l=tree2(p,q);return(3);}
    }else if(i<0){
      if(tr->r!=NULL){tr=tr->r;continue;}
      else{tr->r=tree2(p,q);return(4);}
    }
    if(i==0){tr->count++;return(5);}
    printf("0\n");
    printf("%d",i);
    mat_print(q);mat_print(tr->mq);
    break;
  }

  return(-1);
}

//二分木の出力 CUTの値以上の回数観測されたP,Qを出力
void treeprint(struct mlist *tr){
  if(tr!=NULL){
    treeprint(tr->l);
    if(tr->count>=CUT){
      //if((get_er(r,tr->mp,tr->mq)<60)&&(tr->count>=CUT)){
      //if(get_er(r,tr->mp,tr->mq)<45){
      printf("===================\n");
      printf("%d %f\n",tr->count,get_er(r,tr->mp,tr->mq));
      mat_print(tr->mp);
      mat_print(tr->mq);
      mat_print(mat_mul(tr->mp,tr->mq));
      
    }
    treeprint(tr->r);
  }
}

int main(void){
  int i,j,sigp,sigq,step;
  char filename[20]="matrix1.txt";
  struct matrix *p,*q,*p2,*q2,*r2;
  struct mlist *root=NULL;
  double err,Run,a,b;
  FILE *gp;
  srand((unsigned)time(NULL));
  r=mat_read(filename);
  M=r->row;N=r->col;

  p=mat_test(mat_alloc(M,K),MIN,MAX,10);
  q=mat_test(mat_alloc(K,N),MIN,MAX,10);
  mat_print(p);mat_print(q);
  root=tree2(p,q);
  mat_print(root->mp);


  for(step=0;step<STEP;step++){
    i=rand()%(p->row*p->col);
    j=rand()%(q->row*q->col);
    sigp=(rand()%2)*2-1;
    sigq=(rand()%2)*2-1;
    
    p2=mat_copy(p);
    q2=mat_copy(q);

    *(p2->element+i)=(double)(rand()%10*(MAX-MIN)+10*MIN)/10;
    *(q2->element+j)=(double)(rand()%10*(MAX-MIN)+10*MIN)/10;
    
    Run=(double)(rand()%1000)/1000;

    if(Run<exp(-1*ALPHA*get_er2(r,p2,q2))/exp(-1*ALPHA*get_er2(r,p,q)) ){
      mat_free(p);p=p2;mat_free(q);q=q2;
    }else{
      mat_free(p2); mat_free(q2);
    }
    treecheck(p,q,root);
  }
  treeprint(root);
  printf("\n");
  return(0);
}
