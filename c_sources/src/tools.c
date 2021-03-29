
#include "tools.h"

 void myprintf(const char *fmt, ...) {

	int (*printfcn)(const char *, ...);
	char str[1024] = "";
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	va_end(args);

 #ifndef MATLAB_MEX_FILE
	printfcn = &printf;
 #else
	printfcn = &ssPrintf;
 #endif

 #ifdef DEBUG
	(*printfcn)("%s",str);
 #endif

 }

 int fileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 0;
    }
    return 1;
 }

 void fileparts(const char* path, char* folder) {
	 
	size_t i;
	
	strcpy(folder,path);
	for(i = strlen(folder) - 1; i; i--) {
		if (folder[i] == '/' || folder[i] == '\\'){
			folder[i] = '\0';
			return;
		}
	}
 }

