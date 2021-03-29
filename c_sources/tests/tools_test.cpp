/*
 * Copyright 2020 The MathWorks, Inc.
 */

 #include "gtest/gtest.h"
  
 #include "testutils.h"
    
 extern "C" {
	 #include "tools.h"
 }
 
 TEST(ToolsTest, FileParts)
 {
	/* [ SETUP ] */
	std::string absolutefilename = "C:\\Temp\\projectA\\fileB.txt";
	char folder[STRING_MAX_LENGTH];

	/* [ EXERCISE ] */
	fileparts(absolutefilename.c_str(),folder);

	/* [ VERIFY ] */
	EXPECT_STREQ(folder, "C:\\Temp\\projectA");

 }