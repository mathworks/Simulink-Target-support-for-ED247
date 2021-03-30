#!/bin/bash

ecicpath=../../_files/ecic/ecic_func_exchange_a429_mc_tester_simple.xml
dumpfile=./dump.log
timeout=100

export ED247_LOG_LEVEL=4
# export ED247_LOG_FILEPATH=./dumper.log
export PATH=../../../../ED247_LIBRARY/_install/lib/:$PATH

./dumper.exe $ecicpath $dumpfile $timeout