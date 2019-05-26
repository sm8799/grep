#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "buffer.h"
idata *ptr1;
idata *ptr2;
void oinit(ilist *l) {
        l -> head = l -> tail = l -> tr = NULL; 
}
void opinit(ilist *l) {
	l -> tr = l -> tr -> next;
}
void oinsert(ilist *l, idata d) {
        inode *tmp;
        int i = 0;
        tmp = (inode *)malloc(sizeof(inode));
        tmp -> d.line = (char *)malloc(sizeof(char)*(d.len + 1));
        strcpy(tmp -> d.line, d.line);
        tmp -> d.len = d.len;
        tmp -> d.b_off = d.b_off;
        tmp -> d.f_name = (char *)malloc(strlen(d.f_name) + 1);
        strcpy(tmp -> d.f_name, d.f_name);
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
idata *oprovide(ilist *l) {
	int i;
	if(l -> tr == NULL) {
		ptr2 = NULL;
                return ptr2;
	}
	ptr1 = (idata *)malloc(sizeof(idata));
	ptr1 -> line = (char *)malloc(sizeof(char)*(l -> tr -> d.len + 1));
	strcpy(ptr1 -> line, l -> tr -> d.line);
	ptr1 -> f_name = (char *)malloc(strlen(l -> tr -> d.f_name));
	strcpy(ptr1 -> f_name, l -> tr -> d.f_name);
	ptr1 -> b_off = l -> tr -> d.b_off;
	ptr1 -> len = l -> tr -> d.len;
        return ptr1;
}
void odestroy(ilist *l) {
        while( l -> head != NULL) {
                inode *q;
                q = l -> head;
                l -> head = l -> head -> next;
                free(q -> d.line);
                free(q -> d.f_name);
                free(q);
                q = NULL;
        }
}
void oprintlist(ilist *l) {
	inode *tmp;
	tmp = l -> head;
	while(tmp != NULL) {
		printf("%s:%d:len = %d:%s:", tmp -> d.f_name, tmp -> d.b_off, tmp -> d.len,  tmp -> d.line);
		tmp = tmp -> next;
	}
}
