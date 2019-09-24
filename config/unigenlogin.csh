#!/bin/csh

if ( ! $?UNIGEN )	setenv UNIGEN ${PWD}

if ( !($?LD_LIBRARY_PATH) ) setenv LD_LIBRARY_PATH

setenv PATH ${UNIGEN}/bin':'${PATH}
setenv LD_LIBRARY_PATH ${UNIGEN}/lib':'${LD_LIBRARY_PATH}

