#!/bin/bash

echo ""
echo " >> EtherCAT Master Auto Make "
echo ""
echo " >>>> Deleting Last Library Compilation "
echo ""
cd ../lib/ 
make clean
echo ""
echo " >>>> Deleting Last Source Compilation "
echo ""
cd ../src/ 
make clean
echo ""
echo " >>>> System Initialization "
echo ""
echo " >>>>>> Change Initial Config File Authority "
chmod 755 1_sys_env_info.sh
chmod 755 2_set_time.sh
chmod 755 3_load_modules.sh
echo ""
echo " >>>>>> System Initialization "
./1_sys_env_info.sh 
./2_set_time.sh 
./3_load_modules.sh 
echo ""
echo " >>>> Starting Library Compilation "
echo ""
cd ../lib/ 
make
echo ""
echo " >>>> Starting Source Compilation "
echo ""
cd ../src/	
make
echo ""

