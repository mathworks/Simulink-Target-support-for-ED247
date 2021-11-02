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

 TEST_F(A429Test, A429ICD)
 {

	cmd_read_status_t status;
	std::string filename = filefolder_ + "/a429_cmd_simple01.xml";

	cmd_data_t *data;
	data = (cmd_data_t*)malloc(sizeof(cmd_data_t));
	EXPECT_TRUE(data != NULL);

	ed247simulink::Tools tools;
	ed247simulink::Cmd cmd = ed247simulink::Cmd(&tools);

	/* [ EXERCISE ] */
	status = cmd.readData(filename.c_str(), data);

	/* [ VERIFY ] */
	EXPECT_EQ(status,CMD_READ_OK);

	// Number of elements
	EXPECT_EQ(data->counter.a429, 2);

	// Details
	// -------
	//
	EXPECT_STREQ(	data->a429[0].name,						"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		data->a429[0].n_messages,				3);
	EXPECT_STREQ(	data->a429[0].messages[0].name,			"T11M4_A429_SIMU2SWIM_BUS_1_350_10_I");
	EXPECT_EQ(		data->a429[0].messages[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[0].messages[0].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[0].messages[0].label,		"350");
	EXPECT_STREQ(	data->a429[0].messages[0].sdi,			"10");
	EXPECT_EQ(		data->a429[0].messages[0].period_us,	1000000);
	EXPECT_STREQ(	data->a429[0].messages[0].comment,		"T11_T11_PART1");
	EXPECT_STREQ(	data->a429[0].messages[1].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_10_I");
	EXPECT_EQ(		data->a429[0].messages[1].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[0].messages[1].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[0].messages[1].label,		"200");
	EXPECT_STREQ(	data->a429[0].messages[1].sdi,			"10");
	EXPECT_EQ(		data->a429[0].messages[1].period_us,	1000000);
	EXPECT_STREQ(	data->a429[0].messages[1].comment,		"T11_T11_PART1");
	EXPECT_STREQ(	data->a429[0].messages[2].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_11_I");
	EXPECT_EQ(		data->a429[0].messages[2].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[0].messages[2].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[0].messages[2].label,		"200");
	EXPECT_STREQ(	data->a429[0].messages[2].sdi,			"11");
	EXPECT_EQ(		data->a429[0].messages[2].period_us,	1000000);
	EXPECT_STREQ(	data->a429[0].messages[2].comment,		"T11_T11_PART1");

	EXPECT_STREQ(	data->a429[1].name,						"T11M4_A429_SIMU2SWIM_BUS_2_I");
	EXPECT_EQ(		data->a429[1].n_messages,				2);
	EXPECT_STREQ(	data->a429[1].messages[0].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_11_I");
	EXPECT_EQ(		data->a429[1].messages[0].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[1].messages[0].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[1].messages[0].label,		"200");
	EXPECT_STREQ(	data->a429[1].messages[0].sdi,			"11");
	EXPECT_EQ(		data->a429[1].messages[0].period_us,	1000000);
	EXPECT_STREQ(	data->a429[1].messages[0].comment,		"T11_T11_PART1");
	EXPECT_STREQ(	data->a429[1].messages[1].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_10_I");
	EXPECT_EQ(		data->a429[1].messages[1].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[1].messages[1].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[1].messages[1].label,		"200");
	EXPECT_STREQ(	data->a429[1].messages[1].sdi,			"10");
	EXPECT_EQ(		data->a429[1].messages[1].period_us,	1000000);
	EXPECT_STREQ(	data->a429[1].messages[1].comment,		"T11_T11_PART1");

	// [ TEARDOWN ]
	free(data);

 }

 TEST_F(A429Test, A429Configuration)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a429_mc_simple01.xml";

	data_characteristics_t* inputs;
	data_characteristics_t* outputs;

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector connector = ed247simulink::ED247Connector(filename.c_str(),&tools);

	// [ SETUP ]
	connector.allocateMemory();

	// [ EXERCISE ]
	status = connector.readED247Configuration();
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	inputs = connector.getInputs();
	outputs = connector.getOutputs();

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		inputs->nstreams,					2);
	EXPECT_EQ(		inputs->nsignals,					5);

	//
	EXPECT_STREQ(	inputs->streams[0].name,			"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A429);
	EXPECT_STREQ(	inputs->streams[0].icd,			"a429_cmd_simple01.xml");
	EXPECT_STREQ(	inputs->streams[0].bus,			"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		inputs->streams[0].nsignals,		3);
	EXPECT_EQ(		&(inputs->signals[0]),			inputs->streams[0].signals[0]);
	EXPECT_EQ(		&(inputs->signals[1]),			inputs->streams[0].signals[1]);
	EXPECT_EQ(		&(inputs->signals[2]),			inputs->streams[0].signals[2]);
	//
	EXPECT_STREQ(	inputs->signals[0].name,			"T11M4_A429_SIMU2SWIM_BUS_1_350_10_I");
	EXPECT_EQ(		inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		inputs->signals[0].size[0],		4);
	EXPECT_EQ(		inputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	inputs->signals[1].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_10_I");
	EXPECT_EQ(		inputs->signals[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[1].dimensions,	1);
	EXPECT_EQ(		inputs->signals[1].size[0],		4);
	EXPECT_EQ(		inputs->signals[1].sample_time,	1.F);
	//
	EXPECT_STREQ(	inputs->signals[2].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_11_I");
	EXPECT_EQ(		inputs->signals[2].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[2].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[2].dimensions,	1);
	EXPECT_EQ(		inputs->signals[2].size[0],		4);
	EXPECT_EQ(		inputs->signals[2].sample_time,	1.F);

	//
	EXPECT_STREQ(	inputs->streams[1].name,			"T11M4_A429_SIMU2SWIM_BUS_2_I");
	EXPECT_EQ(		inputs->streams[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->streams[1].stream_type,	ED247_STREAM_TYPE_A429);
	EXPECT_STREQ(	inputs->streams[1].icd,			"a429_cmd_simple01.xml");
	EXPECT_STREQ(	inputs->streams[1].bus,			"T11M4_A429_SIMU2SWIM_BUS_2_I");
	EXPECT_EQ(		inputs->streams[1].nsignals,		2);
	EXPECT_EQ(		&(inputs->signals[3]),			inputs->streams[1].signals[0]);
	EXPECT_EQ(		&(inputs->signals[4]),			inputs->streams[1].signals[1]);
	//
	EXPECT_STREQ(	inputs->signals[3].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_11_I");
	EXPECT_EQ(		inputs->signals[3].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[3].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[3].dimensions,	1);
	EXPECT_EQ(		inputs->signals[3].size[0],		4);
	EXPECT_EQ(		inputs->signals[3].sample_time,	1.F);
	//
	EXPECT_STREQ(	inputs->signals[4].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_10_I");
	EXPECT_EQ(		inputs->signals[4].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[4].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[4].dimensions,	1);
	EXPECT_EQ(		inputs->signals[4].size[0],		4);
	EXPECT_EQ(		inputs->signals[4].sample_time,	1.F);

	// Outputs
	EXPECT_EQ(		outputs->nstreams,				0);

	// [ TEARDOWN ]
	connector.freeMemory();

 }
 
  TEST_F(A429Test, A429Transmission)
 {
	int i,nrecv;
	unsigned char sendvalues[5][4];
	unsigned char recvvalues[5][4];
	memset(sendvalues,0, 5*4*sizeof(char));
	memset(recvvalues,0, 5*4*sizeof(char));

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* senddata;
	data_characteristics_t* recvdata;

	std::string sendconfiguration = filefolder_ + "/a429_mc_simple01.xml";
	std::string recvconfiguration = filefolder_ + "/a429_mc_simple02.xml";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector sendconnector = ed247simulink::ED247Connector(sendconfiguration.c_str(),&tools);
	ed247simulink::ED247Connector recvconnector = ed247simulink::ED247Connector(recvconfiguration.c_str(),&tools);

	// [ SETUP ]
	sendconnector.allocateMemory();
	recvconnector.allocateMemory();

	cstatus = sendconnector.readED247Configuration();
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = recvconnector.readED247Configuration();
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	senddata = sendconnector.getInputs();
	recvdata = recvconnector.getOutputs();

	for (i = 0; i < senddata->nsignals; i++){
		senddata->signals[i].valuePtr = (void*) &(sendvalues[i]);
	}
	for (i = 0; i < recvdata->nsignals; i++){
		recvdata->signals[i].valuePtr = (void*) &(recvvalues[i]);
	}	

	nrecv = 0;

	// [ EXERCISE ]
	sendvalues[0][0] =  1;
	sendvalues[0][1] =  2;
	sendvalues[0][2] =  3;
	sendvalues[0][3] =  4;
	sendvalues[1][0] =  5;
	sendvalues[1][1] =  6;
	sendvalues[1][2] =  7;
	sendvalues[1][3] =  8;
	sendvalues[2][0] =  9;
	sendvalues[2][1] = 10;
	sendvalues[2][2] = 11;
	sendvalues[2][3] = 12;
	sendvalues[3][0] = 13;
	sendvalues[3][1] = 14;
	sendvalues[3][2] = 15;
	sendvalues[3][3] = 16;
	sendvalues[4][0] = 17;
	sendvalues[4][1] = 18;
	sendvalues[4][2] = 19;
	sendvalues[4][3] = 20;

	sstatus = sendconnector.sendSimulinkToED247();
	rstatus = recvconnector.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,5);
	EXPECT_EQ(*((char *)recvdata->signals[0].valuePtr + 0), sendvalues[0][0]);
	EXPECT_EQ(*((char *)recvdata->signals[0].valuePtr + 1), sendvalues[0][1]);
	EXPECT_EQ(*((char *)recvdata->signals[0].valuePtr + 2), sendvalues[0][2]);
	EXPECT_EQ(*((char *)recvdata->signals[0].valuePtr + 3), sendvalues[0][3]);
	EXPECT_EQ(*((char *)recvdata->signals[1].valuePtr + 0), sendvalues[1][0]);
	EXPECT_EQ(*((char *)recvdata->signals[1].valuePtr + 1), sendvalues[1][1]);
	EXPECT_EQ(*((char *)recvdata->signals[1].valuePtr + 2), sendvalues[1][2]);
	EXPECT_EQ(*((char *)recvdata->signals[1].valuePtr + 3), sendvalues[1][3]);
	EXPECT_EQ(*((char *)recvdata->signals[2].valuePtr + 0), sendvalues[2][0]);
	EXPECT_EQ(*((char *)recvdata->signals[2].valuePtr + 1), sendvalues[2][1]);
	EXPECT_EQ(*((char *)recvdata->signals[2].valuePtr + 2), sendvalues[2][2]);
	EXPECT_EQ(*((char *)recvdata->signals[2].valuePtr + 3), sendvalues[2][3]);
	EXPECT_EQ(*((char *)recvdata->signals[3].valuePtr + 0), sendvalues[3][0]);
	EXPECT_EQ(*((char *)recvdata->signals[3].valuePtr + 1), sendvalues[3][1]);
	EXPECT_EQ(*((char *)recvdata->signals[3].valuePtr + 2), sendvalues[3][2]);
	EXPECT_EQ(*((char *)recvdata->signals[3].valuePtr + 3), sendvalues[3][3]);
	EXPECT_EQ(*((char *)recvdata->signals[4].valuePtr + 0), sendvalues[4][0]);
	EXPECT_EQ(*((char *)recvdata->signals[4].valuePtr + 1), sendvalues[4][1]);
	EXPECT_EQ(*((char *)recvdata->signals[4].valuePtr + 2), sendvalues[4][2]);
	EXPECT_EQ(*((char *)recvdata->signals[4].valuePtr + 3), sendvalues[4][3]);

	// [ TEARDOWN ]
	sendconnector.freeMemory();
	recvconnector.freeMemory();

 }