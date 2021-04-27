#!/bin/bash

if [[ -z "${LIBPATH}" ]]; then
    libpath=/c/Files/SANDBOX/git/ED247/ED247_LIBRARY/_install/lib:/c/Files/SANDBOX/git/ED247/libxml2/bin:/c/Files/SANDBOX/git/ED247/libxml2/lib
else
    libpath=${LIBPATH}
fi

if [[ -z "${EXENAME}" ]]; then
    exename=send_a429_r2019b
else
    exename=${EXENAME}
fi

if [[ -z "${LOGFILE}" ]]; then
    logfile=$exename.log
else
    logfile=${LOGFILE}
fi

export PATH=$libpath:$PATH

./$exename > $logfile