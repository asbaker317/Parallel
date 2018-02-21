#include "grph.hpp"

extern "C" {
#include "../simple-4.4J/src/simple.h"
}

#define bits(x,k,j) ((x>>k) & ~(~0<<j))

#define KEY(A) A.adj_min_weight_key
typedef A_Network DATA;


void all_countsort_smp(int q,
        DATA *inArr,
        DATA *lSorted,
        int R,
        int bitOff, int m,
        THREADED)
    /****************************************************/
    /* R (range)      must be a multiple of SMPS */
    /* q (elems/proc) must be a multiple of SMPS */
{
    register int
        j,
        k,
        last, temp,
        offset;

    int *myHisto,
        *mhp,
        *mps,
        *psHisto,
        *allHisto;

    myHisto  = (int *)node_malloc(THREADS*R*sizeof(int), TH);
    psHisto  = (int *)node_malloc(THREADS*R*sizeof(int), TH);

    mhp = myHisto + MYTHREAD*R;

    for (k=0 ; k<R ; k++)
        mhp[k] = 0;

    pardo(k, 0, q, 1)
        mhp[bits(KEY(inArr->at(k)),bitOff,m)]++; // was inArr[k], changed to: inArr->at(k) ??

    node_Barrier();

    pardo(k, 0, R, 1) {
        last = psHisto[k] = myHisto[k];
        for (j=1 ; j<THREADS ; j++) {
            temp = psHisto[j*R + k] = last + myHisto[j*R +  k];
            last = temp;
        }
    }

    allHisto = psHisto+(THREADS-1)*R;

    node_Barrier();

    offset = 0;

    mps = psHisto + (MYTHREAD*R);
    for (k=0 ; k<R ; k++) {
        mhp[k]  = (mps[k] - mhp[k]) + offset;
        offset += allHisto[k];
    }

    node_Barrier();

    pardo(k, 0, q, 1) {
        j = bits(KEY(inArr->at(k)),bitOff,m);   ///////// same as above
        lSorted[mhp[j]] = inArr[k];
        mhp[j]++;
    }

    node_Barrier();

    node_free(psHisto, TH);
    node_free(myHisto, TH);
}



/****************************************************/
void all_radixsort_smp_s2(int q,
        DATA *inArr,
        DATA *lSorted,
        THREADED)
    /****************************************************/
{
    DATA *lTemp;

    lTemp = (DATA *)node_malloc(q*sizeof(DATA), TH);

    all_countsort_smp(q, inArr,   lTemp,   (1<<16),  0, 16, TH);
    all_countsort_smp(q, lTemp,   lSorted, (1<<4), 16, 4, TH);

    node_free(lTemp, TH);
}

/****************************************************/
