#!/bin/bash

g++ -o chatbot src/chatbot_main.cpp ../../shared/sync_entity/sync_entity.cpp /c/Files/PROGRAMS/ThirdParty/MinGW64/4.9.2-airbus/x86_64-w64-mingw32/lib/libws2_32.a -I../../_install/inc -I../../shared/sync_entity -std=c++11 -L../../_install/lib -led247

