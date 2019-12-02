#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /afs/cern.ch/user/s/skkwan/Public/triggerDevel/hls_taus/solution1/.autopilot/db/a.g.bc ${1+"$@"}
