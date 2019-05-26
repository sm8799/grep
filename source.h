typedef struct node {
        char *str;
        struct node *next;
}node;
typedef struct list {
        node *head;
        node *tail;
        node *tr;
}list;
void dinit(list *l);
void dinsert(list *l, char *str);
char *dprovide(list *l);
void dpinit(list *l);
void ddestroy(list *l);
void dprintlist(list *l);
void finit(list *l);
void initz(list *l);
void finsert(list *l, char *str);
char *fprovide(list *l);
void fpinit(list *l);
void fdestroy(list *l);
void fprintlist(list *l);
void pinit(list *l);
void pinsert(list *l, char *str);
char *pprovide(list *l);
void ppinit(list *l);
void pdestroy(list *l);
void pprintlist(list *l);
void init(list *l);
