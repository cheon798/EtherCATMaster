#!/bin/bash 

echo ""
echo " =================== Unload Module and Check Slaves  ================== "
echo ""
echo "    >> NOTICES : Unload the IgH EtherCAT master module from the kernel  "
echo ""
echo "  1. Unload the IgH EtherCAT Master Module "
echo ""
echo -n "    >> "
/opt/etherlab/etc/init.d/ethercat stop
echo ""
echo " ====================================================================== "
echo ""
