/*
 * Copyright 2020 The MathWorks, Inc.
 */

 #include "gtest/gtest.h"
 
 #include "testutils.h"
 
 #include <iostream>
 #include <cstdlib>
 #include <string>
   
 extern "C" {
	 #include "ed247_cmd_xml.h"
 }
 
 TEST_F(ConfigurationTest, InvalidICDFile)
 {

	/* [ SETUP ] */
	cmd_read_status_t status;
	cmd_data_t *data;
	const char *filename = "";

	/* [ EXERCISE ] */
	status = cmd_read_data(filename,data);

	/* [ VERIFY ] */
	EXPECT_EQ(status, CMD_READ_INVALID_FILE);

 }
   
 TEST_F(ConfigurationTest, BridgeMessageDescription)
 {
	
	cmd_read_status_t status;
	cmd_data_t *data;
	std::string filename = filefolder_ + "/bridge_messages_description.xml";

	/* [ EXERCISE ] */
	cmd_read_data(filename.c_str(), data);
	
	/* [ VERIFY ] */
	// Number of elements
    EXPECT_EQ(data->counter.a664, 2); // A664 messages
    EXPECT_EQ(data->counter.a429, 4); // A429 buses
    EXPECT_EQ(data->counter.nad, 5); // NAD variables
		
	// A664 details
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
	
	// A429 details
	// ------------
	//
	EXPECT_STREQ(	data->a429[0].name,						"T11M4_A429_SIMU2SWIM_BUS_1_I");
	EXPECT_EQ(		data->a429[0].n_messages,				4);
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
	EXPECT_STREQ(	data->a429[0].messages[3].name,			"T11M4_A429_SIMU2SWIM_BUS_1_304_DD_I");
	EXPECT_EQ(		data->a429[0].messages[3].direction,	ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->a429[0].messages[3].type,			A429_MESSAGE_QUEUING);
	EXPECT_STREQ(	data->a429[0].messages[3].label,		"304");
	EXPECT_STREQ(	data->a429[0].messages[3].sdi,			"");
	EXPECT_EQ(		data->a429[0].messages[3].period_us,	0);
	EXPECT_STREQ(	data->a429[0].messages[3].comment,		"T11_T11_PART1");		
	
	//
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
	//
	EXPECT_STREQ(	data->a429[2].name,						"T11M4_A429_SWIM2SIMU_BUS_1_O");
	EXPECT_EQ(		data->a429[2].n_messages,				4);
	EXPECT_STREQ(	data->a429[2].messages[0].name,			"T11M4_A429_SWIM2SIMU_BUS_1_350_10_O");
	EXPECT_EQ(		data->a429[2].messages[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[2].messages[0].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[2].messages[0].label,		"350");
	EXPECT_STREQ(	data->a429[2].messages[0].sdi,			"10");
	EXPECT_EQ(		data->a429[2].messages[0].period_us,	0);
	EXPECT_STREQ(	data->a429[2].messages[0].comment,		"T11_T11_PART1");	
	EXPECT_STREQ(	data->a429[2].messages[1].name,			"T11M4_A429_SWIM2SIMU_BUS_1_200_10_O");
	EXPECT_EQ(		data->a429[2].messages[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[2].messages[1].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[2].messages[1].label,		"200");
	EXPECT_STREQ(	data->a429[2].messages[1].sdi,			"10");
	EXPECT_EQ(		data->a429[2].messages[1].period_us,	0);
	EXPECT_STREQ(	data->a429[2].messages[1].comment,		"T11_T11_PART1");
	EXPECT_STREQ(	data->a429[2].messages[2].name,			"T11M4_A429_SWIM2SIMU_BUS_1_200_11_O");
	EXPECT_EQ(		data->a429[2].messages[2].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[2].messages[2].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[2].messages[2].label,		"200");
	EXPECT_STREQ(	data->a429[2].messages[2].sdi,			"11");
	EXPECT_EQ(		data->a429[2].messages[2].period_us,	0);
	EXPECT_STREQ(	data->a429[2].messages[2].comment,		"T11_T11_PART1");
	EXPECT_STREQ(	data->a429[2].messages[3].name,			"T11M4_A429_SWIM2SIMU_BUS_1_304_DD_O");
	EXPECT_EQ(		data->a429[2].messages[3].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[2].messages[3].type,			A429_MESSAGE_QUEUING);
	EXPECT_STREQ(	data->a429[2].messages[3].label,		"304");
	EXPECT_STREQ(	data->a429[2].messages[3].sdi,			"");
	EXPECT_EQ(		data->a429[2].messages[3].period_us,	0);
	EXPECT_STREQ(	data->a429[2].messages[3].comment,		"T11_T11_PART1");	
	//
	EXPECT_STREQ(	data->a429[3].name,						"T11M4_A429_SWIM2SIMU_BUS_2_O");
	EXPECT_EQ(		data->a429[3].n_messages,				2);
	EXPECT_STREQ(	data->a429[3].messages[0].name,			"T11M4_A429_SWIM2SIMU_BUS_2_200_11_O");
	EXPECT_EQ(		data->a429[3].messages[0].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[3].messages[0].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[3].messages[0].label,		"200");
	EXPECT_STREQ(	data->a429[3].messages[0].sdi,			"11");
	EXPECT_EQ(		data->a429[3].messages[0].period_us,	0);
	EXPECT_STREQ(	data->a429[3].messages[0].comment,		"T11_T11_PART1");	
	EXPECT_STREQ(	data->a429[3].messages[1].name,			"T11M4_A429_SWIM2SIMU_BUS_2_200_10_O");
	EXPECT_EQ(		data->a429[3].messages[1].direction,	ED247_DIRECTION_IN);
	EXPECT_EQ(		data->a429[3].messages[1].type,			A429_MESSAGE_SAMPLING);
	EXPECT_STREQ(	data->a429[3].messages[1].label,		"200");
	EXPECT_STREQ(	data->a429[3].messages[1].sdi,			"10");
	EXPECT_EQ(		data->a429[3].messages[1].period_us,	0);
	EXPECT_STREQ(	data->a429[3].messages[1].comment,		"T11_T11_PART1");

	// NAD details
	// ------------
	//
	EXPECT_STREQ(	data->nad[0].name,						"CLUSTER_T1_MIF");
	EXPECT_EQ(		data->nad[0].direction,					ED247_DIRECTION_IN);
	EXPECT_EQ(		data->nad[0].type,						ED247_NAD_TYPE_UINT32);
	EXPECT_STREQ(	data->nad[0].comment,					"T1");
	//
	EXPECT_STREQ(	data->nad[1].name,						"CPIOM_T11_STATE");
	EXPECT_EQ(		data->nad[1].direction,					ED247_DIRECTION_IN);
	EXPECT_EQ(		data->nad[1].type,						ED247_NAD_TYPE_UINT32);
	EXPECT_STREQ(	data->nad[1].comment,					"T11");
	//
	EXPECT_STREQ(	data->nad[2].name,						"CPIOM_T11_OPS");
	EXPECT_EQ(		data->nad[2].direction,					ED247_DIRECTION_IN);
	EXPECT_EQ(		data->nad[2].type,						ED247_NAD_TYPE_UINT32);
	EXPECT_STREQ(	data->nad[2].comment,					"T11");
	//
	EXPECT_STREQ(	data->nad[3].name,						"CPIOM_T11_LRM_RESET");
	EXPECT_EQ(		data->nad[3].direction,					ED247_DIRECTION_OUT);
	EXPECT_EQ(		data->nad[3].type,						ED247_NAD_TYPE_UINT32);
	EXPECT_STREQ(	data->nad[3].comment,					"T11");
	//
	EXPECT_STREQ(	data->nad[4].name,						"CPIOM_T11_HELLO");
	EXPECT_EQ(		data->nad[4].direction,					ED247_DIRECTION_IN);
	EXPECT_EQ(		data->nad[4].type,						ED247_NAD_TYPE_UINT8);
	EXPECT_STREQ(	data->nad[4].comment,					"T1");

 }