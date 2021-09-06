/*
 * Copyright 2020 The MathWorks, Inc.
 */

 #include "gtest/gtest.h"
  
 #include "testutils.h"
 
 #include <unistd.h>
 #include <iostream>
 #include <cstdlib>
 #include <string>

 #include "ed247_interface.h"

 TEST_F(A825Test, A825ICD)
 {

	cmd_read_status_t status;
	std::string filename = filefolder_ + "/a825_cmd_simple01.xml";

	cmd_data_t *data;
	data = (cmd_data_t*)malloc(sizeof(cmd_data_t));
	EXPECT_TRUE(data != NULL);

	ed247simulink::Tools tools;
	ed247simulink::Cmd cmd = ed247simulink::Cmd(tools);

	/* [ EXERCISE ] */
	status = cmd.readData(filename.c_str(), data);

	/* [ VERIFY ] */
	EXPECT_EQ(status,CMD_READ_OK);

	// Number of elements
	EXPECT_EQ(data->counter.a825, 2);

	// Details
	// ------------
	//
	EXPECT_STREQ(	data->a825[0].name,						"Stream0");
	EXPECT_EQ(		data->a825[0].sample_max_number,		3);
	EXPECT_EQ(		data->a825[0].period_us,				1000000);
	EXPECT_STREQ(	data->a825[0].comment,					"Stream0");

	EXPECT_STREQ(	data->a825[1].name,						"Stream1");
	EXPECT_EQ(		data->a825[1].sample_max_number,		5);
	EXPECT_EQ(		data->a825[1].period_us,				64000);
	EXPECT_STREQ(	data->a825[1].comment,					"Stream1");

	// [ TEARDOWN ]
	free(data);

 }

 TEST_F(A825Test, A825Configuration)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a825_mc_1.xml";

	data_characteristics_t* inputs;
	data_characteristics_t* outputs;

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector connector = ed247simulink::ED247Connector(filename.c_str(),tools);

	// [ SETUP ]
	connector.allocateMemory();

	// [ EXERCISE ]
	status = connector.readED247Configuration();
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	inputs = connector.getInputs();
	EXPECT_EQ(		inputs->nstreams,					2);
	EXPECT_EQ(		inputs->nsignals,					2);

	//
	EXPECT_STREQ(	inputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		inputs->streams[0].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A825);
	EXPECT_STREQ(	inputs->streams[0].icd,			"a825_cmd_simple01.xml");
	EXPECT_STREQ(	inputs->streams[0].bus,			"Stream0");
	EXPECT_EQ(		inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(inputs->signals[0]),			inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	inputs->signals[0].name,			"Stream0");
	EXPECT_EQ(		inputs->signals[0].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		inputs->signals[0].size[0],		3);
	EXPECT_EQ(		inputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	inputs->signals[1].name,			"Stream1");
	EXPECT_EQ(		inputs->signals[1].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		inputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[1].dimensions,	1);
	EXPECT_EQ(		inputs->signals[1].size[0],		5);
	EXPECT_EQ(		inputs->signals[1].sample_time,	0.064F);

	// Outputs
	outputs = connector.getOutputs();
	EXPECT_EQ(		outputs->nstreams,					2);
	EXPECT_EQ(		outputs->nsignals,					2);

	//
	EXPECT_STREQ(	outputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		outputs->streams[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A825);
	EXPECT_STREQ(	outputs->streams[0].icd,			"a825_cmd_simple01.xml");
	EXPECT_STREQ(	outputs->streams[0].bus,			"Stream0");
	EXPECT_EQ(		outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(outputs->signals[0]),			outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	outputs->signals[0].name,			"Stream0");
	EXPECT_EQ(		outputs->signals[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		outputs->signals[0].size[0],		3);
	EXPECT_EQ(		outputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	outputs->signals[1].name,			"Stream1");
	EXPECT_EQ(		outputs->signals[1].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		outputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		outputs->signals[1].dimensions,	1);
	EXPECT_EQ(		outputs->signals[1].size[0],		5);
	EXPECT_EQ(		outputs->signals[1].sample_time,	0.064F);

	// [ TEARDOWN ]
	connector.freeMemory();

 }

 TEST_F(A825Test, A825Transmission)
 {
	int i,nrecv;
	unsigned char sendvalues01[3];
	unsigned char recvvalues01[3];
	unsigned char sendvalues02[5];
	unsigned char recvvalues02[5];
	memset(sendvalues01,0, 3*sizeof(char));
	memset(recvvalues01,0, 3*sizeof(char));
	memset(sendvalues02,0, 5*sizeof(char));
	memset(recvvalues02,0, 5*sizeof(char));

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* data01in;
	data_characteristics_t* data01out;
	data_characteristics_t* data02in;
	data_characteristics_t* data02out;

	std::string configuration01 = filefolder_ + "/a825_mc_1.xml";
	std::string configuration02 = filefolder_ + "/a825_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector conn01 = ed247simulink::ED247Connector(configuration01.c_str(),tools);
	ed247simulink::ED247Connector conn02 = ed247simulink::ED247Connector(configuration02.c_str(),tools);

	// [ SETUP ]
	conn01.allocateMemory();
	conn02.allocateMemory();

	cstatus = conn01.readED247Configuration();
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = conn02.readED247Configuration();
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	data01in  = conn01.getInputs();
	data01out = conn01.getOutputs();
	data02in  = conn02.getInputs();
	data02out = conn02.getOutputs();

	ASSERT_EQ(data01in->nsignals, 	2);
	ASSERT_EQ(data01out->nsignals, 	2);
	ASSERT_EQ(data02in->nsignals, 	2);
	ASSERT_EQ(data02out->nsignals, 	2);

	data01in->signals[0].valuePtr 	= (void*) &(sendvalues01);
	data01in->signals[1].valuePtr 	= (void*) &(sendvalues02);
	data02out->signals[0].valuePtr 	= (void*) &(recvvalues01);
	data02out->signals[1].valuePtr 	= (void*) &(recvvalues02);

	// [ EXERCISE ]
	sendvalues01[0] =  1;
	sendvalues01[1] =  2;
	sendvalues01[2] =  3;
	sendvalues02[0] =  4;
	sendvalues02[1] =  5;
	sendvalues02[2] =  6;
	sendvalues02[3] =  7;
	sendvalues02[4] =  8;

	nrecv = 0;
	sstatus = conn01.sendSimulinkToED247();
	rstatus = conn02.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,2);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 0), sendvalues01[0]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 1), sendvalues01[1]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 2), sendvalues01[2]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[1].valuePtr + 0), sendvalues02[0]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[1].valuePtr + 1), sendvalues02[1]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[1].valuePtr + 2), sendvalues02[2]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[1].valuePtr + 3), sendvalues02[3]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[1].valuePtr + 4), sendvalues02[4]);

	nrecv = 0;
	sstatus = conn02.sendSimulinkToED247();
	rstatus = conn01.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_A825_SKIP);
	ASSERT_EQ(rstatus, RECEIVE_A825_SKIP);

	EXPECT_EQ(nrecv,0);

	// [ TEARDOWN ]
	conn01.freeMemory();
	conn02.freeMemory();

 }
