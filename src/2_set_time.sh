#!/bin/bash

echo ""
echo " ===================== System Time Synchronization ==================== "
echo ""
echo "    >> NOTICES : This process requires an active Internet connection "
echo "                 and system administration authority "
echo ""
echo "  1. Set the systme clock Synchronization. "
echo ""
echo "    >> Time server clock from time.bora.net... "
echo ""
echo -n "       "
rdate -p time.bora.net
echo ""
echo "    >> Set OS & CMOS time to server time... "
echo ""
echo -n "       "
rdate -s time.bora.net && hwclock
sleep 0.5s
