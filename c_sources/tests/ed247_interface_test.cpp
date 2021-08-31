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
	IO_t *data;
	const char *filename = "";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&data);

	// [ EXERCISE ]
	status = interface.readED247Configuration(filename,data,NULL);

	// [ VERIFY ]
	EXPECT_EQ(status, INVALID_FILE);

	// [ TEARDOWN ]
	interface.ioFreeMemory(data);

 }

 TEST_F(TransmissionTest, ANATransmission)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/ana_mc_1.xml";	
	std::string recvconfiguration = filefolder_ + "/ana_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&senddata);
	interface.ioAllocateMemory(&recvdata);

	cstatus = interface.readED247Configuration(sendconfiguration.c_str(),senddata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = interface.readED247Configuration(recvconfiguration.c_str(),recvdata,NULL);
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

	sstatus = interface.sendSimulinkToED247(senddata);
	rstatus = interface.receiveED247ToSimulink(recvdata, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr), sendvalues[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr), sendvalues[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr), sendvalues[2]);

	// [ TEARDOWN ]
	interface.ioFreeMemory(senddata);
	interface.ioFreeMemory(recvdata);

 }
 
 TEST_F(TransmissionTest, DISTransmission)
 {
	int i,nrecv;
	unsigned char sendvalues[4] = {0,0,0,0};
	unsigned char recvvalues[3] = {0,0,0};

	configuration_status_t cstatus;
	send_status_t sstatus;
	receive_status_t rstatus;

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/dis_mc_1.xml";
	std::string recvconfiguration = filefolder_ + "/dis_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&senddata);
	interface.ioAllocateMemory(&recvdata);

	cstatus = interface.readED247Configuration(sendconfiguration.c_str(),senddata,NULL);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);
	cstatus = interface.readED247Configuration(recvconfiguration.c_str(),recvdata,NULL);
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

	sstatus = interface.sendSimulinkToED247(senddata);
	rstatus = interface.receiveED247ToSimulink(recvdata, &nrecv);

	ASSERT_EQ(sstatus, SEND_OK);
	ASSERT_EQ(rstatus, RECEIVE_OK);

	// [ VERIFY ]
	EXPECT_EQ(nrecv,3);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[0].valuePtr), sendvalues[0]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[1].valuePtr), sendvalues[1]);
	EXPECT_EQ(*((unsigned char *)recvdata->outputs->signals[2].valuePtr), sendvalues[3]);

	// [ TEARDOWN ]
	interface.ioFreeMemory(senddata);
	interface.ioFreeMemory(recvdata);

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

	IO_t *senddata;
	IO_t *recvdata;
	std::string sendconfiguration = filefolder_ + "/dis_mc_1.xml";
	std::string recvconfiguration = filefolder_ + "/dis_mc_2.xml";

	ed247simulink::Tools tools;
	ed247simulink::Interface interface = ed247simulink::Interface(tools);

	// [ SETUP ]
	interface.ioAllocateMemory(&senddata);
	interface.ioAllocateMemory(&recvdata);

	sprintf(logfilename,"%s/LogFileTest.log", filefolder_.c_str());

	cstatus = interface.readED247Configuration(sendconfiguration.c_str(),senddata,(const char*)logfilename);
	ASSERT_EQ(cstatus, CONFIGURATION_SUCCESS);

	for (i = 0; i < senddata->inputs->nsignals; i++){
		senddata->inputs->signals[i].valuePtr = (void*) &(sendvalues[i]);
	}

	// [ EXERCISE ]
	sendvalues[0] = 1;sendvalues[1] = 2;sendvalues[2] = 3;sendvalues[3] = 4;
	sstatus = interface.sendSimulinkToED247(senddata);
	ASSERT_EQ(sstatus, SEND_OK);

	// [ VERIFY ]
	EXPECT_EQ(access( logfilename, F_OK ),0);

	// [ TEARDOWN ]
	interface.ioFreeMemory(senddata);

 }

