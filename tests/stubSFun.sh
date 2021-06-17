#!/bin/bash
#
# Syntax
#	./stubSFun.sh
#	./stubSFun.sh <archivename> <configurationfile>
#	DEBUG=1 ./stubSFun.sh
#
# Inputs
#	archivename : Name of ZIP archive (without extension) containing configuration files, ex. myconfig
#	configurationfile : Name of ECIC configuration file (without extension), ex. myECIC
#
# Copyright 2020 The MathWorks, Inc.
#

# Tools
MINGW_FOLDER=/c/Files/PROGRAMS/ThirdParty/MinGW64/4.9.2-airbus/bin
#CMAKE_FOLDER=/c/Files/PROGRAMS/ThirdParty/cmake/3.20.2

# Application
ROOT_FOLDER=$(cd `dirname $0` && pwd)
ROOT_FOLDER=$(dirname $ROOT_FOLDER)
BUILD_FOLDER=${ROOT_FOLDER}/work/stubbed_sfun
SOURCEFOLDER=${ROOT_FOLDER}/tests/stubbed_sfun
TESTFILEFOLDER=${ROOT_FOLDER}/tests/_files

EXE=stubbedSFun.exe

# Inputs
ARCHIVENAME=${1:-"A429_simple01"}
CONFIGURATIONFILE=${2:-"a429_01_ecic_in"}

# Environment variables
CC_OPTS=""
if [ ! -z "$DEBUG" ]; then
	CC_OPTS="${CC_OPTS} -DDEBUG -DDEFINE_DEBUG=ON"
	LOGFILE=
fi

export CC=${MINGW_FOLDER}/gcc.exe
export CXX=${MINGW_FOLDER}/g++.exe
export AR=${MINGW_FOLDER}/ar.exe

export PATH=${ROOT_FOLDER}/../ED247_LIBRARY/_install/lib:${PATH}

echo "## Delete previous build folder : ${BUILD_FOLDER}"
rm -Rf ${BUILD_FOLDER}
mkdir -p ${BUILD_FOLDER}

pushd ${BUILD_FOLDER}

echo "## Compile the debug program for test purpose"
INCLUDES="-I${ROOT_FOLDER}/c_sources/include -I${ROOT_FOLDER}/libraries/ed247 -I${ROOT_FOLDER}/libraries/ed247/sources -I${SOURCEFOLDER} -I${ROOT_FOLDER}/../ED247_LIBRARY/_install/inc -I${ROOT_FOLDER}/../libxml2/include"
INTERFACE_SOURCES="${ROOT_FOLDER}/c_sources/src/ed247_cmd_xml.c ${ROOT_FOLDER}/c_sources/src/ed247_interface.c ${ROOT_FOLDER}/c_sources/src/tools.c"
LIBRARIES="-L${ROOT_FOLDER}/../libxml2/lib -lxml2 ${MINGW_FOLDER}/../x86_64-w64-mingw32/lib/libws2_32.a -L${ROOT_FOLDER}/../ED247_LIBRARY/_install/lib -led247"
SFUN_SOURCES="${ROOT_FOLDER}/libraries/ed247/ed247_sfun.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_configure.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_send.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_receive.c"
TEST_SOURCES="${SOURCEFOLDER}/main.c ${SOURCEFOLDER}/sfcn_stubs.c"

${CC} -o ${EXE} ${CC_OPTS} ${INCLUDES} ${SFUN_SOURCES} ${INTERFACE_SOURCES} ${TEST_SOURCES} ${LIBRARIES}
status=$?

if [ $status -eq 0 ]; then

	echo ""
	echo "## Prepare configuration files"
	echo ""

	unzip -o "${TESTFILEFOLDER}/${ARCHIVENAME}.zip"

	echo ""
	echo "## Execute program"
	echo ""

	./${EXE} "${BUILD_FOLDER}/${CONFIGURATIONFILE}.xml" "${BUILD_FOLDER}/stubbed.log"

	echo ""
	echo "## Quit and reset"
	echo ""
	popd

fi