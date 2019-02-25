#!/bin/bash

echo ""
echo " ==================== Load Module and Check Slaves  =================== "
echo ""
echo "    >> NOTICES : For normal operating, check the connection between "
echo "                 the slaves and the master. "
echo ""
echo "  1. Load the IgH EtherCAT Master Module "
echo ""
echo -n "    >> "
/opt/etherlab/etc/init.d/ethercat start
sleep 4s
echo ""
echo "  2. Confirm the IgH EtherCAT Master Module loaded into the kernel "
echo ""
echo "    >> Print kernel message buffer for check EMM "
echo ""
dmesg -T | tail -23
echo ""
echo "  3. Check the connected slaves on the I/O bus-line "
echo ""
/opt/etherlab/bin/ethercat-tools slaves
echo ""
echo ""
echo "  4. Make the PDO header file and xml file "
echo ""
echo "    >> The PDO header file named 'pdo.h' is created "
/opt/etherlab/bin/ethercat-tools cstruct > pdo.h
echo ""
echo "    >> The xml file named 'pho.xml' is created "
/opt/etherlab/bin/ethercat-tools xml > pdo.xml
echo ""
echo " ====================================================================== "


