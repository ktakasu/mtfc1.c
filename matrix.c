
#include "matrix.h"

//行列aを表示
void mat_print(struct matrix *a){
  int i,j;
  printf("%d %d\n",a->row,a->col);
  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      printf("%f ",*(a->element+ j + i*a->col));
    }
    printf("\n");
  }
}

//新たな行列を作る
struct matrix *mat_alloc(int m,int n){
  struct matrix *p;
  double *d;
  int i,j;

  p=(struct matrix *)malloc(sizeof(*p));
  p->row=m;
  p->col=n;
  if((p->element=(double *)calloc(m*n,sizeof(double)*m*n))==NULL){
    fprintf(stderr,"not enough memory");
    exit(1);
  }
  return(p);
}

//行列をファイルから読み込み
struct matrix *mat_read(char *filename){
  FILE *fp;
  int m,n,k;
  double d;
  struct matrix *p;

  if((fp=fopen(filename,"r"))==NULL){
    fprintf(stderr,"can't open %s\n",filename);
    return(NULL);
  }
  fscanf(fp,"%d %d",&m,&n);
  p=mat_alloc(m,n);

  k=0;
  while(fgetc(fp)!=EOF){
    if(k>=m*n)break;
    fscanf(fp,"%lf",&d);
    *(p->element+k)=d;
    k++;
  }
  return(p);
}

//行列をファイルに書き込み
int mat_write(char *filename,struct matrix *p){
  FILE *fp;
  int i,j;

  if((fp=fopen(filename,"w"))==NULL){
      fprintf(stderr,"can't open %s\n",filename);
      return(-1);
  }

  fprintf(fp,"%d %d\n",p->row,p->col);

  for(i=0;i<p->row*p->col;i++){
    fprintf(fp,"%f",*(p->element+i));
    if((i%p->col)==(p->col-1))fprintf(fp,"\n");
    else fprintf(fp," ");
  }

  return(0);
}

//行列を削除しメモリを解放する
void mat_free(struct matrix *a){
  int i,j;
  if(a->col<=0)return;
  if(a->row<=0)return;
  free(a->element);
  free(a);
  return;
}

//行列をコピー
struct matrix *mat_copy(struct matrix *a){
  struct matrix *p;
  int i,j;

  p=mat_alloc(a->row,a->col);
  p->row=a->row;
  p->col=p->col;
  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      *(p->element+j+i*a->col)=*(a->element+j+i*a->col);
    }
  }
  return(p);
}

//単位行列を作る(さらにk倍する)
struct matrix *mat_uni(int n,double k){
  struct matrix *p;
  int i;
  p=mat_alloc(n,n);
  for(i=0;i<n;i++){
    *(p->element+i+i*n)=k;
  }
  return(p);
}

//テスト用行列を作る(中身はランダム:minからmaxまでをsec個に分割)
struct matrix *mat_test(struct matrix *a,int min,int max,int sec){
  double *d;
  int i,j;
  if(a==NULL)return(NULL);

  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      *(a->element+j+i*a->col)=(double)(rand()%((max-min)*sec))/sec+min;
    }
  }
  return(a);
}

//足し算 aとbを足す
struct matrix *mat_add(struct matrix *a,struct matrix *b){
  int i,j;
  struct matrix *c;
  if(a->row!=b->row){printf("error at mat_add:row \n");return NULL;}
  if(a->col!=b->col){printf("error at mat_add:col \n");return NULL;}
  c=mat_alloc(a->row,a->col);

  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      *(c->element+j+i*a->col) = 
	*(a->element+j+i*a->col) + *(b->element+j+i*a->col);
    }
  }
  return(c);
}

//引き算 aからbを引く
struct matrix *mat_sub(struct matrix *a,struct matrix *b){
  int i,j;
  struct matrix *c;
  if(a->row!=b->row){printf("error at mat_sub:row ab\n");return NULL;}
  if(a->col!=b->col){printf("error at mat_sub:col ab\n");return NULL;}
  c=mat_alloc(a->row,a->col);

  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      *(c->element+j+i*a->col) = 
	*(a->element+j+i*a->col) - *(b->element+j+i*a->col);
    }
  }
  return(c);
}

//掛け算 a,bを掛ける
struct matrix *mat_mul(struct matrix *a,struct matrix *b){
  int i,j,k;
  double temp=0;
  struct matrix *c;

  if((a==NULL)||(b==NULL))
    {printf("error at mat_mul;input is NULL\n");return NULL;}
  if(a->col!=b->row){printf("error at mat_mul;a->col!=b->row\n");return NULL;}
  c=mat_alloc(a->row,b->col);

  for(i=0;i<a->row;i++){
    for(j=0;j<b->col;j++){
      for(k=0;k<a->col;k++){
	temp+=*(a->element+k+i*a->col) * *(b->element+j+k*b->col);
      }
      *(c->element+j+i*c->col)=temp;
      temp=0;
    }
  }
  return(c);
}

//転置行列
struct matrix *mat_trans(struct matrix *a){
  int i,j;
  struct matrix *a_tra;

  if(a==NULL){printf("error at mat_trans:input is NULL");return NULL;}
  a_tra=mat_alloc(a->col,a->row);

  for(i=0;i<a->row;i++){
    for(j=0;j<a->col;j++){
      *(a_tra->element+i+j*a->row)=*(a->element+j+i*a->col);
    }
  }
  return(a_tra);
}
//逆行列(はき出し法)a_oriの逆行列
struct matrix *mat_inv(struct matrix *a_ori){
  int i,j,k,m,n;
  double d,dd;
  struct matrix *a;
  struct matrix *a_inv;

  if(a_ori==NULL){ printf("error at mat_inv:input is NULL\n");return NULL;}
  if(a_ori->row!=a_ori->col){printf("error at mat_inv:a_ori\n");return NULL;}
  a_inv=mat_alloc(a_ori->row,a_ori->col);

  a=mat_copy(a_ori);
  for(i=0;i<a->row;i++){
    for(j=0;j<a->row;j++){
      if(i==j)*(a_inv->element+j+i*a->col)=1;
      else *(a_inv->element+j+i*a->col)=0;
    }
  }

  for(i=0;i<a->row;i++){
    d=*(a->element+i+i*a->col);
    for(j=0;j<a->row;j++){
      *(a->element+j+i*a->col)=(1/d)* *(a->element+j+i*a->col);
      *(a_inv->element+j+i*a->col)=(1/d)* *(a_inv->element+j+i*a->col);
    }
    for(k=0;k<a->row;k++){
      if(i!=k){
	dd=*(a->element+i+k*a->col);
	for(n=0;n<a->row;n++){
          *(a->element+n+k*a->col)=
	    *(a->element+n+k*a->col)- dd * *(a->element+n+i*a->col);
	  *(a_inv->element+n+k*a->col)=
	    *(a_inv->element+n+k*a->col)-dd * *(a_inv->element+n+i*a->col);
	}
      }
    }
  }
  return(a_inv);
}

//行列からk列のみを取り出す
struct matrix *get_col(struct matrix *p,int k){
  int i;
  struct matrix *v;
  v=mat_alloc(p->row,1);
  for(i=0;i<p->row;i++) *(v->element+i)=*(p->element+k+i*p->col);
  return(v);
}

//行列からk行のみを取り出す
struct matrix *get_row(struct matrix *p,int k){
  int i;
  struct matrix *v;
  v=mat_alloc(1,p->col);
  for(i=0;i<p->col;i++) *(v->element+i)=*(p->element+i+k*p->col);
  return(v);
}
