
#ifndef _MATRIX_H_
#define _MATRIX_H_

struct matrix{
  int row;   //行数
  int col;   //列数
  double *element;//要素
};

void mat_print(struct matrix *a);
struct matrix *mat_alloc(int m,int n);
struct matrix *mat_read(char *filename);
int mat_write(char *filename,struct matrix *p);
void mat_free(struct matrix *a);
struct matrix *mat_copy(struct matrix *a);
struct matrix *mat_uni(int n,double k);
struct matrix *mat_test(struct matrix *a,int min,int max,int sec);
struct matrix *mat_add(struct matrix *a,struct matrix *b);
struct matrix *mat_sub(struct matrix *a,struct matrix *b);
struct matrix *mat_mul(struct matrix *a,struct matrix *b);
struct matrix *mat_trans(struct matrix *a);
struct matrix *mat_inv(struct matrix *a_ori);
struct matrix *get_col(struct matrix *p,int k);
struct matrix *get_row(struct matrix *p,int k);

#endif // _MATRIX_H_
