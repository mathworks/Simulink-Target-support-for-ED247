
#include "tools.h"

namespace ed247simulink {

 void Tools::myprintf(const char *fmt, ...) {

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

 int Tools::fileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
	myprintf("Does file \"%s\" exist ?", filename);
    if (file = fopen(filename, "r")){
        fclose(file);
		myprintf("\tYES\n");
        return 0;
    }
	myprintf("\tNO\n");
    return 1;
 }

 void Tools::fileparts(const char* path, char* folder) {
	 
	size_t i;
	
	strcpy(folder,path);
	for(i = strlen(folder) - 1; i; i--) {
		if (folder[i] == '/' || folder[i] == '\\'){
			folder[i] = '\0';
			return;
		}
	}

	if (strcmp(folder,path) == 0){
		strcpy(folder,"");
	}

 }

}

