#!/bin/sh
#
export local_name=`hostname`
#
case "$local_name" in
  'goblin.internal.grad.kiev.ua')
        echo 'Starting script:'
        gmake clean
        rm   config.cache
        echo '*****  configure *****' >> night.log
        configure 2>&1 | cat >> night.log
        echo ""
        echo '*****  Make *****' >> night.log
        gmake 2>&1 | cat >> night.log
        echo
        ;;
  *)
        echo "Not such machine"
        exit 1
esac
