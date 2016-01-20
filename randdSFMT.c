#include <stdio.h>
#include <stdlib.h>
#include "dSFMT.h"

double ran1() {
    return dsfmt_gv_genrand_close_open();
}

void seedit(const char *flag) {
    FILE *pfseed;
    unsigned int seed2;

    if (flag[0] == 's') {
        pfseed = fopen("/dev/urandom", "rb");
        if (pfseed) {
            fread(&seed2, sizeof(seed2), 1, pfseed);
            fclose(pfseed);
        } else {
            printf("Cannot open /dev/urandom\n");
            exit(1);
        }
        /*seed2 = time(NULL);*/
        dsfmt_gv_init_gen_rand(seed2);
        printf("\n%u\n", seed2);
    }
}

int commandlineseed(char **seeds) {
    unsigned int seed2;
    seed2 = atoi(seeds[0]);
    printf("\n%u\n", seed2);
    dsfmt_gv_init_gen_rand(seed2);
    return 1;
}
