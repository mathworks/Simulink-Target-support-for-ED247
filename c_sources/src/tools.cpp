/******************************************************************************
 *
 * Copyright (c) 2021 The MathWorks Inc.
 *
 */
 
#include "tools.h"

namespace ed247simulink {

 void Tools::myprintf(const char *fmt, ...) {

	int (*printfcn)(const char *, ...);
	char str[1024] = "";
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	va_end(args);

 #ifdef DEBUG

 #ifndef MATLAB_MEX_FILE
	printf("%s",str);
 #else
	ssPrintf("%s",str);
 #endif

 #endif

 #ifdef SIMULINK_REAL_TIME
 	LOG(info,0) << str << std::endl;
 #endif
 
 #ifdef TOFILE
 std::ofstream debugfile;
 debugfile.open ("debug.log", std::ios_base::app);
 debugfile << str; 
 debugfile.close();
 #endif

 }

 int Tools::fileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
	myprintf("Does file \"%s\" exist ?", filename);
    if ((file = fopen(filename, "r"))){
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

