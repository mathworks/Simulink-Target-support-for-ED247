#include "ed247_sfun_configure.h"

namespace ed247sfcn {

	/*
	 * CONSTRUCTORS
	 */
	Configure::Configure(SimStruct *S, ed247simulink::ED247Connector* connector){
		_S = S;
		_connector = connector;
		_tools = new ed247simulink::Tools();
	}

	Configure::Configure(SimStruct *S, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools){
		_S = S;
		_connector = connector;
		_tools = tools;
	}

	/*
	 * PUBLIC METHODS
	 */
	configuration_status_t Configure::initialize(){

		configuration_status_t status;

		ssSetNumDWork(_S, 0);

		_tools->myprintf("\n\n=== CONFIGURATION INITIALIZATION START ===\n");

		_connector->allocateMemory();

		status = _connector->readED247Configuration();
		_tools->myprintf("Read status = %d\n", status);

		_tools->myprintf("%d input streams, %d input signals\n", _connector->getInputs()->nstreams, _connector->getInputs()->nsignals);
		_tools->myprintf("%d output streams, %d output signals\n", _connector->getOutputs()->nstreams, _connector->getOutputs()->nsignals);

		_tools->myprintf("\n=== CONFIGURATION INITIALIZATION END ===\n\n");

		return status;

	}

	void Configure::start(){}

	void Configure::outputs(){}

	void Configure::update(){}

	void Configure::terminate(){
		//_connector->freeMemory();
	}

	void Configure::RTW(real_T* blockTypeID, char_T* configurationFile){

		char *base;

		char_T* filename = (char_T *)( mxGetData(ssGetSFcnParam(_S,1)) );

		base = strtok(filename, "\\");
		while(base) {
			strcpy(filename, base);
			base = strtok(NULL, "\\");
		}
		strncpy( configurationFile, filename, strlen(filename) + 1 );

		*blockTypeID = 0.0;

	}

}
