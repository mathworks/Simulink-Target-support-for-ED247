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

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&data);

	// [ EXERCISE ]
	status = interface.readED247Configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					2);
	EXPECT_EQ(		data->inputs->nsignals,					2);

	//
	EXPECT_STREQ(	data->inputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		data->inputs->streams[0].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		data->inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A825);
	EXPECT_STREQ(	data->inputs->streams[0].icd,			"a825_cmd_simple01.xml");
	EXPECT_STREQ(	data->inputs->streams[0].bus,			"Stream0");
	EXPECT_EQ(		data->inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->inputs->signals[0]),			data->inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->inputs->signals[0].name,			"Stream0");
	EXPECT_EQ(		data->inputs->signals[0].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		data->inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[0].size[0],		3);
	EXPECT_EQ(		data->inputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	data->inputs->signals[1].name,			"Stream1");
	EXPECT_EQ(		data->inputs->signals[1].direction,		ED247_DIRECTION_IN);
	EXPECT_EQ(		data->inputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[1].size[0],		5);
	EXPECT_EQ(		data->inputs->signals[1].sample_time,	0.064F);

	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,					2);
	EXPECT_EQ(		data->outputs->nsignals,					2);

	//
	EXPECT_STREQ(	data->outputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		data->outputs->streams[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A825);
	EXPECT_STREQ(	data->outputs->streams[0].icd,			"a825_cmd_simple01.xml");
	EXPECT_STREQ(	data->outputs->streams[0].bus,			"Stream0");
	EXPECT_EQ(		data->outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->outputs->signals[0]),			data->outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->outputs->signals[0].name,			"Stream0");
	EXPECT_EQ(		data->outputs->signals[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[0].size[0],		3);
	EXPECT_EQ(		data->outputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	data->outputs->signals[1].name,			"Stream1");
	EXPECT_EQ(		data->outputs->signals[1].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->outputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[1].size[0],		5);
	EXPECT_EQ(		data->outputs->signals[1].sample_time,	0.064F);

	// [ TEARDOWN ]
	interface.ioFreeMemory(data);

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

	IO_t *data01;
	IO_t *data02;
	std::string configuration01 = filefolder_ + "/a825_mc_1.xml";
	std::string configuration02 = filefolder_ + "/a825_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&data01);
	interface.ioAllocateMemory(&data02);

	cstatus = interface.readED247Configuration(configuration01.c_str(),data01,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = interface.readED247Configuration(configuration02.c_str(),data02,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	ASSERT_EQ(data01->inputs->nsignals, 	2);
	ASSERT_EQ(data01->outputs->nsignals, 	2);
	ASSERT_EQ(data02->inputs->nsignals, 	2);
	ASSERT_EQ(data02->outputs->nsignals, 	2);

	data01->inputs->signals[0].valuePtr 	= (void*) &(sendvalues01);
	data01->inputs->signals[1].valuePtr 	= (void*) &(sendvalues02);
	data02->outputs->signals[0].valuePtr 	= (void*) &(recvvalues01);
	data02->outputs->signals[1].valuePtr 	= (void*) &(recvvalues02);

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
	sstatus = interface.sendSimulinkToED247(data01);
	rstatus = interface.receiveED247ToSimulink(data02, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,2);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 0), sendvalues01[0]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 1), sendvalues01[1]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 2), sendvalues01[2]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[1].valuePtr + 0), sendvalues02[0]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[1].valuePtr + 1), sendvalues02[1]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[1].valuePtr + 2), sendvalues02[2]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[1].valuePtr + 3), sendvalues02[3]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[1].valuePtr + 4), sendvalues02[4]);

	nrecv = 0;
	sstatus = interface.sendSimulinkToED247(data02);
	rstatus = interface.receiveED247ToSimulink(data01, &nrecv);

	ASSERT_EQ(sstatus, SEND_A825_SKIP);
	ASSERT_EQ(rstatus, RECEIVE_A825_SKIP);

	EXPECT_EQ(nrecv,0);

	// [ TEARDOWN ]
	interface.ioFreeMemory(data01);
	interface.ioFreeMemory(data02);

 }
