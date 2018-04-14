#!/bin/bash

# Order matters!

SOFTDIR=/home/evovch/soft

#export ERSYS=${SOFTDIR}/er
#export ERBUILD=${SOFTDIR}/er_build

#cd ${ERBUILD}/
#. ./config.sh
#cd -

cd ${SOFTDIR}/go4-5.3.0_fairsoftroot/
. ./go4login
cd -

cd ${SOFTDIR}/FairSoft_install/bin/
. ./thisroot.sh
cd -
