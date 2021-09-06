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

	data_characteristics_t* inputs;
	data_characteristics_t* outputs;

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector connector = ed247simulink::ED247Connector(filename.c_str(),tools);

	// [ SETUP ]
	connector.allocateMemory();

	// [ EXERCISE ]
	status = connector.readED247Configuration();
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	inputs = connector.getInputs();
	outputs = connector.getOutputs();

	EXPECT_EQ(inputs->nstreams, 12);
	EXPECT_EQ(inputs->nsignals, 573);

	EXPECT_EQ(outputs->nstreams, 27);
	EXPECT_EQ(outputs->nsignals, 599);

	// [ TEARDOWN ]
	connector.freeMemory();

 }
