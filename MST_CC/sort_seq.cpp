#include "grph.hpp"
//#include "../ESSENS/Core/Basic_IO/Format/Level0/ADJ/network_defs.hpp"
#include "../ESSENS/Core/Basic_IO/Format/Level0/structure_defs.hpp"
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <strings.h>
#include <values.h>
}


//typedef vector <int_double> DATA_TYPE;

//std::pair<int, double> MAX_VAL;
//std::pair<int, double> MAX_VAL_m1;

//MAX_VAL = { MAXINT, MAXINT};
//MAX_VAL_m1 = { MAXINT-1, MAXINT-1};

//MAX_VAL.first = MAXINT;
//MAX_VAL.second = MAXINT;
//MAX_VAL_m1.first = MAXINT-1;
//MAX_VAL_m1.second = MAXINT-1;

void insertsort(vector<int_double> A, int n) {

    std::pair<int, double> MAX_VAL;
    std::pair<int, double> MAX_VAL_m1;

    MAX_VAL = { MAXINT, MAXINT};
    MAX_VAL_m1 = { MAXINT-1, MAXINT-1};

    register std::pair<int, double> item;
    register int i,j;

    for (i=1 ; i<n ; i++) {
        item = A[i];
        j = i-1;
        while ((j>=0)&&(item.first < A[j].first || (item.first==A[j].first && item.second<A[j].second))) {
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = item;
    }
}
/*
vector<int_double> * mergesort_nr_2(vector<int_double> *Buffer1,
        vector<int_double> *Buffer2,
        int col_size) {


    int
        i, j, k,
    values, times,
    adjustments, last1, last2,
    tot, tot1, tot2,
    rem;

    std::pair<int, double>
        *start_ptr,
        *finish_ptr,
        *temp_ptr,
        *ex_ptr,
        *s1_ptr, *s2_ptr;

    start_ptr = Buffer1[0];
    finish_ptr = Buffer2[1];

    std::pair<int, double> MAX_VAL;
    std::pair<int, double> MAX_VAL_m1;

    MAX_VAL = { MAXINT, MAXINT};
    MAX_VAL_m1 = { MAXINT-1, MAXINT-1};

    times = (int) ceil(log((double) col_size)/log((double) 2));

    adjustments = 0;
    values = col_size;

    if (col_size & 1)
        temp_ptr = start_ptr + col_size - 1;
    else
        temp_ptr = start_ptr + col_size;
    while (start_ptr < temp_ptr) {
        if ((*start_ptr).first > (*(start_ptr+1)).first || ((*start_ptr).first == (*(start_ptr+1)).first) && (*start_ptr).second > (*(start_ptr+1)).second) {
            *(finish_ptr++) = *(start_ptr+1);
            *(finish_ptr) = *(start_ptr);
            if ((*(finish_ptr++)).first > MAX_VAL_m1.first) {
                adjustments++;
                (*(finish_ptr - 1)) = MAX_VAL_m1;
            }
            *(finish_ptr++) = MAX_VAL;
            start_ptr+=2;
        }
        else {
            *(finish_ptr++) = *(start_ptr++);
            *(finish_ptr) = *(start_ptr++);
            if ((*(finish_ptr++)).first > MAX_VAL_m1.first) {
                adjustments++;
                (*(finish_ptr-1)) = MAX_VAL_m1;
                if ((*(finish_ptr - 2)).first > MAX_VAL_m1.first) {
                    adjustments++;
                    (*(finish_ptr-2)) = MAX_VAL_m1;
                }
            }

            *(finish_ptr++) = MAX_VAL;
        }
    }


    last1 = last2 = 2;

    if (col_size & 1) {
        *(finish_ptr) = *(temp_ptr);
        if ((*(finish_ptr++)).first > MAX_VAL_m1.first) {
            adjustments++;
            (*(finish_ptr-1)) = MAX_VAL_m1;
        }
        *(finish_ptr) = MAX_VAL;
        last2 = 1;
    }

    ex_ptr = Buffer1;
    Buffer1 = Buffer2;
    Buffer2 = ex_ptr;

    tot = 2;
    tot1 = 1;
    tot2 = col_size >> 1;

    for (i=2;i<times;i++) {
        start_ptr = Buffer1;
        finish_ptr = Buffer2;
        tot <<= 1;
        tot1 <<= 1;
        tot2 >>= 1;
        for (j=1;j<tot2;j++) {
            s2_ptr = (s1_ptr = (start_ptr)) + tot1 + 1;
            temp_ptr = finish_ptr + tot;
            while (finish_ptr < temp_ptr) {
                if ((*s1_ptr).first < (*s2_ptr).first || (((*s1_ptr).first == (*s2_ptr).first )&& ((*s1_ptr).second < (*s2_ptr).second)))
                    *(finish_ptr++) = *(s1_ptr++);
                else
                    *(finish_ptr++) = *(s2_ptr++);
            }
            *(finish_ptr++) = MAX_VAL;
            start_ptr += (tot + 2);
        }

        rem = (values & (tot - 1));

        if (! rem) {
            s2_ptr = (s1_ptr = (start_ptr)) + tot1 + 1;
            temp_ptr = finish_ptr + tot;
            while (finish_ptr < temp_ptr) {
                if ((*s1_ptr).first < (*s2_ptr).first || (((*s1_ptr).first == (*s2_ptr).first) && ((*s1_ptr).second < (*s2_ptr).second)))
                    *(finish_ptr++) = *(s1_ptr++);
                else
                    *(finish_ptr++) = *(s2_ptr++);
            }
            *(finish_ptr++) = MAX_VAL;
            last1 = last2 = tot;
        }
        else {
            if (rem <= tot1) {
                s1_ptr = start_ptr;
                for (k=0;k<=tot1;k++)
                    *(finish_ptr + k) = *(s1_ptr + k);
                finish_ptr += (tot1 + 1);
                start_ptr += (tot1 + 1);
                s2_ptr = (s1_ptr = (start_ptr)) + last1 + 1;
                temp_ptr = finish_ptr + last1 + last2;
                while (finish_ptr < temp_ptr) {
                    if (((*s1_ptr).first < (*s2_ptr).first) || (((*s1_ptr).first == (*s2_ptr).first) &&( (*s1_ptr).second < (*s2_ptr).second)))
                        *(finish_ptr++) = *(s1_ptr++);
                    else
                        *(finish_ptr++) = *(s2_ptr++);
                }
                *(finish_ptr++) = MAX_VAL;
                last2 += last1;
                last1 = tot1;
            }
            else {
                s2_ptr = (s1_ptr = (start_ptr)) + tot1 + 1;
                temp_ptr = finish_ptr + tot;
                while (finish_ptr < temp_ptr) {
                    if (((*s1_ptr).first < (*s2_ptr).first) || (((*s1_ptr).first == (*s2_ptr).first )&&( (*s1_ptr).second < (*s2_ptr).second)))
                        *(finish_ptr++) = *(s1_ptr++);
                    else
                        *(finish_ptr++) = *(s2_ptr++);
                }
                *(finish_ptr++) = MAX_VAL;
                start_ptr += (tot + 2);
                s2_ptr = (s1_ptr = (start_ptr)) + last1 + 1;
                temp_ptr = finish_ptr + last1 + last2;
                while (finish_ptr < temp_ptr) {
                    if (((*s1_ptr).first < (*s2_ptr).first) || (((*s1_ptr).first == (*s2_ptr).first) && ((*s1_ptr).second < (*s2_ptr).second)))
                        *(finish_ptr++) = *(s1_ptr++);
                    else
                        *(finish_ptr++) = *(s2_ptr++);
                }
                *(finish_ptr++) = MAX_VAL;
                last2 += last1;
                last1 = tot;
            }
        }
        ex_ptr = Buffer1;
        Buffer1 = Buffer2;
        Buffer2 = ex_ptr;
    }

    start_ptr = Buffer1;
    finish_ptr = Buffer2;
    s2_ptr = (s1_ptr = (start_ptr)) + last1 + 1;
    temp_ptr = finish_ptr + last1 + last2;

    while (finish_ptr < temp_ptr) {
        if ((*s1_ptr).first < (*s2_ptr).first || ((*s1_ptr).first == (*s2_ptr).first && (*s1_ptr).second < (*s2_ptr).second))
            *(finish_ptr++) = *(s1_ptr++);
        else
            *(finish_ptr++) = *(s2_ptr++);
    }

    finish_ptr = Buffer2 + values;;
    temp_ptr = finish_ptr - adjustments;

    while ((--finish_ptr) >= temp_ptr)
        *finish_ptr =  MAX_VAL;

    finish_ptr = Buffer2;

    return(finish_ptr);

}

void mergesort_nr(vector<int_double> *A, int elems) {
    vector<int_double> *finlist, *B;
    std::pair<int, double> MAX_VAL;
    std::pair<int, double> MAX_VAL_m1;

    MAX_VAL = { MAXINT, MAXINT};
    MAX_VAL_m1 = { MAXINT-1, MAXINT-1};

    B = (vector<int_double> *)malloc(2*elems*sizeof(vector<int_double>));
    assert_malloc(B);

    finlist = mergesort_nr_2(A, B, elems);
    if (finlist == B)
        bcopy(B,A,elems*sizeof(vector<int_double>));

    free(B);

}
*/
