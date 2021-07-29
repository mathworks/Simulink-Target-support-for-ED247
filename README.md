# Simulink ​support for ED247 communication standard

The "Simulink ​support for ED247 communication standard" enables ED247 communication inside a Simulink models.
It provides Simulink blocks in a library to send and receive data in various formats (A429, A664, A825, NAD, VNAD, Analogic, Discrete).

## Setup 

1. Open project in MATLAB
1. Configure dependencies
	1. Create configuration variable
		>> config = ed247.Configuration.default()
	1. Set ED247 library installation folder
		>> config.ED247 = "<ed247_installation folder>";
	1. (Windows) Set MinGW64 installation folder
		>> config.MinGW = "<mingw64_installation_root_folder>";
	1. Save configuration
		>> save(config)

Additional information about set up

### MathWorks Products (http://www.mathworks.com)

Requires MATLAB release R2020b or newer
- [Simulink](https://www.mathworks.com/products/simulink.html)

Optional (for Real-Time execution on Speedgoat)
- [Simulink Real-Time](https://www.mathworks.com/products/simulink-real-time.html)

### 3rd Party Products:
3p:
- [ED247 library](https://github.com/airbus/ED247_LIBRARY)
- (Windows) [MinGW64](https://url-to-product2)

## Installation

Before proceeding, ensure that the below products are installed:  
* [ED247 library](https://github.com/airbus/ED247_LIBRARY) 

### Compile S-Function

S-Function can be compiled using the following command:

>> ed247.compile()

## Deployment Steps

### Speedgoat execution

1. Configure model for deployment on Speedgoat
	- Set solver type to Fixed-Step (Configuration Parameters -> Solver -> Solver Type)
	- Select slrealtime.tlc (Configuration Parameters -> Code Generation -> System Target File)
1. Compile executable
	>> ed247.Speedgoat.deployModel(<modelname>)
1. Download and execute
	1. Open Simulink Real-Time Explorer
		>> slrtExplorer
	1. Connect development computer (host) to Speedgoat target
	1. Load application (<modelname>.mldatx)
	1. Run application

### Custom toolbox

Following commands will test, compile and package source code into a MATLAB custom toolbox which allows an easy sharing of Simulink ​support for ED247 communication standard

>> p = ci.Pipeline.forFolder();
>> run(p)

## Getting Started 
Information about Getting Started

1. Create a Simulink model
1. Add ED247 configuration block to model
	- Type "ED247 Configuration" in the model
OR
	- Open Simulink Library Browser and navigate to library "ED247"

## Unit tests

Unit tests are located in tests/ folder.
Unit tests can be run using following command:

>> p = ci.Pipeline.forFolder();
>> test(p) 

### Note

- Few tests are using [Parallel Computing Toolbox](https://www.mathworks.com/products/parallel-computing.html)

## Examples

To learn how to use this in testing workflows, see [Examples](/doc/examples/). 

Each example is stored in a folder which contains a Simulink model and XML configuration files (ECIC and ICD).

- receiveA429 : Receive ARINC 429 messages
- sendA429 : Send ARINC 429 messages
- receiveNAD : Receive Non-Avionic data

## License

The license for <insert repo name> is available in the [LICENSE.TXT](LICENSE.TXT) file in this GitHub repository.

Include any other License information here, including third-party content using separate license agreements 

## Community Support
[MATLAB Central](https://www.mathworks.com/matlabcentral)

Copyright 2021 The MathWorks, Inc.

