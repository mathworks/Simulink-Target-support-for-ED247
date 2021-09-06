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

 TEST_F(A664Test, A664ICD)
 {

	cmd_read_status_t status;
	std::string filename = filefolder_ + "/a664_cmd_simple01.xml";

	cmd_data_t *data;
	data = (cmd_data_t*)malloc(sizeof(cmd_data_t));
	EXPECT_TRUE(data != NULL);

	ed247simulink::Tools tools;
	ed247simulink::Cmd cmd = ed247simulink::Cmd(tools);

	/* [ EXERCISE ] */
	cmd.readData(filename.c_str(), data);

	/* [ VERIFY ] */
	// Number of elements
	EXPECT_EQ(data->counter.a664, 2);

	// Details
	// ------------
	//
	EXPECT_STREQ(	data->a664[0].name,						"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		data->a664[0].direction,				ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a664[0].sample_max_size_bytes,	4);
	EXPECT_EQ(		data->a664[0].period_us,				1000000);
	EXPECT_STREQ(	data->a664[0].comment,					"T11_T11_PART1");
	//
	EXPECT_STREQ(	data->a664[1].name,						"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		data->a664[1].direction,				ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a664[1].sample_max_size_bytes,	4);
	EXPECT_EQ(		data->a664[1].period_us,				64000);
	EXPECT_STREQ(	data->a664[1].comment,					"T11_T11_PART1");

	// [ TEARDOWN ]
	free(data);

 }
 
 TEST_F(A664Test, A664Configuration)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a664_mc_simple01.xml";

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
	EXPECT_EQ(		inputs->nstreams,					1);
	EXPECT_EQ(		inputs->nsignals,					1);

	//
	EXPECT_STREQ(	inputs->streams[0].name,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	inputs->streams[0].icd,			"a664_cmd_simple01.xml");
	EXPECT_STREQ(	inputs->streams[0].bus,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(inputs->signals[0]),			inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	inputs->signals[0].name,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		inputs->signals[0].size[0],		4);
	EXPECT_EQ(		inputs->signals[0].sample_time,	0.064F);
	
	// Outputs
	outputs = connector.getOutputs();
	EXPECT_EQ(		outputs->nstreams,					1);
	EXPECT_EQ(		outputs->nsignals,					1);
	
	//
	EXPECT_STREQ(	outputs->streams[0].name,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	outputs->streams[0].icd,			"a664_cmd_simple01.xml");
	EXPECT_STREQ(	outputs->streams[0].bus,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(outputs->signals[0]),			outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	outputs->signals[0].name,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		outputs->signals[0].size[0],		4);
	EXPECT_EQ(		outputs->signals[0].sample_time,	1.F);

	// [ TEARDOWN ]
	connector.freeMemory();

 }

 TEST_F(A664Test, A664ConfigurationMessageSize)
 {

	configuration_status_t status;

	data_characteristics_t* inputs;
	data_characteristics_t* outputs;

	std::string filename = filefolder_ + "/a664_mc_simple03.xml";

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
	EXPECT_EQ(		inputs->nstreams,					1);
	EXPECT_EQ(		inputs->nsignals,					1);

	//
	EXPECT_STREQ(	inputs->streams[0].name,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	inputs->streams[0].icd,			"a664_cmd_simple03.xml");
	EXPECT_STREQ(	inputs->streams[0].bus,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(inputs->signals[0]),			inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	inputs->signals[0].name,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		inputs->signals[0].size[0],		2);
	EXPECT_EQ(		inputs->signals[0].sample_time,	1.F);

	// Outputs
	outputs = connector.getOutputs();
	EXPECT_EQ(		outputs->nstreams,				1);
	EXPECT_EQ(		outputs->nsignals,				1);

	//
	EXPECT_STREQ(	outputs->streams[0].name,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	outputs->streams[0].icd,			"a664_cmd_simple03.xml");
	EXPECT_STREQ(	outputs->streams[0].bus,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(outputs->signals[0]),			outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	outputs->signals[0].name,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		outputs->signals[0].size[0],		6);
	EXPECT_EQ(		outputs->signals[0].sample_time,	0.064F);

	// [ TEARDOWN ]
	connector.freeMemory();

 }
 
 TEST_F(A664Test, A664Transmission)
 {
	int i,nrecv;
	unsigned char sendvalues[2][4];
	unsigned char recvvalues[2][4];
	memset(sendvalues,0, 2*4*sizeof(char));
	memset(recvvalues,0, 2*4*sizeof(char));

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* data01in;
	data_characteristics_t* data01out;
	data_characteristics_t* data02in;
	data_characteristics_t* data02out;

	std::string configuration01 = filefolder_ + "/a664_mc_simple01.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple02.xml";

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

	ASSERT_EQ(data01in->nsignals, 	1);
	ASSERT_EQ(data01out->nsignals, 	1);
	ASSERT_EQ(data02in->nsignals, 	1);
	ASSERT_EQ(data02out->nsignals, 	1);

	data01in->signals[0].valuePtr 	= (void*) &(sendvalues[0]);
	data01out->signals[0].valuePtr 	= (void*) &(recvvalues[0]);
	data02in->signals[0].valuePtr 	= (void*) &(sendvalues[1]);
	data02out->signals[0].valuePtr 	= (void*) &(recvvalues[1]);

	// [ EXERCISE ]
	sendvalues[0][0] =  1;
	sendvalues[0][1] =  2;
	sendvalues[0][2] =  3;
	sendvalues[0][3] =  4;
	sendvalues[1][0] =  5;
	sendvalues[1][1] =  6;
	sendvalues[1][2] =  7;
	sendvalues[1][3] =  8;

	nrecv = 0;
	sstatus = conn01.sendSimulinkToED247();
	rstatus = conn02.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 0), sendvalues[0][0]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 1), sendvalues[0][1]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 2), sendvalues[0][2]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 3), sendvalues[0][3]);

	nrecv = 0;
	sstatus = conn02.sendSimulinkToED247();
	rstatus = conn01.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 0), sendvalues[1][0]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 1), sendvalues[1][1]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 2), sendvalues[1][2]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 3), sendvalues[1][3]);

	// [ TEARDOWN ]
	conn01.freeMemory();
	conn02.freeMemory();

 }

 TEST_F(A664Test, A664TransmissionMessageSize)
 {
	int i,nrecv;
	unsigned char sendvalues01[2] = {0,0};
	unsigned char sendvalues02[6] = {0,0,0,0,0,0};
	unsigned char recvvalues01[6] = {0,0,0,0,0,0};
	unsigned char recvvalues02[2] = {0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* data01in;
	data_characteristics_t* data01out;
	data_characteristics_t* data02in;
	data_characteristics_t* data02out;

	std::string configuration01 = filefolder_ + "/a664_mc_simple03.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple04.xml";

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

	ASSERT_EQ(data01in->nsignals, 	1);
	ASSERT_EQ(data01out->nsignals, 	1);
	ASSERT_EQ(data02in->nsignals, 	1);
	ASSERT_EQ(data02out->nsignals, 	1);

	data01in->signals[0].valuePtr 	= (void*) &(sendvalues01);
	data01out->signals[0].valuePtr 	= (void*) &(recvvalues01);
	data02in->signals[0].valuePtr 	= (void*) &(sendvalues02);
	data02out->signals[0].valuePtr 	= (void*) &(recvvalues02);

	// [ EXERCISE ]
	sendvalues01[0] = 1;
	sendvalues01[1] = 2;
	sendvalues02[0] = 3;
	sendvalues02[1] = 4;
	sendvalues02[2] = 5;
	sendvalues02[3] = 6;
	sendvalues02[4] = 7;
	sendvalues02[5] = 8;

	nrecv = 0;
	sstatus = conn01.sendSimulinkToED247();
	rstatus = conn02.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 0), sendvalues01[0]);
	EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 1), sendvalues01[1]);

	nrecv = 0;
	sstatus = conn02.sendSimulinkToED247();
	rstatus = conn01.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 0), sendvalues02[0]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 1), sendvalues02[1]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 2), sendvalues02[2]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 3), sendvalues02[3]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 4), sendvalues02[4]);
	EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 5), sendvalues02[5]);

	// [ TEARDOWN ]
	conn01.freeMemory();
	conn02.freeMemory();

 }
 
 TEST_F(A664Test, A664TransmissionLoop)
 {
	int SIMULATION_STEPS = 10;

	int i,nrecv;
	unsigned char sendvalues[2][4];
	unsigned char recvvalues[2][4];
	unsigned char prevvalue[4] = {0,0,0,0};
	memset(sendvalues,0, 2*4*sizeof(char));
	memset(recvvalues,0, 2*4*sizeof(char));

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* data01in;
	data_characteristics_t* data01out;
	data_characteristics_t* data02in;
	data_characteristics_t* data02out;

	std::string configuration01 = filefolder_ + "/a664_mc_simple01.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple02.xml";

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

	ASSERT_EQ(data01in->nsignals, 	1);
	ASSERT_EQ(data01out->nsignals, 	1);
	ASSERT_EQ(data02in->nsignals, 	1);
	ASSERT_EQ(data02out->nsignals, 	1);

	data01in->signals[0].valuePtr 	= (void*) &(sendvalues[0]);
	data01out->signals[0].valuePtr 	= (void*) &(recvvalues[0]);
	data02in->signals[0].valuePtr 	= (void*) &(sendvalues[1]);
	data02out->signals[0].valuePtr 	= (void*) &(recvvalues[1]);

	// [ EXERCISE ] 
	for (i = 0; i<SIMULATION_STEPS; i++){

		sendvalues[0][0] =  i * 1;
		sendvalues[0][1] =  i * 2;
		sendvalues[0][2] =  i * 3;
		sendvalues[0][3] =  i * 4;
		sendvalues[1][0] =  i + 5;
		sendvalues[1][1] =  i + 6;
		sendvalues[1][2] =  i + 7;
		sendvalues[1][3] =  i + 8;

		// Consider "block" 01 -> send and receive
		nrecv = 0;
		rstatus = conn01.receiveED247ToSimulink(&nrecv);
		sstatus = conn01.sendSimulinkToED247();

		ASSERT_EQ(sstatus, SEND_OK);
		if (i == 0){
			ASSERT_EQ(rstatus, NO_DATA_BEFORE_TIMEOUT);
		} else {
			ASSERT_EQ(rstatus, RECEIVE_OK);
		}

		// Consider "block" 02 -> receive and send
		nrecv = 0;
		rstatus = conn02.receiveED247ToSimulink(&nrecv);
		sstatus = conn02.sendSimulinkToED247();

		ASSERT_EQ(sstatus, SEND_OK);
		ASSERT_EQ(rstatus, RECEIVE_OK);

		EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 0), sendvalues[0][0]);
		EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 1), sendvalues[0][1]);
		EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 2), sendvalues[0][2]);
		EXPECT_EQ(*((unsigned char *)data02out->signals[0].valuePtr + 3), sendvalues[0][3]);
		EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 0), prevvalue[0]);
		EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 1), prevvalue[1]);
		EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 2), prevvalue[2]);
		EXPECT_EQ(*((unsigned char *)data01out->signals[0].valuePtr + 3), prevvalue[3]);

		prevvalue[0] = sendvalues[1][0];
		prevvalue[1] = sendvalues[1][1];
		prevvalue[2] = sendvalues[1][2];
		prevvalue[3] = sendvalues[1][3];

	}

	// [ TEARDOWN ]
	conn01.freeMemory();
	conn02.freeMemory();

 }
 