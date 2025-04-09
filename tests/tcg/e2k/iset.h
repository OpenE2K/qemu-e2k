#ifndef ISET_H
#define ISET_H

#include <stdio.h>
#include <stdint.h>

#define MDL_E3M              0    /* Elbrus */
#define MDL_ES               1    /* Elbrus-S */
#define MDL_E3S              2    /* Elbrus-3S */
#define MDL_E2S              3    /* Elbrus-4C */
#define MDL_ES2              4    /* Elbrus-2C+ */
#define MDL_RESERVED         5    /* unknown "reserved" processor */
#define MDL_ES2_NO_DSP       6    /* Elbrus-2CM */
#define MDL_E8C              7    /* Elbrus-8C  */
#define MDL_E1CP             8    /* Elbrus-1C+ */
#define MDL_E8C2             9    /* Elbrus-8C2 */
#define MDL_E12C            10    /* Elbrus-12C */
#define MDL_E16C            11    /* Elbrus-16C */
#define MDL_E2C3            12    /* Elbrus-2C3 */
#define MDL_E48C            13    /* Elbrus-48C FIXME: assumption */
#define MDL_E8V7            14    /* Elbrus-8v7 FIXME: assumption */

static int get_iset(void) {
    static int version = 0;
    if (version <= 0) {
        static struct { int iset; int mdl; } v[] = {
            { 7, MDL_E48C },
            { 6, MDL_E12C },
            { 5, MDL_E8C2 },
            { 4, MDL_E8C  },
            { 3, MDL_E2S  },
            { 2, MDL_ES   },
            { 1, MDL_E3M  },
            { 0, 0 },
        };
        int mdl;
        asm("rrd %%idr, %0" : "=r"(mdl));
        mdl &= 0xff;
        for (int i = 0; v[i].iset > 0; ++i) {
            if (mdl >= v[i].mdl) {
                version = v[i].iset;
                break;
            }
        }
    }
    return version;
}

#define push_iset(X) asm(".push_iset " #X)
#define pop_iset() asm(".pop_iset ")

#define iset get_iset()
#define if_iset(X, test) \
    if (iset < (X)) { \
        if (test && *test) { \
            fprintf(stderr, "skip %s at %s:%d\n", test, __FILE__, __LINE__); \
        } else { \
            fprintf(stderr, "skip at %s:%d\n", __FILE__, __LINE__); \
        } \
    } else

#endif /* ISET_H */

