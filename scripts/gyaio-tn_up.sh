#!/bin/bash
#
# gyaio-tn_up is a helper script used to start a node that was previously stopped.
# It is not intended to be run stand-alone; it is a companion to the
# gyaio-tn_bounce.sh and gyaio-tn_roll.sh scripts.

connected="0"

rundir=programs/nodgya
prog=nodgya


if [ "$PWD" != "$GYAIO_HOME" ]; then
    echo $0 must only be run from $GYAIO_HOME
    exit -1
fi

if [ ! -e $rundir/$prog ]; then
    echo unable to locate binary for nodgya
    exit -1
fi

if [ -z "$GYAIO_NODE" ]; then
    echo data directory not set
    exit -1
fi

datadir=var/lib/node_$GYAIO_NODE
now=`date +'%Y_%m_%d_%H_%M_%S'`
log=stderr.$now.txt
touch $datadir/$log
rm $datadir/stderr.txt
ln -s $log $datadir/stderr.txt

relaunch() {
    echo "$rundir/$prog $* --data-dir $datadir --config-dir etc/gyaio/node_$GYAIO_NODE > $datadir/stdout.txt  2>> $datadir/$log "
    nohup $rundir/$prog $* --data-dir $datadir --config-dir etc/gyaio/node_$GYAIO_NODE > $datadir/stdout.txt  2>> $datadir/$log &
    pid=$!
    echo pid = $pid
    echo $pid > $datadir/$prog.pid

    for (( a = 10; $a; a = $(($a - 1)) )); do
        echo checking viability pass $((11 - $a))
        sleep 2
        running=$(pgrep $prog | grep -c $pid)
        echo running = $running
        if [ -z "$running" ]; then
            break;
        fi
        connected=`grep -c "net_plugin.cpp:.*connection" $datadir/$log`
        if [ "$connected" -ne 0 ]; then
            break;
        fi
    done
}

if [ -z "$GYAIO_LEVEL" ]; then
    echo starting with no modifiers
    relaunch $*
    if [ "$connected" -eq 0 ]; then
        GYAIO_LEVEL=replay
    else
        exit 0
    fi
fi

if [ "$GYAIO_LEVEL" == replay ]; then
    echo starting with replay
    relaunch $* --replay
    if [  "$connected" -eq 0 ]; then
        GYAIO_LEVEL=resync
    else
        exit 0
    fi
fi
if [ "$GYAIO_LEVEL" == resync ]; then
    echo starting wih resync
    relaunch $* --resync
fi
