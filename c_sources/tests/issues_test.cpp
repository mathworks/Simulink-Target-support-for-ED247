/*
 * Copyright 2021 The MathWorks, Inc.
 */

 #include "gtest/gtest.h"
  
 #include "testutils.h"
 
 #include <unistd.h>
 #include <iostream>
 #include <cstdlib>
 #include <string>

 #include "ed247_interface.h"

 TEST_F(IssuesTest, ELACFull)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/ELACe2C_ECIC.xml";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&data);

	// [ EXERCISE ]
	status = interface.readED247Configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	EXPECT_EQ(data->inputs->nstreams, 12);
	EXPECT_EQ(data->inputs->nsignals, 573);

	EXPECT_EQ(data->outputs->nstreams, 27);
	EXPECT_EQ(data->outputs->nsignals, 599);

	// [ TEARDOWN ]
	interface.ioFreeMemory(data);

 }
