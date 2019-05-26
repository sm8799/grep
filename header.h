typedef struct declaration {
	int iden, stp, flag_m, ctr, c_count[10000], c_c, rev, flag_c, j;
}declaration;

void init_buffer_source();
void addinsource();
void addinbuffer();
void printcount();
void printoutput();
void destroy_buffer_source();
int grep(idata d, char *str);
int iwgrep(idata d, char *str);
int igrep(idata d, char *str);
int wgrep(idata d, char *str);
int checkforregex(char *string);
