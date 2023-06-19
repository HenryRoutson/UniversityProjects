
/* 
	MODULAR SEARCH LIBRARY
*/


#include <stdio.h>
#include <math.h>

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int closest_index(void *A[], int n, void *T, double (*minus)(void *, void *)) {
    int c = 0; // index

    for (int i = 1; i < n; i++) {
        double cur_dist = fabs(minus(A[i], T));
        double min_dist = fabs(minus(A[c], T));
        if (min_dist >  cur_dist) {
            c = i;
        }
    }

    return c;
}

// https://en.wikipedia.org/wiki/Binary_search_algorithm
int approx_binary_search(void *A[], int n, void *T, double (*minus)(void *, void *)) {
    // impliment iterative binary search
    // for use with pointer arrays

    // minus() allows use of comparisions such as strcmp,
    // as compared to using a function to return a scalar

    int L = 0, M, R = n - 1;
    M = (L + R) / 2;

    if (n <= 3) { return closest_index(&A[M], n, T, minus); }

    while (L <= R) {
        M = (L + R) / 2;

        if (minus(A[M], T) < 0) { // A[M] < T
            L = M + 1; 
        } else 
        if (minus(A[M], T) > 0) { // A[M] > T
            R = M - 1;
        } else {
            return M;
        }
    }

    // check closest if no exact
    M--;
    if (M < 0) {
         M = 0;
    }
    M = min(M, n - 3);
    return M + closest_index(&A[M], 3, T, minus);     
}



/*

DOCUMENTATION

// EXAMPLE

double minus(void *p1 , void *p2) {
    double first = *(double *) p1;
    double second = *(double *) p2;

    return first - second;
}

int main()
{
	
	double A[] = {1,2,3,4};
	double T = 5;
	
	void *p_A[] = {&A[0], &A[1], &A[2], &A[3]};
	int n = sizeof(p_A) / sizeof(*p_A);
	int i = approx_binary_search(p_A, n, (void *)&T, minus);
	printf("%i", i);

	return 0;
}


*/

