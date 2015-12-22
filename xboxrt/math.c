
float sinf(float x) {
    __asm__ __volatile__ ("fsin" : "+t" (x));
    return x;
}

float cosf(float x) {
    __asm__ __volatile__ ("fcos" : "+t" (x));
    return x;
}

double sqrt(double x) {
    __asm__ __volatile__ ("fsqrt" : "+t" (x));
    return x;
}
