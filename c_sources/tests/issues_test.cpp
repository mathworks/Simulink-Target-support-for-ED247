/*
 * Copyright 2021 The MathWorks, Inc.
 */

 #include "gtest/gtest.h"
  
 #include "testutils.h"
 
 #include <unistd.h>
 #include <iostream>
 #include <cstdlib>
 #include <string>

 extern "C" {
	 #include "ed247_interface.h"
 }

 TEST_F(IssuesTest, ELACFull)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/ELACe2C_ECIC.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	EXPECT_EQ(data->inputs->nstreams, 12);
	EXPECT_EQ(data->inputs->nsignals, 573);

	EXPECT_EQ(data->outputs->nstreams, 27);
	EXPECT_EQ(data->outputs->nsignals, 599);

	// [ TEARDOWN ]
	io_free_memory(data);

 }
