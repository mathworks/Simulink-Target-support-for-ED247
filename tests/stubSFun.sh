#!/bin/bash
#
# Syntax
#	./stubSFun.sh
#
# Copyright 2020 The MathWorks, Inc.
#

EXE=stubbedSFun.exe
ARCHIVENAME=A429_simple01
CONFIGURATIONFILE=a429_01_ecic_in

# Inputs
TEST_FILTER=${1:-""}

# Environment variables
CC_OPTS=""
if [ ! -z "$DEBUG" ]; then
	CC_OPTS="${CC_OPTS} -DDEFINE_DEBUG=ON"
fi

# Tools
#MINGW_FOLDER=/c/Files/SANDBOX/git/P11222-AirbusOperationsSAS-ED247/ED247_LIBRARY/deps/new_dependencies/MINGW/mingw4.9.2/x64/bin
#CMAKE_FOLDER=/c/Files/SANDBOX/git/P11222-AirbusOperationsSAS-ED247/ED247_LIBRARY/deps/new_dependencies/CMAKE/mingw4.9.2/x64

MINGW_FOLDER=/c/Files/PROGRAMS/ThirdParty/MinGW64/4.9.2-airbus/bin
CMAKE_FOLDER=/c/Files/PROGRAMS/ThirdParty/cmake/3.20.2

# Application
ROOT_FOLDER=$(cd `dirname $0` && pwd)
ROOT_FOLDER=$(dirname $ROOT_FOLDER)
BUILD_FOLDER=${ROOT_FOLDER}/work
TESTFILEFOLDER=${ROOT_FOLDER}/tests/_files

export CC=${MINGW_FOLDER}/gcc.exe
export CXX=${MINGW_FOLDER}/g++.exe
export AR=${MINGW_FOLDER}/ar.exe

export PATH=${ROOT_FOLDER}/../ED247_LIBRARY/_install/lib:${PATH}

pushd ${BUILD_FOLDER}

echo "## Delete previous executable"
rm -f ${EXE}

echo "## Compile the debug program for test purpose"
gcc -o ${EXE} -DDEBUG -I${ROOT_FOLDER}/c_sources/include -I${ROOT_FOLDER}/libraries/ed247 -I${ROOT_FOLDER}/libraries/ed247/sources -I${ROOT_FOLDER}/work/stubbed_sfun -I${ROOT_FOLDER}/../ED247_LIBRARY/_install/inc -I${ROOT_FOLDER}/../libxml2/include -L${ROOT_FOLDER}/../ED247_LIBRARY/_install/lib ${ROOT_FOLDER}/libraries/ed247/ed247_sfun.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_configure.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_send.c ${ROOT_FOLDER}/libraries/ed247/sources/ed247_sfun_receive.c ${ROOT_FOLDER}/c_sources/src/ed247_cmd_xml.c ${ROOT_FOLDER}/c_sources/src/ed247_interface.c ${ROOT_FOLDER}/c_sources/src/tools.c ${ROOT_FOLDER}/work/stubbed_sfun/main.c -L${ROOT_FOLDER}/../libxml2/lib -lxml2 ${MINGW_FOLDER}/../x86_64-w64-mingw32/lib/libws2_32.a -led247

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

