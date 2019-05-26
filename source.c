#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "source.h"
char *ptr1;
char *ptr2;
void dinit(list *l) {
        l -> head = l -> tail = l -> tr = NULL; 
}
void dpinit(list *l) {
	if(l -> tr == l -> tail)
		l -> head = l -> tr;
	l -> tr = l -> tr -> next;
}
void dinsert(list *l, char *str) {
        node *tmp;
        tmp = (node *)malloc(sizeof(node));
        tmp -> str = (char *)malloc(strlen(str) + 1);
        strcpy(tmp -> str, str);
        if(l -> head == NULL) {
                l -> head = l -> tail = tmp;
                l -> head -> next = NULL;
                l -> tr = l -> head;
                l -> tr -> next = NULL;
                return;
        }
        l -> tail -> next = tmp;
        l -> tail = tmp;
        l -> tail -> next = NULL;
}
char *dprovide(list *l) {
	if(l -> tr == NULL) {
		ptr2 = NULL;
                return ptr2;
	}
	ptr1 = (char *)malloc(strlen(l -> tr -> str) + 1);
        strcpy(ptr1, l -> tr -> str);
        return ptr1;
}
void ddestroy(list *l) {
        while( l -> head != NULL) {
                node *q;
                q = l -> head;
                l -> head = l -> head -> next;
                free(q -> str);
                free(q);
                q = NULL;
        }
}
void dprintlist(list *l) {
	node *tmp;
	tmp = l -> head;
	while(tmp != NULL) {
		printf("%s\n", tmp -> str);
		tmp = tmp -> next;
	}
}
void finit(list *l) {
        l -> head = l -> tail = l -> tr = NULL; 
}
void fpinit(list *l) {
	l -> tr = l -> tr -> next;
}
void initz(list *l) {
	l -> tr = l -> head;
}
void finsert(list *l, char *str) {
        node *tmp;
        tmp = (node *)malloc(sizeof(node));
        tmp -> str = (char *)malloc(strlen(str) + 1);
        strcpy(tmp -> str, str);
        if(l -> head == NULL) {
                l -> head = l -> tail = tmp;
                l -> head -> next = NULL;
                l -> tr = l -> head;
                l -> tr -> next = NULL;
                return;
        }
        l -> tail -> next = tmp;
        l -> tail = tmp;
        l -> tail -> next = NULL;
}
char *fprovide(list *l) {
	if(l -> tr == NULL) {
		ptr2 = NULL;
                return ptr2;
	}
	ptr1 = (char *)malloc(strlen(l -> tr -> str) + 1);
        strcpy(ptr1, l -> tr -> str);
        return ptr1;
}
void fdestroy(list *l) {
        while( l -> head != NULL) {
                node *q;
                q = l -> head;
                l -> head = l -> head -> next;
                free(q -> str);
                free(q);
                q = NULL;
        }
}
void fprintlist(list *l) {
	node *tmp;
	tmp = l -> head;
	while(tmp != NULL) {
		printf("%s\n", tmp -> str);
		tmp = tmp -> next;
	}
}
void pinit(list *l) {
        l -> head = l -> tail = l -> tr = NULL; 
}
void pinsert(list *l, char *str) {
        node *tmp;
        tmp = (node *)malloc(sizeof(node));
        tmp -> str = (char *)malloc(strlen(str) + 1);
        strcpy(tmp -> str, str);
        if(l -> head == NULL) {
                l -> head = l -> tail = tmp;
                l -> head -> next = NULL;
                l -> tr = l -> head;
                l -> tr -> next = NULL;
                return;
        }
        l -> tail -> next = tmp;
        l -> tail = tmp;
        l -> tail -> next = NULL;
}
char *pprovide(list *l) {
	if(l -> tr == NULL) {
		ptr2 = NULL;
                return ptr2;
	}
	ptr1 = (char *)malloc(strlen(l -> tr -> str) + 1);
        strcpy(ptr1, l -> tr -> str);
        return ptr1;
}
void init(list *l) {
	l -> tr = l -> head;
}
void ppinit(list *l) {
	l -> tr = l -> tr -> next;
}
void pdestroy(list *l) {
        while( l -> head != NULL) {
                node *q;
                q = l -> head;
                l -> head = l -> head -> next;
                free(q -> str);
                free(q);
                q = NULL;
        }
}
void pprintlist(list *l) {
	node *tmp;
	tmp = l -> head;
	while(tmp != NULL) {
		printf("%s\n", tmp -> str);
		tmp = tmp -> next;
	}
}
