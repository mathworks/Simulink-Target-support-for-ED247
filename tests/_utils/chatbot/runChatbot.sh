#!/bin/bash

ecicpath=../../_files/ecic/ecic_func_exchange_nad_mc_main.xml
timestep=1000
loopcount=10

export ED247_LOG_LEVEL=4
# export ED247_LOG_FILEPATH=./chatbot.log
export PATH=../../../../ED247_LIBRARY/_install/lib/:$PATH

./chatbot.exe $ecicpath $timestep $loopcount