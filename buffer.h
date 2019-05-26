typedef struct idata {
	char *line;
	char *f_name;
	int b_off;
	int len;
}idata;
typedef struct inode {
	idata d;
	struct inode *next;
}inode;
typedef struct ilist {
	inode *head;
	inode *tail;
	inode *tr;
}ilist;
void oinit(ilist *l);
void opinit(ilist *l);
void oinsert(ilist *l, idata d);
idata *oprovide(ilist *l);
void odestroy(ilist *l);
void oprintlist(ilist *l);
