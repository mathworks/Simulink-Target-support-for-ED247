/*
 * Copyright 2020 The MathWorks, Inc.
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
 

 TEST_F(A664Test, A664ICD)
 {

	cmd_read_status_t status;
	cmd_data_t *data;
	std::string filename = filefolder_ + "/a664_cmd_simple01.xml";

	/* [ EXERCISE ] */
	cmd_read_data(filename.c_str(), data);

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

 }
 
 TEST_F(A664Test, A664Configuration)
 {
	
	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a664_mc_simple01.xml";
	
	// [ SETUP ]
	io_allocate_memory(&data);
	
	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);
	
	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					1);
	EXPECT_EQ(		data->inputs->nsignals,					1);
	
	//
	EXPECT_STREQ(	data->inputs->streams[0].name,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		data->inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	data->inputs->streams[0].icd,			"a664_cmd_simple01.xml");
	EXPECT_STREQ(	data->inputs->streams[0].bus,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		data->inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->inputs->signals[0]),			data->inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->inputs->signals[0].name,			"T11MX_19492_WAIT_STEP_I");
	EXPECT_EQ(		data->inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[0].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[0].sample_time,	0.064F);
	
	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,					1);
	EXPECT_EQ(		data->outputs->nsignals,					1);
	
	//
	EXPECT_STREQ(	data->outputs->streams[0].name,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		data->outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	data->outputs->streams[0].icd,			"a664_cmd_simple01.xml");
	EXPECT_STREQ(	data->outputs->streams[0].bus,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		data->outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->outputs->signals[0]),			data->outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->outputs->signals[0].name,			"T11MX_15430_TEST_STEP_O");
	EXPECT_EQ(		data->outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[0].size[0],		4);
	EXPECT_EQ(		data->outputs->signals[0].sample_time,	1.F);

	// [ TEARDOWN ]
	io_free_memory(data);

 }

 TEST_F(A664Test, A664ConfigurationMessageSize)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a664_mc_simple03.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					1);
	EXPECT_EQ(		data->inputs->nsignals,					1);

	//
	EXPECT_STREQ(	data->inputs->streams[0].name,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		data->inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	data->inputs->streams[0].icd,			"a664_cmd_simple03.xml");
	EXPECT_STREQ(	data->inputs->streams[0].bus,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		data->inputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->inputs->signals[0]),			data->inputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->inputs->signals[0].name,			"T11MX_15430_TEST_STEP");
	EXPECT_EQ(		data->inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[0].size[0],		2);
	EXPECT_EQ(		data->inputs->signals[0].sample_time,	1.F);

	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,				1);
	EXPECT_EQ(		data->outputs->nsignals,				1);

	//
	EXPECT_STREQ(	data->outputs->streams[0].name,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		data->outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->streams[0].stream_type,	ED247_STREAM_TYPE_A664);
	EXPECT_STREQ(	data->outputs->streams[0].icd,			"a664_cmd_simple03.xml");
	EXPECT_STREQ(	data->outputs->streams[0].bus,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		data->outputs->streams[0].nsignals,		1);
	EXPECT_EQ(		&(data->outputs->signals[0]),			data->outputs->streams[0].signals[0]);
	//
	EXPECT_STREQ(	data->outputs->signals[0].name,			"T11MX_19492_WAIT_STEP");
	EXPECT_EQ(		data->outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[0].size[0],		6);
	EXPECT_EQ(		data->outputs->signals[0].sample_time,	0.064F);

	// [ TEARDOWN ]
	io_free_memory(data);

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
	
	IO_t *data01;
	IO_t *data02;
	std::string configuration01 = filefolder_ + "/a664_mc_simple01.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple02.xml";
	
	// [ SETUP ]
	io_allocate_memory(&data01);
	io_allocate_memory(&data02);
	
	cstatus = read_ed247_configuration(configuration01.c_str(),data01,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = read_ed247_configuration(configuration02.c_str(),data02,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	
	ASSERT_EQ(data01->inputs->nsignals, 	1);
	ASSERT_EQ(data01->outputs->nsignals, 	1);
	ASSERT_EQ(data02->inputs->nsignals, 	1);
	ASSERT_EQ(data02->outputs->nsignals, 	1);
	
	data01->inputs->signals[0].valuePtr 	= (void*) &(sendvalues[0]);
	data01->outputs->signals[0].valuePtr 	= (void*) &(recvvalues[0]);
	data02->inputs->signals[0].valuePtr 	= (void*) &(sendvalues[1]);
	data02->outputs->signals[0].valuePtr 	= (void*) &(recvvalues[1]);
	
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
	sstatus = send_simulink_to_ed247(data01);
	rstatus = receive_ed247_to_simulink(data02, &nrecv);
	
	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);
	
	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 0), sendvalues[0][0]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 1), sendvalues[0][1]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 2), sendvalues[0][2]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 3), sendvalues[0][3]);
	
	nrecv = 0;
	sstatus = send_simulink_to_ed247(data02);
	rstatus = receive_ed247_to_simulink(data01, &nrecv);
	
	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);
	
	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 0), sendvalues[1][0]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 1), sendvalues[1][1]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 2), sendvalues[1][2]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 3), sendvalues[1][3]);
		
	// [ TEARDOWN ]
	io_free_memory(data01);
	io_free_memory(data02);
	
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

	IO_t *data01;
	IO_t *data02;
	std::string configuration01 = filefolder_ + "/a664_mc_simple03.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple04.xml";

	// [ SETUP ]
	io_allocate_memory(&data01);
	io_allocate_memory(&data02);

	cstatus = read_ed247_configuration(configuration01.c_str(),data01,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = read_ed247_configuration(configuration02.c_str(),data02,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	ASSERT_EQ(data01->inputs->nsignals, 	1);
	ASSERT_EQ(data01->outputs->nsignals, 	1);
	ASSERT_EQ(data02->inputs->nsignals, 	1);
	ASSERT_EQ(data02->outputs->nsignals, 	1);

	data01->inputs->signals[0].valuePtr 	= (void*) &(sendvalues01);
	data01->outputs->signals[0].valuePtr 	= (void*) &(recvvalues01);
	data02->inputs->signals[0].valuePtr 	= (void*) &(sendvalues02);
	data02->outputs->signals[0].valuePtr 	= (void*) &(recvvalues02);

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
	sstatus = send_simulink_to_ed247(data01);
	rstatus = receive_ed247_to_simulink(data02, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 0), sendvalues01[0]);
	EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 1), sendvalues01[1]);

	nrecv = 0;
	sstatus = send_simulink_to_ed247(data02);
	rstatus = receive_ed247_to_simulink(data01, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	EXPECT_EQ(nrecv,1);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 0), sendvalues02[0]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 1), sendvalues02[1]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 2), sendvalues02[2]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 3), sendvalues02[3]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 4), sendvalues02[4]);
	EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 5), sendvalues02[5]);

	// [ TEARDOWN ]
	io_free_memory(data01);
	io_free_memory(data02);

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
	
	IO_t *data01;
	IO_t *data02;
	std::string configuration01 = filefolder_ + "/a664_mc_simple01.xml";
	std::string configuration02 = filefolder_ + "/a664_mc_simple02.xml";
	
	// [ SETUP ]
	io_allocate_memory(&data01);
	io_allocate_memory(&data02);
	
	cstatus = read_ed247_configuration(configuration01.c_str(),data01,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = read_ed247_configuration(configuration02.c_str(),data02,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	
	ASSERT_EQ(data01->inputs->nsignals, 	1);
	ASSERT_EQ(data01->outputs->nsignals, 	1);
	ASSERT_EQ(data02->inputs->nsignals, 	1);
	ASSERT_EQ(data02->outputs->nsignals, 	1);
	
	data01->inputs->signals[0].valuePtr 	= (void*) &(sendvalues[0]);
	data01->outputs->signals[0].valuePtr 	= (void*) &(recvvalues[0]);
	data02->inputs->signals[0].valuePtr 	= (void*) &(sendvalues[1]);
	data02->outputs->signals[0].valuePtr 	= (void*) &(recvvalues[1]);
	
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
		rstatus = receive_ed247_to_simulink(data01, &nrecv);
		sstatus = send_simulink_to_ed247(data01);
		
		ASSERT_EQ(sstatus, SEND_OK);
		if (i == 0){
			ASSERT_EQ(rstatus, NO_DATA_BEFORE_TIMEOUT);
		} else {
			ASSERT_EQ(rstatus, RECEIVE_OK);
		}
		
		// Consider "block" 02 -> receive and send
		nrecv = 0;
		rstatus = receive_ed247_to_simulink(data02, &nrecv);
		sstatus = send_simulink_to_ed247(data02);		
		
		ASSERT_EQ(sstatus, SEND_OK);
		ASSERT_EQ(rstatus, RECEIVE_OK);
		
		EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 0), sendvalues[0][0]);
		EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 1), sendvalues[0][1]);
		EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 2), sendvalues[0][2]);
		EXPECT_EQ(*((unsigned char *)data02->outputs->signals[0].valuePtr + 3), sendvalues[0][3]);
		EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 0), prevvalue[0]);
		EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 1), prevvalue[1]);
		EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 2), prevvalue[2]);
		EXPECT_EQ(*((unsigned char *)data01->outputs->signals[0].valuePtr + 3), prevvalue[3]);
		
		prevvalue[0] = sendvalues[1][0];
		prevvalue[1] = sendvalues[1][1];
		prevvalue[2] = sendvalues[1][2];
		prevvalue[3] = sendvalues[1][3];
		
	}
	
	// [ TEARDOWN ]
	io_free_memory(data01);
	io_free_memory(data02);
	
 }
 