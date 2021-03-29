#!/bin/bash
#
# Syntax
#	./runTests.sh
#
# Copyright 2020 The MathWorks, Inc.
#

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
CMAKE_FOLDER=/c/Files/PROGRAMS/ThirdParty/cmake

# Application
ROOT_FOLDER=$(cd `dirname $0` && pwd)
BUILD_FOLDER=${ROOT_FOLDER}/build
TESTFILEFOLDER=${ROOT_FOLDER}/tests/_files

export CC=${MINGW_FOLDER}/gcc.exe
export CXX=${MINGW_FOLDER}/g++.exe
export AR=${MINGW_FOLDER}/ar.exe

export PATH=${ROOT_FOLDER}/../ed247/windows/mingw4.9.2/x64/lib:${PATH}

pushd ${BUILD_FOLDER}

# Remove executable and cache if CLEAN environment variable is set
if [ ! -z "$CLEAN" ]; then
	rm -f ed247SimulinkAdapter.exe CMakeCache.txt
fi

# Compilation
${CMAKE_FOLDER}/bin/cmake -G "MSYS Makefiles" -D CMAKE_C_COMPILER=${CC} -D CMAKE_CXX_COMPILER=${CXX} -D CMAKE_AR=${AR} ${CC_OPTS} ..
make

#Copy library to lib/ folder
#make install

if [ "$?" -ne "0" ]; then
	echo "Compilation FAILED"
	exit $?
fi

if [ -z "$TEST_FILTER" ]; then
	OPTS=""
else
	OPTS="--gtest_filter=*${TEST_FILTER}*"
fi

# Execute tests
FILEFOLDER=${TESTFILEFOLDER} ${BUILD_FOLDER}/ed247SimulinkAdapter.exe ${OPTS}
TEST_STATUS=$?

popd

echo "Test status = ${TEST_STATUS}"

exit ${TEST_STATUS}