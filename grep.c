#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<dirent.h>
#include "buffer.h" 		// for output
#include "source.h"  		// for patterns, files and directories
#include "header.h" 		// for function, variables in the same program
declaration dc; 		// declaration for some of the global variables
list pl, fl, dl; 		// required list for pattern, file and directory
ilist ol; 			// list for maintaining output
static int fc = 0; 		// to count number of files
char str[11]; 			// used as a requirement for getopt
int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("grep:invalid arguements\n");
		exit(0);
	}
	init_buffer_source(); 		// to initialise all sorts of list
	addinsource(argc, argv);	 // files, patterns and directories if any, are added here
	addinbuffer(); 			// output which my program shows is get added here
	initz(&fl);			// files are initialised
	if(dc.flag_c == 1) {
		printcount(); 		// to print count if demanded 
		destroy_buffer_source();// to destroy all sorts of list
		return 0;
	}
	printoutput();
	destroy_buffer_source(); 
	return 0;
}
void init_buffer_source() {
	oinit(&ol);
	finit(&fl);
	pinit(&pl);
	dinit(&dl);
}
void addinsource(int argc, char *argv[]) {
	DIR *dr, *df, *dk;
	struct dirent *de;
	int opt, fd, fd1, p, x, index;
	char pat_file[40], ch, *z, *tmp, *tmp1, haha[2], pattern[400];
	tmp = (char *)malloc(1000000);
	tmp1 = (char *)malloc(1000000);
	haha[0] ='/';
	haha[1] = '\0';
	dc.stp = 0; dc.flag_m = -1; dc.rev = 0; dc.flag_c = 0;
	for(dc.j = 0; dc.j < 11; dc.j++) {
		str[dc.j] = '\0';
	}
	while((opt = getopt(argc, argv, ":ivwqchHbm:e:f:r")) != -1) {
		switch(opt) {
			case 'i':
				str[2] = 'i';
				break;
			case 'e':
				str[10] = 'e';
				strcpy(pattern, optarg);
				dc.iden = checkforregex(pattern);
				pinsert(&pl, pattern);
				break;
			case 'w':
				str[3] = 'w';
				break;
			case 'v':
				dc.rev = 1;
				str[4] = 'v';
				break;
			case 'q':
				exit(1);
				break;
			case 'c':
				dc.flag_c = 1;
				str[8] = 'c';
				break;
			case 'h':
				str[5] = 'h';
				break;
			case 'H':
				str[5] = 'H';
				break;
			case 'b':
				str[6] = 'b';
				break;
			case 'm':
				dc.stp = 1;
				str[7] = 'm';
				dc.ctr = atoi(optarg);
				break;
			case 'r':
				str[0] = 'r';
				break;  
			case 'f':
				str[9] ='f';
				strcpy(pat_file, optarg);
				dr = opendir(pat_file);
				if(dr != NULL) {
					printf("GREP: %s is a Directory\n", pat_file);
					exit(1);
				}
				fd = open(pat_file, O_RDONLY);
				if(fd == -1) {
					printf("GREP: is %s a file ?\n", pat_file);
					exit(1);
				}
				while(1) {
					p = 0;
					while(x = read(fd, &ch, 1)){
						pattern[p] =ch;
						p++;
						if(ch == '\n') {
							pattern[--p] = '\0';
							break;
						}
					}
					if(p != 0) {
						dc.iden = checkforregex(pattern);//check for regular expession
						pinsert(&pl, pattern);
					}
					if(x == 0)
						break;
				}
				close(fd);
				break;
			case ':':
				printf("option needs a value\n");  
				break;  
			case '?':  
				printf("unknown option: %c\n", optopt); 
				break;		    
		}
	}
	if(str[10] != 'e' && str[9] != 'f'&& argc != optind) {
		strcpy(pattern, argv[optind]);
		optind++;
		dc.iden = checkforregex(pattern);// check for regular expressions
		pinsert(&pl, pattern);
	}
	if(str[0] == 'r' && argc == optind) {
		if((dr = opendir(".")) != NULL) {
			closedir(dr);
			dinsert(&dl, ".");
			while(1) {
				z = dprovide(&dl);
				if(z == NULL) {
					break;
				}
				dr = opendir(z);
				if(dr == NULL) {
					break;
				}
				while((de = readdir(dr)) != NULL) {
					if((strcmp(de -> d_name, ".")) == 0 || 
							(strcmp(de -> d_name, "..")) == 0) {
						continue;
					}
					strcpy(tmp, z);
					strcat(tmp, haha);
					strcat(tmp, de -> d_name);
					df = opendir(tmp);
					if(df == NULL) {
						fd1 = open(tmp, O_RDONLY);
						if(fd1 == -1) {
							close(fd1);
							continue;
						}
						finsert(&fl, tmp);
						fc++;
						closedir(df);
						close(fd1);
						continue;
					}
					else {
						strcpy(tmp1, z);
						strcat(tmp1, haha);
						strcat(tmp1, de -> d_name);
						dinsert(&dl, tmp1);
					}
					tmp[0] = '\0';
					tmp1[0] ='\0';
					closedir(df);
				}
				dpinit(&dl);
				closedir(dr);
			}
		}
	}
	for(index = optind; index < argc; index++) {
		if(str[0] == 'r') {
			if((dr = opendir(argv[index])) != NULL) {
				closedir(dr);
				dinsert(&dl, argv[index]);
				while(1) {
					z = dprovide(&dl);
					if(z == NULL) {
						break;
					}
					dr = opendir(z);
					if(dr == NULL) {
						break;
					}
					while((de = readdir(dr)) != NULL) {
						if((strcmp(de -> d_name, ".")) == 0 || 
								(strcmp(de -> d_name, "..")) == 0) {
							continue;
						}
						strcpy(tmp, z);
						strcat(tmp, haha);
						strcat(tmp, de -> d_name);
						df = opendir(tmp);
						if(df == NULL) {
							fd1 = open(tmp, O_RDONLY);
							if(fd1 == -1) {
								close(fd1);
								continue;
							}
							finsert(&fl, tmp);
							fc++;
							closedir(df);
							close(fd1);
							continue;
						}
						else {
							strcpy(tmp1, z);
							strcat(tmp1, haha);
							strcat(tmp1, de -> d_name);
							dinsert(&dl, tmp1);
						}
						tmp[0] = '\0';
						tmp1[0] ='\0';
						closedir(df);
					}
					dpinit(&dl);
					closedir(dr);
				}
			}
			else if((fd = open(argv[index], O_RDONLY)) != -1) {
				finsert(&fl, argv[index]);
				fc++;
			}
			else
				printf("Unknown %s\n", argv[index]);
		}
		else if((dr = opendir(argv[index])) != NULL) {
			printf("folder %s is a directory \n", argv[index]);
			closedir(dr);
		}
		else if((fd = open(argv[index], O_RDONLY)) != -1) {
			finsert(&fl, argv[index]);
			fc++;
			close(fd);
		}
	}
	free(tmp);
	free(tmp1);
}
void addinbuffer() {
	int fd, size = 128, i, x, imp = 0, c_c_c = 0, b = 0;
	char *fn, ch, *PATTERN;
	idata d;
	dc.c_c = 0;
	while((fn = fprovide(&fl)) != NULL) { //fprovide provides file name in list
		fd = open(fn, O_RDONLY);
		if(fd == -1) {
			printf("%s not a file\n", fn);
			break;
		}
		if(dc.stp == 1) {
			dc.flag_m = dc.ctr;
		}
		while(1) {
			d.line = (char *)malloc(sizeof(char) * size);
			i = 0;
			d.b_off = b;
			while((x = read(fd, &ch, 1))) {
				b++;
				d.line[i] = ch;
				i++;
				if(ch == '\n' || ch == '\0')
					break;
				if(size == i) {
					size = size * 2;
					d.line = (char *)realloc(d.line, size);
				}
			}
			d.line[i] = '\0';
			if(x == 0 && i == 0)
				break;
			d.len = i;
			d.f_name = (char *)malloc(strlen(fn) + 1);
			strcpy(d.f_name, fn);
			if(str[2] == 'i' && str[3] == 'w') {
				while((PATTERN = pprovide(&pl)) != NULL) { //pprovide provides patterns one by one
					if(iwgrep(d,PATTERN)) //for both -i and -w
						imp = 1;
					ppinit(&pl); // pattern shifts one node forward
				}
				if(dc.rev == 1 && imp != 1) {
					oinsert(&ol, d); // inserting in a output list
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev != 1 && imp != 1) {
					;
				}
				else if(dc.rev != 1 && imp == 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev == 1 && imp == 1) {
					imp = 0;;
				}
			}
			else if(str[2] != 'i' && str[3] != 'w') {
				while((PATTERN = pprovide(&pl)) != NULL) {
					if(grep(d,PATTERN)) // for no -i & -w
						imp = 1;
					ppinit(&pl);
				}
				if(dc.rev == 1 && imp != 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev != 1 && imp != 1) {
					;
				}
				else if(dc.rev != 1 && imp == 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev == 1 && imp == 1) {
					imp = 0;
				}
			}
			else if(str[2] != 'i' && str[3] == 'w') {
				while((PATTERN = pprovide(&pl)) != NULL) {
					if(wgrep(d,PATTERN)) // for -w only
						imp = 1;
					ppinit(&pl);
				}
				if(dc.rev == 1 && imp != 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev != 1 && imp != 1) {
					;
				}
				else if(dc.rev != 1 && imp == 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev == 1 && imp ==1) {
					imp = 0;;
				}
			}
			else if(str[2] == 'i' && str[3] != 'w') {
				while((PATTERN = pprovide(&pl)) != NULL) {
					if(igrep(d,PATTERN)) // for -i only
						imp = 1;
					ppinit(&pl);
				}
				if(dc.rev == 1 && imp != 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev != 1 && imp != 1) {
					;
				}
				else if(dc.rev != 1 && imp == 1) {
					oinsert(&ol, d);
					imp = 0;
					c_c_c++;
					if(dc.stp == 1) {
						dc.flag_m--;
						if(dc.flag_m == 0) {
							break;
						}
					}
				}
				else if(dc.rev == 1 && imp ==1) {
					imp = 0;
				}
			}
			free(d.line);
			d.line = NULL;
			free(d.f_name);
			d.f_name = NULL;
			init(&pl); // initialises the pattern list to first pattern
			if(dc.flag_m == 0) {
				break;
			}
			if(x == 0) {
				break;
			}
			size = 128;
		}
		b = 0;
		dc.c_count[dc.c_c] = c_c_c;
		dc.c_c++;
		c_c_c = 0;
		fpinit(&fl);//shifts file to next node
	}
}
void printcount() {
	int cal = -1;
	char *fn;
	while(cal < dc.c_c) {
		fn = fprovide(&fl);
		if(fn != NULL && fc == 1 && str[5] != 'H') {
			cal++;
			printf("%d\n", dc.c_count[cal]);
			break;
		}
		else if(fn != NULL && fc == 1 && str[5] == 'H') {
			cal++;
			printf("%s:%d\n", fn, dc.c_count[cal]);
			break;
		}
		if(fn != NULL && fc != 1 && str[5] != 'h') {
			cal++;
			printf("%s:%d\n", fn, dc.c_count[cal]);
			fpinit(&fl);
		}
		else if(fn != NULL && fc != 1 && str[5] == 'h') {
			cal++;
			printf("%d\n", dc.c_count[cal]);
			fpinit(&fl);
		}
		else
			break;
	}
}
void printoutput() {
	char bin_file[128];
	idata *d1;
	while((d1 = oprovide(&ol)) != NULL) {
		if(strlen(d1 -> line) < d1 -> len) {
			if(strcmp(bin_file, d1 -> f_name) != 0) {
				strcpy(bin_file, d1 -> f_name);
				printf("Binary file %s matches\n", bin_file);
			}
			opinit(&ol);
			continue;
		}
		if(str[5] != 'h' && fc != 1 && str[6] != 'b') {
			printf("%s:%s", d1 -> f_name, d1 -> line);
		}
		else if(str[5] == 'h' && str[6] != 'b') {
			printf("%s", d1 -> line);
		}
		else if(str[5] == 'H' && fc == 1 && str[6] != 'b') {
			printf("%s:%s", d1 -> f_name, d1 -> line);
		}
		else if(str[5] == '\0' && fc == 1 && str[6] != 'b') {
			printf("%s", d1 -> line);
		}
		else if(str[5] != 'h' && fc != 1 && str[6] != 'b') {
			printf("%s:%d:%s", d1 -> f_name, d1 -> b_off, d1 -> line);
		}
		else if(str[5] == 'h' && str[6] == 'b') {
			printf("%d:%s", d1 -> b_off, d1 -> line);
		}
		else if(str[5] == 'H' && fc == 1 && str[6] == 'b') {
			printf("%s:%d:%s", d1 -> f_name, d1 -> b_off, d1 -> line);
		}
		else if(str[5] == '\0' && fc != 1 && str[6] == 'b') {
			printf("%s:%d:%s", d1 -> f_name, d1 -> b_off, d1 -> line);
		}
		else if(str[5] == '\0' && fc == 1 && str[6] == 'b') {
			printf("%d:%s", d1 -> b_off, d1 -> line);
		}
		opinit(&ol);
	}
}
void destroy_buffer_source() {
	odestroy(&ol);
	fdestroy(&fl);
	ddestroy(&dl);
	pdestroy(&pl);
}
int grep(idata d, char *str) {
	int i = 0, j = 0, l2, cnt = 0, count = 0, k;
	l2 = strlen(str);
	while(i <= d.len) {
		k = i;
		if(dc.iden == 2)
			i = d.len - l2 - 1;
		j = 0;
		while(j <= l2) {
			if(str[j] == d.line[i]) {
				i++;
				j++;
				cnt++;
				if (cnt == l2) {
					if(dc.iden == 1 && i > l2)
						break;
					count++;
					cnt = 0;
				}
			}
			else if(str[j] != d.line[i]) {
				if(cnt == l2) {
					cnt = 0;
					i = k + 1;
				}
				else if(cnt != l2) {
					cnt = 0;
					i = k + 1;
					break;
				}
			}
		}
		if((dc.iden == 1 && i > l2) || dc.iden == 2)
			break;
	}
	if(count > 0) {
		return 1;
	}
	return 0;
}
int iwgrep(idata d, char *str) {
	int i = 0, j = 0, l2, cnt = 0, count = 0, k;
	l2 = strlen(str);
	while(i <= d.len) {
		k = i;
		if(dc.iden == 2)
			i = d.len - l2 - 1;
		j = 0;
		while(j <= l2) {
			if((str[j] == d.line[i]) || ((str[j] - 32) == d.line[i]) || ((str[j] + 32) == d.line[i])) {
				i++;
				j++;
				cnt++;
				if (cnt == l2) {
					if(dc.iden == 1 && i > l2)
						break;
					count++;
					if((d.line[i] >= 65 && d.line[i] <= 90) || (d.line[i] >= 97 && d.line[i] <= 122)
							|| (d.line[i - l2 -1] >= 65 && d.line[i - l2 -1] <= 90) ||
							(d.line[i - l2 -1] >= 97 && d.line[i - l2 -1] <= 122) ||
							(d.line[i] == 95 || d.line[i - l2 -1] == 95) || 
							(d.line[i] >= 48 && d.line[i] <= 57)||
							(d.line[i - l2 -1] >= 48 && d.line[i - l2 -1] <= 57)) {
						count--;
						cnt = 0;
					}
					cnt = 0;
				}
			}
			else if(str[j] != d.line[i]) {
				if(cnt == l2) {
					cnt = 0;
					i = k + 1;
				}
				else if(cnt != l2) {
					cnt = 0;
					i = k + 1;
					break;
				}				
			}
		}
		if((dc.iden == 1 && i > l2) || dc.iden == 2)
			break;
	}
	if(count > 0) {
		return 1;
	}
	return 0;
}
int igrep(idata d, char *str) {
	int i = 0, j = 0, l2, cnt = 0, count = 0, k;
	l2 = strlen(str);
	while(i <= d.len) {
		k = i;
		if(dc.iden == 2)
			i = d.len - l2 - 1;
		j = 0;
		while(j <= l2) {
			if((str[j] == d.line[i]) || ((str[j] - 32) == d.line[i]) || ((str[j] + 32) == d.line[i])) {
				i++;
				j++;
				cnt++;
				if (cnt == l2) {
					if(dc.iden == 1 && i > l2)
						break;
					count++;
					cnt = 0;
				}
			}
			else if(str[j] != d.line[i]) {
				if(cnt == l2) {
					cnt = 0;
					i = k + 1;
				}
				else if(cnt != l2) {
					cnt = 0;
					i = k + 1;
					break;
				}				
			}
		}
		if((dc.iden == 1 && i > l2) || dc.iden == 2)
			break;
	}
	if(count > 0) {
		return 1;
	}
	return 0;
}
int wgrep(idata d, char *str) {
	int i = 0, j = 0, l2, cnt = 0, count = 0, k;
	l2 = strlen(str);
	while(i <= d.len) {
		k = i;
		if(dc.iden == 2)
			i = d.len - l2 - 1;
		j = 0;
		while(j <= l2) {
			if(str[j] == d.line[i]) {
				i++;
				j++;
				cnt++;
				if (cnt == l2) {
					if(dc.iden == 1 && i > l2)
						break;
					count++;
					if((d.line[i] >= 65 && d.line[i] <= 90) || (d.line[i] >= 97 && d.line[i] <= 122)
							|| (d.line[i - l2 -1] >= 65 && d.line[i - l2 -1] <= 90) ||
							(d.line[i - l2 -1] >= 97 && d.line[i - l2 -1] <= 122) ||
							(d.line[i] == 95 || d.line[i - l2 -1] == 95) || 
							(d.line[i] >= 48 && d.line[i] <= 57)||
							(d.line[i - l2 -1] >= 48 && d.line[i - l2 -1] <= 57)) {
						count--;
						cnt = 0;
					}
					cnt = 0;
				}
			}
			else if(str[j] != d.line[i]) {
				if(cnt == l2) {
					cnt = 0;
					i = k + 1;
				}
				else if(cnt != l2) {
					cnt = 0;
					i = k + 1;
					break;
				}				
			}
		}
		if((dc.iden == 1 && i > l2) || dc.iden == 2)
			break;
	}
	if(count > 0) {
		return 1;
	}
	return 0;
}
enum states {SPC, BRA, LET};
int checkforregex(char *string) {
	int l, i = 0, q, j = 0, l1 = 0, m = 0, a, z = 1, y = 0, flag_dot = 0;
	char bracket[100][100],dub[10000][100], arr[20],arr1[2], str[50], sis[50], currchar, expr[200], pat[200], ch;
	enum states currstate = SPC;
	l = strlen(string);
	if(string[i] == '^') {
		while(string[i] != '\0') {
			string[i] = string[i+1];
			i++;
		}
		return 1;
	}
	else if(string[l - 1] == '$') {
		string[l - 1] = '\0';
		return 2;
	}
	else {
		for(i = 0; i < l; i++) {
			if(string[i] == '?') {
				q = i - 1;
				while(string[i] != '\0') {
					string[i] = string[i+1];
					i++;
				}
				pinsert(&pl, string);
				while(string[q] != '\0') {
					string[q] = string[q+1];
					q++;
				}
				break;
			}
			else if(string[i] == '[') {
				i  = 0;
				while(1) {
					currchar = string[i];
					if(currchar == '\0')
						break;
					switch(currstate) {
						case BRA:
							switch(currchar) {
								case '[':
									i++;
									break;
								case ']':
									currstate = LET;
									break;
								case '\0':
									i++;
									break;
								case '-':
									i++;
									ch = arr[m - 1];
									while(string[i] != ch) {
										arr[m] = ++ch;
										m++;
									}
									i++;
									currstate = BRA;
									break;
								default:
									currstate = BRA;
									arr[m] = currchar;
									m++;
									i++;
									break;
							}
							break;
						case LET:
							currstate = SPC;
							for(a = 0; a < m; a++) {
								arr1[0] = arr[a];
								arr1[1] = '\0';
								for(l1 = 0; l1 < z; l1++) {
									strcpy(pat, bracket[l1]);
									strcpy(sis, str);
									strcat(sis, arr1);
									strcat(pat, sis);
									strcpy(dub[y], pat);
									y++;
								}
							}
							z = m * z;
							for(a = 0; a < y; a++) {
								strcpy(bracket[a], dub[a]);
							}
							i++;
							m = 0;
							y = 0;
							strcpy(str, "\0");
							break;
						case SPC:
							switch(currchar) {
								case '[':
									j = 0;
									currstate = BRA;
									i++;
									break;
								case ']':
									i++;
									break;
								case '\0':
									i++;
									break;
								default:
									str[j] = currchar;
									i++;
									j++;
									currstate = SPC;
									break;
							}
							break;
						default:
							break;
					}
				}
				for(i = 0; i < z; i++) {
					pinsert(&pl, bracket[i]);
				}
				break;
			}
			else if(string[i] == '.') {
				y++;
				if(flag_dot == 0) {
					flag_dot = 1;
					for(j = 0; j <= 94; j++) {
						strcpy(dub[j], string); 
					}
				}
				for(j = 0; j <= 94; j++) {
					dub[j][i] = ' ' + j;
				}
				break;
			}
			else if(string[i] == 47 ) {
				q = i;
				while(string[q] != '\0') {
					string[q] = string[q + 1];
					q++;
				}
				l = l - 1;
				i = i + 1;
			}
		}
	}
	if(flag_dot == 1) {
		for(j = 0; j <= 94; j++)
			pinsert(&pl, dub[j]);
	}
	return 0;
}
