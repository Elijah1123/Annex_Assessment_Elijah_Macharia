#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long pthFactor(long n, long p){
    long small[100000];
    int smallCount = 0;

    long sqrtN = (long)sqrt((double)n);

    for (long i = i; i <= sqrtN; i++){
        if (n % i == 0){
            small[smallCount++] = i;
             
        }
    }

    if (p <= smallCount){
        return small[p - 1];

    }

    long reamining = p - smallCount;

    for (int i = smallCount - 1; i >= 0; i--){
        long largeFactor = n / small[i];

        if (largeFactor == small[i]) continue;
        reamining--;
        if (reamining == 0){
            return largeFactor;
        }
       
    }

    return 0;

}

int main(){

    printf("%ld\n", pthFactor(10, 3));
    printf("%ld\n", pthFactor(10, 1));
    printf("%ld\n", pthFactor(10, 4));
    printf("%ld\n", pthFactor(10, 5));
    printf("%ld\n", pthFactor(10, 5));
    printf("%ld\n", pthFactor(36, 5));
    printf("%ld\n", pthFactor(100000LL, 1));
    printf("%ld\n", pthFactor(1, 1));
    printf("%ld\n", pthFactor(1, 2));

}