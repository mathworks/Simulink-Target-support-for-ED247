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
 
  TEST_F(NADTest, NADConfiguration1)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/nad_mc_1.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					2);
	EXPECT_EQ(		data->inputs->nsignals,					4);

	//
	EXPECT_STREQ(	data->inputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		data->inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[0].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->inputs->streams[0].nsignals,		2);	
	EXPECT_EQ(		&(data->inputs->signals[0]),			data->inputs->streams[0].signals[0]);
	EXPECT_EQ(		&(data->inputs->signals[1]),			data->inputs->streams[0].signals[1]);
	//
	EXPECT_STREQ(	data->inputs->signals[0].name,			"Signal00");
	EXPECT_EQ(		data->inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[0].size[0],		1);
	EXPECT_EQ(		data->inputs->signals[0].sample_size,	1);
	EXPECT_EQ(		data->inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[0].sample_time,	0.01F);	
	EXPECT_EQ(		data->inputs->signals[0].signal_type,	ED247_SIGNAL_TYPE_NAD);
	//
	EXPECT_STREQ(	data->inputs->signals[1].name,			"Signal01");
	EXPECT_EQ(		data->inputs->signals[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[1].size[0],		1);
	EXPECT_EQ(		data->inputs->signals[1].sample_size,	1);
	EXPECT_EQ(		data->inputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[1].sample_time,	0.01F);
	EXPECT_EQ(		data->inputs->signals[1].signal_type,	ED247_SIGNAL_TYPE_NAD);

	//
	EXPECT_STREQ(	data->inputs->streams[1].name,			"Stream1");
	EXPECT_EQ(		data->inputs->streams[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[1].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->inputs->streams[1].nsignals,		2);
	EXPECT_EQ(		&(data->inputs->signals[2]),			data->inputs->streams[1].signals[0]);
	EXPECT_EQ(		&(data->inputs->signals[3]),			data->inputs->streams[1].signals[1]);
	//
	EXPECT_STREQ(	data->inputs->signals[2].name,			"Signal10");
	EXPECT_EQ(		data->inputs->signals[2].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[2].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[2].size[0],		1);
	EXPECT_EQ(		data->inputs->signals[2].sample_size,	1);
	EXPECT_EQ(		data->inputs->signals[2].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[2].sample_time,	0.01F);
	EXPECT_EQ(		data->inputs->signals[2].signal_type,	ED247_SIGNAL_TYPE_NAD);
	//
	EXPECT_STREQ(	data->inputs->signals[3].name,			"Signal11");
	EXPECT_EQ(		data->inputs->signals[3].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[3].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[3].size[0],		1);
	EXPECT_EQ(		data->inputs->signals[3].sample_size,	1);
	EXPECT_EQ(		data->inputs->signals[3].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[3].sample_time,	0.01F);	
	EXPECT_EQ(		data->inputs->signals[3].signal_type,	ED247_SIGNAL_TYPE_NAD);

	// Outputs
	EXPECT_EQ(data->outputs->nstreams,0);
	EXPECT_EQ(data->outputs->nsignals,0);

	// [ TEARDOWN ]
	io_free_memory(data);

 }
 
 TEST_F(NADTest, NADConfiguration2)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/nad_mc_2.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					0);
	EXPECT_EQ(		data->inputs->nsignals,					0);

	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,				2);
	EXPECT_EQ(		data->outputs->nsignals,				3);

	//
	EXPECT_STREQ(	data->outputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		data->outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->streams[0].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->outputs->streams[0].nsignals,		2);	
	EXPECT_EQ(		&(data->outputs->signals[0]),			data->outputs->streams[0].signals[0]);
	EXPECT_EQ(		&(data->outputs->signals[1]),			data->outputs->streams[0].signals[1]);
	//
	EXPECT_STREQ(	data->outputs->signals[0].name,			"Signal00");
	EXPECT_EQ(		data->outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[0].size[0],		1);
	EXPECT_EQ(		data->outputs->signals[0].sample_size,	1);
	EXPECT_EQ(		data->outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[0].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[0].signal_type,	ED247_SIGNAL_TYPE_NAD);
	//
	EXPECT_STREQ(	data->outputs->signals[1].name,			"Signal01");
	EXPECT_EQ(		data->outputs->signals[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[1].size[0],		1);
	EXPECT_EQ(		data->outputs->signals[1].sample_size,	1);
	EXPECT_EQ(		data->outputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[1].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[1].signal_type,	ED247_SIGNAL_TYPE_NAD);

	//
	EXPECT_STREQ(	data->outputs->streams[1].name,			"Stream1");
	EXPECT_EQ(		data->outputs->streams[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->outputs->streams[1].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->outputs->streams[1].nsignals,		1);	
	EXPECT_EQ(		&(data->outputs->signals[2]),			data->outputs->streams[1].signals[0]);
	//
	EXPECT_STREQ(	data->outputs->signals[2].name,			"Signal11");
	EXPECT_EQ(		data->outputs->signals[2].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[2].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[2].size[0],		1);
	EXPECT_EQ(		data->outputs->signals[2].sample_size,	1);
	EXPECT_EQ(		data->outputs->signals[2].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[2].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[2].signal_type,	ED247_SIGNAL_TYPE_NAD);

	// [ TEARDOWN ]
	io_free_memory(data);

 }
 
 TEST_F(NADTest, NADConfigurationWithDimensions)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/nad_mc_4.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					0);
	EXPECT_EQ(		data->inputs->nsignals,					0);

	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,				2);
	EXPECT_EQ(		data->outputs->nsignals,				3);

	//
	EXPECT_STREQ(	data->outputs->streams[0].name,			"Stream0");
	EXPECT_EQ(		data->outputs->streams[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->streams[0].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->outputs->streams[0].nsignals,		2);
	EXPECT_EQ(		&(data->outputs->signals[0]),			data->outputs->streams[0].signals[0]);
	EXPECT_EQ(		&(data->outputs->signals[1]),			data->outputs->streams[0].signals[1]);
	//
	EXPECT_STREQ(	data->outputs->signals[0].name,			"Signal00");
	EXPECT_EQ(		data->outputs->signals[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[0].dimensions,	2);
	EXPECT_EQ(		data->outputs->signals[0].size[0],		2);
	EXPECT_EQ(		data->outputs->signals[0].size[1],		3);
	EXPECT_EQ(		data->outputs->signals[0].sample_size,	6);
	EXPECT_EQ(		data->outputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[0].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[0].signal_type,	ED247_SIGNAL_TYPE_NAD);
	//
	EXPECT_STREQ(	data->outputs->signals[1].name,			"Signal01");
	EXPECT_EQ(		data->outputs->signals[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->outputs->signals[1].size[0],		4);
	EXPECT_EQ(		data->outputs->signals[1].sample_size,	4);
	EXPECT_EQ(		data->outputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[1].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[1].signal_type,	ED247_SIGNAL_TYPE_NAD);

	//
	EXPECT_STREQ(	data->outputs->streams[1].name,			"Stream1");
	EXPECT_EQ(		data->outputs->streams[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->streams[1].stream_type,	ED247_STREAM_TYPE_NAD);
	EXPECT_EQ(		data->outputs->streams[1].nsignals,		1);	
	EXPECT_EQ(		&(data->outputs->signals[2]),			data->outputs->streams[1].signals[0]);
	//
	EXPECT_STREQ(	data->outputs->signals[2].name,			"Signal11");
	EXPECT_EQ(		data->outputs->signals[2].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->outputs->signals[2].dimensions,	3);
	EXPECT_EQ(		data->outputs->signals[2].size[0],		2);
	EXPECT_EQ(		data->outputs->signals[2].size[1],		3);
	EXPECT_EQ(		data->outputs->signals[2].size[2],		4);
	EXPECT_EQ(		data->outputs->signals[2].sample_size,	24);
	EXPECT_EQ(		data->outputs->signals[2].type,			SS_UINT8);
	EXPECT_EQ(		data->outputs->signals[2].sample_time,	0.01F);
	EXPECT_EQ(		data->outputs->signals[2].signal_type,	ED247_SIGNAL_TYPE_NAD);

	// [ TEARDOWN ]
	io_free_memory(data);

 }
 
  TEST_F(NADTest, NADTransmission)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/nad_mc_1.xml";
	std::string recvconfiguration = filefolder_ + "/nad_mc_2.xml";

	// [ SETUP ]
	io_allocate_memory(&senddata);
	io_allocate_memory(&recvdata);

	cstatus = read_ed247_configuration(sendconfiguration.c_str(),senddata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = read_ed247_configuration(recvconfiguration.c_str(),recvdata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	for (i = 0; i < senddata->inputs->nsignals; i++){
		senddata->inputs->signals[i].valuePtr = (void*) &(sendvalues[i]);
	}
	for (i = 0; i < recvdata->outputs->nsignals; i++){
		recvdata->outputs->signals[i].valuePtr = (void*) &(recvvalues[i]);
	}	
	nrecv = 0;

	// [ EXERCISE ]
	sendvalues[0] = 1;sendvalues[1] = 2;sendvalues[2] = 3;sendvalues[3] = 4;

	sstatus = send_simulink_to_ed247(senddata);
	rstatus = receive_ed247_to_simulink(recvdata, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr), sendvalues[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr), sendvalues[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr), sendvalues[3]);

	// [ TEARDOWN ]
	io_free_memory(senddata);
	io_free_memory(recvdata);

 }
 
 TEST_F(NADTest, NADTransmissionWithDimensions)
 {
	int i,nrecv;
	unsigned char sendvalues01[2][3];
	unsigned char sendvalues02[4];
	unsigned char sendvalues03;
	unsigned char sendvalues04[2][3][4];
	unsigned char recvvalues01[2][3];
	unsigned char recvvalues02[4];
	unsigned char recvvalues03[2][3][4];
	memset(sendvalues01,0,2*3*sizeof(char));
	memset(sendvalues02,0,4*sizeof(char));
	memset(sendvalues04,0,2*3*4*sizeof(char));
	memset(recvvalues01,0,2*3*sizeof(char));
	memset(recvvalues02,0,4*sizeof(char));
	memset(recvvalues03,0,2*3*4*sizeof(char));

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/nad_mc_3.xml";
	std::string recvconfiguration = filefolder_ + "/nad_mc_4.xml";

	// [ SETUP ]
	io_allocate_memory(&senddata);
	io_allocate_memory(&recvdata);

	cstatus = read_ed247_configuration(sendconfiguration.c_str(),senddata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = read_ed247_configuration(recvconfiguration.c_str(),recvdata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	senddata->inputs->signals[0].valuePtr = (void*) sendvalues01;
	senddata->inputs->signals[1].valuePtr = (void*) sendvalues02;
	senddata->inputs->signals[2].valuePtr = (void*) &sendvalues03;
	senddata->inputs->signals[3].valuePtr = (void*) sendvalues04;

	recvdata->outputs->signals[0].valuePtr = (void*) recvvalues01;
	recvdata->outputs->signals[1].valuePtr = (void*) recvvalues02;
	recvdata->outputs->signals[2].valuePtr = (void*) recvvalues03;
	
	nrecv = 0;

	// [ EXERCISE ]
	sendvalues01[0][0]		=  1;
	sendvalues01[0][1]		=  2;
	sendvalues01[0][2]		=  3;
	sendvalues01[1][0]		=  4;
	sendvalues01[1][1]		=  5;
	sendvalues01[1][2]		=  6;
	sendvalues02[0]			=  7;
	sendvalues02[1]			=  8;
	sendvalues02[2]			=  9;
	sendvalues02[3]			= 10;
	sendvalues03			= 11;
	sendvalues04[0][0][0]	= 12;
	sendvalues04[0][0][1]	= 13;
	sendvalues04[0][0][2]	= 14;
	sendvalues04[0][0][3]	= 15;
	sendvalues04[0][1][0]	= 16;
	sendvalues04[0][1][1]	= 17;
	sendvalues04[0][1][2]	= 18;
	sendvalues04[0][1][3]	= 19;
	sendvalues04[0][2][0]	= 20;
	sendvalues04[0][2][1]	= 21;
	sendvalues04[0][2][2]	= 22;
	sendvalues04[0][2][3]	= 23;
	sendvalues04[1][0][0]	= 24;
	sendvalues04[1][0][1]	= 25;
	sendvalues04[1][0][2]	= 26;
	sendvalues04[1][0][3]	= 27;
	sendvalues04[1][1][0]	= 28;
	sendvalues04[1][1][1]	= 29;
	sendvalues04[1][1][2]	= 30;
	sendvalues04[1][1][3]	= 31;
	sendvalues04[1][2][0]	= 32;
	sendvalues04[1][2][1]	= 33;
	sendvalues04[1][2][2]	= 34;
	sendvalues04[1][2][3]	= 35;

	sstatus = send_simulink_to_ed247(senddata);
	rstatus = receive_ed247_to_simulink(recvdata, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  0, sendvalues01[0][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  1, sendvalues01[0][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  2, sendvalues01[0][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  3, sendvalues01[1][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  4, sendvalues01[1][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr) +  5, sendvalues01[1][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr) +  0, sendvalues02[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr) +  1, sendvalues02[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr) +  2, sendvalues02[2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr) +  3, sendvalues02[3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  0, sendvalues04[0][0][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  1, sendvalues04[0][0][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  2, sendvalues04[0][0][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  3, sendvalues04[0][0][3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  4, sendvalues04[0][1][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  5, sendvalues04[0][1][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  6, sendvalues04[0][1][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  7, sendvalues04[0][1][3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  8, sendvalues04[0][2][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) +  9, sendvalues04[0][2][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 10, sendvalues04[0][2][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 11, sendvalues04[0][2][3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 12, sendvalues04[1][0][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 13, sendvalues04[1][0][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 14, sendvalues04[1][0][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 15, sendvalues04[1][0][3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 16, sendvalues04[1][1][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 17, sendvalues04[1][1][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 18, sendvalues04[1][1][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 19, sendvalues04[1][1][3]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 20, sendvalues04[1][2][0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 21, sendvalues04[1][2][1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 22, sendvalues04[1][2][2]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr) + 23, sendvalues04[1][2][3]);

	// [ TEARDOWN ]
	io_free_memory(senddata);
	io_free_memory(recvdata);

 }
 