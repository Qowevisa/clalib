#ifndef __clalib_numctl_h
#define __clalib_numctl_h
#include "./container.h"

typedef struct big_number {
    clanumber num;
    unsigned int info;
} BigNumber;

int get_number_from_string(BigNumber *bn, const char *str);

int print_number(BigNumber *bn);

int add_string_to_number(BigNumber *bn, const char *str);

#endif //__clalib_numctl_h
