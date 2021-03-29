 
 #include "testutils.h"

 void FileFolderTest::SetUp(){
	 const char* ff = std::getenv("FILEFOLDER");
	 if (ff){
		 filefolder_.assign(ff,strlen(ff));
	 }
 }
