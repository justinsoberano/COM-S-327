#include "random.h"

void init_rand() { srand(time(NULL)); }
int rand_int(int a, int b) { return rand() % (b - a + 1) + a; }
