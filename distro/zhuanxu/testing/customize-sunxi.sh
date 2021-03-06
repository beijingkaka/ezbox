#!/bin/bash

POOL_DIR=$1
WORK_DIR=$2

usage() {
  echo "usage: ./customize.sh <pool directory> <work directory>"
}

dbg() {
  echo $1
}

if [ "x$POOL_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$WORK_DIR" = "x" ] ; then
  usage
  exit -1
fi

do_customize() {
#  [ ! -d $POOL_DIR/customize/target/linux/generic ] || cp -af $POOL_DIR/customize/target/linux/generic/* $WORK_DIR/target/linux/generic/
#  [ ! -d $POOL_DIR/customize/target/linux/sunxi ] || cp -af $POOL_DIR/customize/target/linux/sunxi/* $WORK_DIR/target/linux/sunxi/
  if [ -d $WORK_DIR/target/linux/generic/base-files ] ; then \
    rm -rf $WORK_DIR/target/linux/generic/base-files/init ; \
    ln -sf sbin/ezcd $WORK_DIR/target/linux/generic/base-files/init ; \
  fi
  echo "do_customize OK!"
}

do_customize

