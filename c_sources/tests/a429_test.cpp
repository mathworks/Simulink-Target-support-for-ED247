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

 TEST_F(A429Test, A429ICD)
 {

	cmd_read_status_t status;
	cmd_data_t *data;
	std::string filename = filefolder_ + "/a429_cmd_simple01.xml";

	/* [ EXERCISE ] */
	cmd_read_data(filename.c_str(), data);

	/* [ VERIFY ] */
	// Number of elements
	EXPECT_EQ(data->counter.a429, 2);

	// Details
	// ------------
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

 }

 TEST_F(A429Test, A429Configuration)
 {

	configuration_status_t status;
	IO_t *data;
	std::string filename = filefolder_ + "/a429_mc_simple01.xml";

	// [ SETUP ]
	io_allocate_memory(&data);

	// [ EXERCISE ]
	status = read_ed247_configuration(filename.c_str(),data,NULL);
	ASSERT_EQ(status, CONFIGURATION_SUCCESS);

	// [ VERIFY ]
	// Inputs
	EXPECT_EQ(		data->inputs->nstreams,					2);
	EXPECT_EQ(		data->inputs->nsignals,					5);

	//
	EXPECT_STREQ(	data->inputs->streams[0].name,			"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		data->inputs->streams[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[0].stream_type,	ED247_STREAM_TYPE_A429);
	EXPECT_STREQ(	data->inputs->streams[0].icd,			"a429_cmd_simple01.xml");
	EXPECT_STREQ(	data->inputs->streams[0].bus,			"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		data->inputs->streams[0].nsignals,		3);
	EXPECT_EQ(		&(data->inputs->signals[0]),			data->inputs->streams[0].signals[0]);
	EXPECT_EQ(		&(data->inputs->signals[1]),			data->inputs->streams[0].signals[1]);
	EXPECT_EQ(		&(data->inputs->signals[2]),			data->inputs->streams[0].signals[2]);
	//
	EXPECT_STREQ(	data->inputs->signals[0].name,			"T11M4_A429_SIMU2SWIM_BUS_1_350_10_I");
	EXPECT_EQ(		data->inputs->signals[0].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[0].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[0].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[0].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[0].sample_time,	1.F);
	//
	EXPECT_STREQ(	data->inputs->signals[1].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_10_I");
	EXPECT_EQ(		data->inputs->signals[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[1].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[1].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[1].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[1].sample_time,	1.F);
	//
	EXPECT_STREQ(	data->inputs->signals[2].name,			"T11M4_A429_SIMU2SWIM_BUS_1_200_11_I");
	EXPECT_EQ(		data->inputs->signals[2].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[2].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[2].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[2].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[2].sample_time,	1.F);

	//
	EXPECT_STREQ(	data->inputs->streams[1].name,			"T11M4_A429_SIMU2SWIM_BUS_2_I");
	EXPECT_EQ(		data->inputs->streams[1].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->streams[1].stream_type,	ED247_STREAM_TYPE_A429);
	EXPECT_STREQ(	data->inputs->streams[1].icd,			"a429_cmd_simple01.xml");
	EXPECT_STREQ(	data->inputs->streams[1].bus,			"T11M4_A429_SIMU2SWIM_BUS_2_I");
	EXPECT_EQ(		data->inputs->streams[1].nsignals,		2);
	EXPECT_EQ(		&(data->inputs->signals[3]),			data->inputs->streams[1].signals[0]);
	EXPECT_EQ(		&(data->inputs->signals[4]),			data->inputs->streams[1].signals[1]);
	//
	EXPECT_STREQ(	data->inputs->signals[3].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_11_I");
	EXPECT_EQ(		data->inputs->signals[3].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[3].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[3].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[3].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[3].sample_time,	1.F);
	//
	EXPECT_STREQ(	data->inputs->signals[4].name,			"T11M4_A429_SIMU2SWIM_BUS_2_200_10_I");
	EXPECT_EQ(		data->inputs->signals[4].direction,		ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->inputs->signals[4].type,			SS_UINT8);
	EXPECT_EQ(		data->inputs->signals[4].dimensions,	1);
	EXPECT_EQ(		data->inputs->signals[4].size[0],		4);
	EXPECT_EQ(		data->inputs->signals[4].sample_time,	1.F);

	// Outputs
	EXPECT_EQ(		data->outputs->nstreams,				0);

	// [ TEARDOWN ]
	io_free_memory(data);

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

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/a429_mc_simple01.xml";
	std::string recvconfiguration = filefolder_ + "/a429_mc_simple02.xml";

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

	sstatus = send_simulink_to_ed247(senddata);
	rstatus = receive_ed247_to_simulink(recvdata, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,5);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[0].valuePtr + 0), sendvalues[0][0]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[0].valuePtr + 1), sendvalues[0][1]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[0].valuePtr + 2), sendvalues[0][2]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[0].valuePtr + 3), sendvalues[0][3]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[1].valuePtr + 0), sendvalues[1][0]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[1].valuePtr + 1), sendvalues[1][1]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[1].valuePtr + 2), sendvalues[1][2]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[1].valuePtr + 3), sendvalues[1][3]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[2].valuePtr + 0), sendvalues[2][0]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[2].valuePtr + 1), sendvalues[2][1]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[2].valuePtr + 2), sendvalues[2][2]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[2].valuePtr + 3), sendvalues[2][3]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[3].valuePtr + 0), sendvalues[3][0]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[3].valuePtr + 1), sendvalues[3][1]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[3].valuePtr + 2), sendvalues[3][2]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[3].valuePtr + 3), sendvalues[3][3]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[4].valuePtr + 0), sendvalues[4][0]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[4].valuePtr + 1), sendvalues[4][1]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[4].valuePtr + 2), sendvalues[4][2]);
	EXPECT_EQ(*((char *)recvdata->outputs->signals[4].valuePtr + 3), sendvalues[4][3]);

	// [ TEARDOWN ]
	io_free_memory(senddata);
	io_free_memory(recvdata);

 }
 