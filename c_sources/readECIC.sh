#!/bin/bash
#
# Syntax
#	./debugDev.sh <ecic_file>
#
# Copyright 2020 The MathWorks, Inc.
#

ECIC_FILE=$1

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
BUILD_FOLDER=${ROOT_FOLDER}/work
TESTFILEFOLDER=${ROOT_FOLDER}/tests/_files

EXE_NAME=test.exe

export CC=${MINGW_FOLDER}/gcc.exe
export CXX=${MINGW_FOLDER}/g++.exe
export AR=${MINGW_FOLDER}/ar.exe

export PATH=${ROOT_FOLDER}/../../ED247_LIBRARY/_install/lib:${PATH}

pushd ${BUILD_FOLDER}

echo "## Delete previous executable (${PWD}/${EXE_NAME})"
rm -f ${EXE_NAME}

echo "## Compile the debug program for test purpose"
${CC} -o test.exe -I${ROOT_FOLDER}/include -I${ROOT_FOLDER}/../../ED247_LIBRARY/_install/inc -I${ROOT_FOLDER}/../../libxml2/include -L${ROOT_FOLDER}/../../ED247_LIBRARY/_install/lib ${ROOT_FOLDER}/src/ed247_cmd_xml.cpp ${ROOT_FOLDER}/src/ed247_interface.cpp ${ROOT_FOLDER}/src/tools.cpp ${BUILD_FOLDER}/debug.cpp -L${ROOT_FOLDER}/../../libxml2/lib -lxml2 ${MINGW_FOLDER}/../x86_64-w64-mingw32/lib/libws2_32.a -led247

# echo "## Copy configuration files"
# cp ${TESTFILEFOLDER}/ELACe2C_CMD.xml ${BUILD_FOLDER}/ELACe2C_CMD2.xml
# cp ${TESTFILEFOLDER}/ELACe2C_ECIC.xml ${BUILD_FOLDER}/ELACe2C_ECIC.xml

echo ""
echo "## Execute program"
echo ""
./${EXE_NAME} "${ECIC_FILE}"

echo ""
echo "## Quit and reset"
echo ""
popd

