#!/bin/bash
#
# gyaio-tn_bounce is used to restart a node that is acting badly or is down.
# usage: gyaio-tn_bounce.sh [arglist]
# arglist will be passed to the node's command line. First with no modifiers
# then with --replay and then a third time with --resync
#
# the data directory and log file are set by this script. Do not pass them on
# the command line.
#
# in most cases, simply running ./gyaio-tn_bounce.sh is sufficient.
#

pushd $GYAIO_HOME

if [ ! -f programs/nodgya/nodgya ]; then
    echo unable to locate binary for nodgya
    exit 1
fi

config_base=etc/gyaio/node_
if [ -z "$GYAIO_NODE" ]; then
    DD=`ls -d ${config_base}[012]?`
    ddcount=`echo $DD | wc -w`
    if [ $ddcount -ne 1 ]; then
        echo $HOSTNAME has $ddcount config directories, bounce not possible. Set environment variable
        echo GYAIO_NODE to the 2-digit node id number to specify which node to bounce. For example:
        echo GYAIO_NODE=06 $0 \<options\>
        cd -
        exit 1
    fi
    OFS=$((${#DD}-2))
    export GYAIO_NODE=${DD:$OFS}
else
    DD=${config_base}$GYAIO_NODE
    if [ ! \( -d $DD \) ]; then
        echo no directory named $PWD/$DD
        cd -
        exit 1
    fi
fi

bash $GYAIO_HOME/scripts/gyaio-tn_down.sh
bash $GYAIO_HOME/scripts/gyaio-tn_up.sh $*
