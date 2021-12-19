#ifndef __clalib_container_h
#define __clalib_container_h

typedef unsigned char td_t;

#define CHUNK_SIZE 500

typedef struct clachunk {
  td_t chunk[CHUNK_SIZE];
  unsigned int info;
  unsigned int len;
  unsigned int pos;
} clachunk;

typedef struct clanode {
    struct clanode  *prev;
    struct clanode  *next;
    struct clachunk *data;
} clanode;

typedef struct clanumber {
    clanode *head;
    clanode *curr;
    clanode *tail;
    unsigned int nodes_n;
    unsigned int digits_n;
} clanumber;

#endif //__clalib_container_h
