#!/bin/bash 

echo ""
echo " =================== System Environment Information =================== "
echo ""
echo -n "  1. HOST NANE        : "
echo $HOSTNAME
echo ""
echo -n "  2. CURRENT USER     : "
echo $USER
echo ""
echo -n "  3. OS NAME          : "
uname -s -r
echo -n "                        "
uname -v -m
echo ""
NAME="EtherCAT Master Platform"
echo "  4. PROJECT NAME     : $NAME"
echo ""
echo -n "  5. CURRENT POSITION : "
echo $PWD
echo ""
echo "  6. PLATFORM SOURCE TREE "
echo ""
cd ../
tree -a ${PWD}
