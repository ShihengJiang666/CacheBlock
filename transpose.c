#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void transpose( int n, int blocksize, int *dst, int *src ) {
    int i,j,k,l;
    /* TO DO: implement blocking (two more loops) */
    /*
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            dst[j + i*n] = src[i + j*n];
    */
    //cache blk part
    for(i = 0; i < n-n%blocksize; i+=blocksize)
        for(j = 0; j < n-n%blocksize; j+=blocksize)
            for(k=0; k<blocksize; k++)
                for(l=0; l<blocksize; l++)
                    dst[j+l+(i+k)*n] = src[i+k+(j+l)*n];

    //for border: 2 parts: 1. right and right-below; 2 below
    for(i=n-n%blocksize; i<n;i++)
        for(j=0;j<n;j++)
            dst[i + j*n] = src[j + i*n];
    for(j=n-n%blocksize; j<n;j++)
        for(i=0;i<n-n%blocksize;i++)
            dst[i + j*n] = src[j + i*n]; 

}

int main( int argc, char **argv ) {
    int n = 2000,i,j;
    int blocksize = 400; /* to do: find a better block size */

    /* allocate an n*n block of integers for the matrices */
    int *A = (int*)malloc( n*n*sizeof(int) );
    int *B = (int*)malloc( n*n*sizeof(int) );

    /* initialize A,B to random integers */
    srand48( time( NULL ) );
    for( i = 0; i < n*n; i++ ) A[i] = lrand48( );
    for( i = 0; i < n*n; i++ ) B[i] = lrand48( );

    /* measure performance */
    struct timeval start, end;

    gettimeofday( &start, NULL );
    transpose( n, blocksize, B, A );
    gettimeofday( &end, NULL );

    double seconds = (end.tv_sec - start.tv_sec) +
        1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "%g milliseconds\n", seconds*1e3 );

    /* check correctness */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            if( B[j+i*n] != A[i+j*n] ) {
                printf("Error!!!! Transpose does not result in correct answer!!\n");
                exit( -1 );
            }

    /* release resources */
    free( A );
    free( B );
    return 0;
}

