#ifndef XBOXRT_CTYPE
#define XBOXRT_CTYPE

int digittoint(int c);
int isalnum(int c);
int isalpha(int c);
int isascii(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int ishexnumber(int c);
int islower(int c);
int isnumber(int c);
int isprint(int c);
int ispunct(int c);
int isrune(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int toascii(int c);
int tolower(int c);
int toupper(int c);

#endif