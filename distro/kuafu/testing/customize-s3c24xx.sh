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
  [ ! -d $POOL_DIR/customize/target/linux/s3c24xx ] || cp -af $POOL_DIR/customize/target/linux/s3c24xx/* $WORK_DIR/target/linux/s3c24xx/
}

do_customize

