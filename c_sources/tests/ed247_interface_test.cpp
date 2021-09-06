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
 
 TEST_F(ConfigurationTest, InvalidECICFile)
 {

	configuration_status_t status;
	const char *filename = "";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector connector = ed247simulink::ED247Connector(tools);

	// [ SETUP ]
	connector.allocateMemory();

	// [ EXERCISE ]
	status = connector.readED247Configuration(filename,NULL);

	// [ VERIFY ]
	EXPECT_EQ(status, INVALID_FILE);

	// [ TEARDOWN ]
	connector.freeMemory();

 }

 TEST_F(TransmissionTest, ANATransmission)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* senddata;
	data_characteristics_t* recvdata;

	std::string sendconfiguration = filefolder_ + "/ana_mc_1.xml";	
	std::string recvconfiguration = filefolder_ + "/ana_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector sendconnector = ed247simulink::ED247Connector(tools);
	ed247simulink::ED247Connector recvconnector = ed247simulink::ED247Connector(tools);

	// [ SETUP ]
	sendconnector.allocateMemory();
	recvconnector.allocateMemory();

	cstatus = sendconnector.readED247Configuration(sendconfiguration.c_str(),NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = recvconnector.readED247Configuration(recvconfiguration.c_str(),NULL);
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
	sendvalues[0] = 1;sendvalues[1] = 2;sendvalues[2] = 3;sendvalues[3] = 4;

	sstatus = sendconnector.sendSimulinkToED247();
	rstatus = recvconnector.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[0].valuePtr), sendvalues[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[1].valuePtr), sendvalues[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[2].valuePtr), sendvalues[2]);

	// [ TEARDOWN ]
	sendconnector.freeMemory();
	recvconnector.freeMemory();

 }

 TEST_F(TransmissionTest, DISTransmission)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* senddata;
	data_characteristics_t* recvdata;
	std::string sendconfiguration = filefolder_ + "/dis_mc_1.xml";
	std::string recvconfiguration = filefolder_ + "/dis_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector sendconnector = ed247simulink::ED247Connector(tools);
	ed247simulink::ED247Connector recvconnector = ed247simulink::ED247Connector(tools);

	// [ SETUP ]
	sendconnector.allocateMemory();
	recvconnector.allocateMemory();

	cstatus = sendconnector.readED247Configuration(sendconfiguration.c_str(),NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = recvconnector.readED247Configuration(recvconfiguration.c_str(),NULL);
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
	sendvalues[0] = 1;sendvalues[1] = 2;sendvalues[2] = 3;sendvalues[3] = 4;

	sstatus = sendconnector.sendSimulinkToED247();
	rstatus = recvconnector.receiveED247ToSimulink(&nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[0].valuePtr), sendvalues[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[1].valuePtr), sendvalues[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->signals[2].valuePtr), sendvalues[3]);

	// [ TEARDOWN ]
	sendconnector.freeMemory();
	recvconnector.freeMemory();

 }
 
 TEST_F(TransmissionTest, Logging)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};
	char logfilename[512];

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	data_characteristics_t* senddata;
	std::string sendconfiguration = filefolder_ + "/dis_mc_1.xml";

	ed247simulink::Tools tools;
	ed247simulink::ED247Connector sendconnector = ed247simulink::ED247Connector(tools);

	// [ SETUP ]
	sendconnector.allocateMemory();

	sprintf(logfilename,"%s/LogFileTest.log", filefolder_.c_str());

	cstatus = sendconnector.readED247Configuration(sendconfiguration.c_str(),(const char*)logfilename);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	senddata = sendconnector.getInputs();

	for (i = 0; i < senddata->nsignals; i++){
		senddata->signals[i].valuePtr = (void*) &(sendvalues[i]);
	}

	// [ EXERCISE ]
	sendvalues[0] = 1;sendvalues[1] = 2;sendvalues[2] = 3;sendvalues[3] = 4;
	sstatus = sendconnector.sendSimulinkToED247();
	ASSERT_EQ(sstatus, SEND_OK);

	// [ VERIFY ]
	EXPECT_EQ(access( logfilename, F_OK ),0);

	// [ TEARDOWN ]
	sendconnector.freeMemory();

 }

