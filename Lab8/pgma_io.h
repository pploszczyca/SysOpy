# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int i4_min ( int i1, int i2 );
int i4mat_max ( int m, int n, int a[] );

void pgma_check_data ( int xsize, int ysize, int maxg, int *garray );
void pgma_example ( int xsize, int ysize, int *garray );

void pgma_read ( char *file_in_name, int *xsize, int *ysize, int *maxg,
       int **garrary );
void pgma_read_data ( FILE *file_in, int xsize, int ysize, int *garray );
void pgma_read_header ( FILE *file_in, int *xsize, int *ysize, int *maxg );


void pgma_write ( char *file_name, char *comment, int xsize, int ysize, 
  int maxval, int *gray );
void pgma_write_old ( char *file_out_name, int xsize, int ysize, int *garray );
void pgma_write_data ( FILE *file_out, int xsize, int ysize, int *garray );
void pgma_write_header ( FILE *file_out, char *file_out_name, int xsize, 
       int ysize, int maxg );

void test_pgma_read ( char *file_name );
void test_pgma_write ( char *file_name );