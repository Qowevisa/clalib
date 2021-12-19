#include <stdio.h>
#include "../inc/numctl.h"
#include "./container.c"

// gets bit from least important to most
#define GET_BIT(num, n) ((num) & (0x1 << (n)))
#define SET_BIT(num, n, val) ((num) |= ((0x1 & (val)) << (n)))

// macro to secure everything
#define SECURE(status, fun)    \
        {status = (fun);      \
            if (status != 0) { \
                return status; \
            }                  \
        }

// macros for bits for info
#define INIT_BIT 0

//
#define INIT_CHECK(bn, name)                                 \
        {if (GET_BIT(bn->info, INIT_BIT) == 0) {             \
                fprintf(stderr,                              \
                "Number is not initialized in %s\n", #name); \
                return 1;                                    \
            }                                                \
        }

int status = 0;

static int number_init(BigNumber *bn) {
    bn->info = 0;
    if (clanumber_init(&bn->num) != 0) {
        fprintf(stderr, "Unexpected behavior in number_init();\n");
        return 1;
    }
    SET_BIT(bn->info, INIT_BIT, 1);
    return 0;
}

int get_number_from_string(BigNumber *bn, const char *str) {
    // init block
    if (GET_BIT(bn->info, INIT_BIT) == 1) {
        fprintf(stderr, "Number is not empty in get_number_from_string();\n");
        return 1;
    } else {
        SECURE(status, number_init(bn));
    }
    //
    size_t i = 0;
    while (str[i] != '\0') {
        SECURE(status, append_digit(&bn->num, str[i]));
        i++;
    }
    return 0;
}

int print_number(BigNumber *bn) {
    INIT_CHECK(bn, print_number);
#ifdef DEBUG
    printf("in print_number():\n");
    printf("DEBUG_INFO       :\n");
    printf("BN_info          : %u\n", bn->info);
    printf("NUM_NODES_N      : %u\n", bn->num.nodes_n);
    printf("NUM_DIGITS_N     : %u\n\n", bn->num.digits_n);
    printf("HEAD_INFO        :\n");
    printf("HEAD             : %p\n", (void*)bn->num.head);
    printf("HEAD_PREV        : %p\n", (void*)bn->num.head->prev);
    printf("HEAD_NEXT        : %p\n\n", (void*)bn->num.head->next);
#endif
    seek_tail_num(bn->num);
    for (unsigned int i = 0; i < bn->num.nodes_n; i++) {
        SECURE(status, print_clachunk(curr_chunk(bn->num)));
        seek_prev_num(bn->num);
    }
    fputc('\n', stdout);
    return 0;
}
