#!/usr/bin/ksh
EMS_VOBS="/vobs/EMS_OTS"
FRAMEWORK_ROOT="/vobs/EMS_OTS/Framework"
PATH=.:/usr/dt/bin:/usr/sbin:/usr/ccs/bin:/usr/local/bin:/usr/local/gnu/bin:/bin:/bin/X11:$PATH
LD_LIBRARY_PATH=/usr/lib:/lib:.:$FRAMEWORK_ROOT/libs:/home/objstadm/ospace2.2.2/ccs-2.2.2/lib
export LD_LIBRARY_PATH
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vobs/cots/TAO/ACE_wrappers/lib:/home/objstadm/occi/lib:/home/objstadm/curl/lib:/home/objstadm/liburadius:/usr/lib/lwp:/vobs/SNMP/libdes:$PATH
export LD_LIBRARY_PATH
export PATH
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/vobs/SNMP/snmp++/lib
export LD_LIBRARY_PATH

