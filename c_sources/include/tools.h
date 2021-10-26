/******************************************************************************
 *
 * Copyright (c) 2021 The MathWorks Inc.
 *
 */
 
 #ifndef __ED247SLADAPTERTOOLS__
 #define __ED247SLADAPTERTOOLS__
 
 #include <stdlib.h>
 #include <string.h>
 #include <stdarg.h>
  
 #ifndef MATLAB_MEX_FILE
	#include <stdio.h>
 #else
	#include "simstruc.h"
 #endif

 // Include Logger for Simulink Real-Time
 #ifdef SIMULINK_REAL_TIME
	#include "Logger.hpp"
 #endif
 
 #define STRING_MAX_LENGTH 		512
 
 #ifdef _WIN32
	#define FILESEP "\\"
 #else
	#define FILESEP "/" 
 #endif

#include <fstream>

namespace ed247simulink {

	class Tools {

		public:
			void myprintf(const char *fmt, ...);
			int fileexists(const char * filename);
			void fileparts(const char* path, char* folder);

	};

}

#endif
