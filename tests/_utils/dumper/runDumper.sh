#!/bin/bash

if [[ -z "${ECICPATH}" ]]; then
    ecicpath=../../_files/ecic/ecic_func_exchange_a429_uc_tester_simple.xml
else
    ecicpath=${ECICPATH}
fi
echo "ECIC path = $ecicpath"

if [[ -z "${DUMPFILE}" ]]; then
    dumpfile=./dump.log
else
    dumpfile=${DUMPFILE}
fi
echo "Dump file = $dumpfile"

if [[ -z "${TIMEOUT}" ]]; then
    timeout=1000
else
    timeout=${TIMEOUT}
fi
echo "Timeout = $timeout"

if [[ -z "${ED247_LIB_PATH}" ]]; then
    ed247libpath=../../../../ED247_LIBRARY/_install/lib/
else
    ed247libpath=${ED247_LIB_PATH}
fi

export ED247_LOG_LEVEL=4
export PATH=$ed247libpath:$PATH

./dumper.exe $ecicpath $dumpfile $timeout