#include <stdio.h>
#include <stdlib.h>
#include "../inc/container.h"

#define RPOS(pos) (CHUNK_SIZE - 1 - (pos))

static int clanumber_init(clanumber *cnum) {
    cnum->head = NULL;
    cnum->curr = NULL;
    cnum->tail = NULL;
    cnum->nodes_n = 0;
    cnum->digits_n = 0;
    return 0;
}

#define LH_BIT 1 << 31

static void append_digit_ch(clachunk *ch, const char c) {
    if (ch->info & LH_BIT) {
        // high
        ch->chunk[RPOS(ch->pos)] |= (c - '0') << 4;
        ch->info -= LH_BIT;
        ch->pos++;
    } else {
        // low
        ch->chunk[RPOS(ch->pos)] |= c - '0';
        ch->info += LH_BIT;
    }
    ch->len++;
}

static int append_digit(clanumber *cnum, const char c) {
    if (c < '0' || c > '9') {
        fprintf(stderr, "Invalid digit in append_digit\n");
        return 1;
    }
    if (cnum->tail == NULL) {
        cnum->tail = (clanode*)malloc(sizeof(clanode));
        cnum->nodes_n++;
        cnum->tail->prev = NULL;
        cnum->tail->next = NULL;
        cnum->tail->data = (clachunk*)malloc(sizeof(clachunk));
        cnum->head = cnum->tail;
        cnum->curr = cnum->tail;
        //
        clachunk *ch = cnum->curr->data;
        ch->len = 0;
        ch->pos = 0;
        ch->info = 0;
        append_digit_ch(ch, c);
        cnum->digits_n++;
    } else {
        if (cnum->digits_n == 2*CHUNK_SIZE*cnum->nodes_n) {
            cnum->head->prev = (clanode*)malloc(sizeof(clanode));
            cnum->nodes_n++;
            cnum->head->prev->next = cnum->head->prev;
            cnum->head->prev->prev = NULL;
            cnum->head->prev->data = (clachunk*)malloc(sizeof(clachunk));
            cnum->head = cnum->head->prev;
            cnum->curr = cnum->head;
            //
            clachunk *ch = cnum->curr->data;
            ch->len = 0;
            ch->pos = 0;
            ch->info = 0;
            append_digit_ch(ch, c);
            cnum->digits_n++;
        } else {
            clachunk *ch = cnum->curr->data;
            append_digit_ch(ch, c);
            cnum->digits_n++;
        }
    }
    return 0;
}

//

static int print_clachunk(clachunk *ch) {
#ifdef DEBUG
    printf("\n");
    printf("in print_clachunk:\n");
    printf("DATA_INFO        :\n");
    printf("DATA_POS         : %u\n", ch->pos);
    printf("DATA_LEN         : %u\n", ch->len);
    printf("DATA_INFO        : %u\n\n", ch->info);
#endif
    if (ch->info & LH_BIT) {
        ch->info -= LH_BIT;
    }
    for (unsigned int i = 0; i < ch->len; i++) {
        if (ch->info & LH_BIT) {
            // high
            fputc((ch->chunk[RPOS(i / 2)] >> 4) + '0', stdout);
            ch->info -= LH_BIT;
        } else {
            // low
            fputc((ch->chunk[RPOS(i / 2)] & 0xF) + '0', stdout);
            ch->info += LH_BIT;
        }
    }
    return 0;
}

#define seek_head_num(cnum) (cnum.curr = cnum.head)

#define seek_next_num(cnum) (cnum.curr = cnum.curr->next)

#define seek_prev_num(cnum) (cnum.curr = cnum.curr->prev)

#define seek_tail_num(cnum) (cnum.curr = cnum.tail)

#define curr_chunk(cnum) (cnum.curr->data)
