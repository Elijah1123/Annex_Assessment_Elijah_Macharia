#include <stdio.h>
#include <math.h>


long pthFactor(long n, long p){
    long factors[2000];
    int count = 0;
    long sqrtN = (long)sqrt(n);
}

for (long i = 1; i <= sqrtN; i++){
    if (n % i ==0){
        count++;
        if (count == p){
            return i;
        }
        factors[count - 1] = i;
    }
}