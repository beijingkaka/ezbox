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
  [ ! -d $POOL_DIR/customize/target/linux/hi35xx ] || cp -af $POOL_DIR/customize/target/linux/hi35xx/* $WORK_DIR/target/linux/hi35xx/
  # mesa math patches
  ln -sf $POOL_DIR/customize/packages/Xorg/lib/mesa/mesa-7.9/patches/901-fix-math.patch $POOL_DIR/packages/Xorg/lib/mesa/mesa-7.9/patches/901-fix-math.patch
  ln -sf $POOL_DIR/customize/packages/Xorg/lib/mesa/mesa-7.9/patches/902-fix-pipe-atomic.patch $POOL_DIR/packages/Xorg/lib/mesa/mesa-7.9/patches/902-fix-pipe-atomic.patch
  # kdrive math patches
  ln -sf $POOL_DIR/customize/packages/Xorg/xorg/xserver/xorg-kdrive/xorg-kdrive-1.10.2/Makefile-hi35xx $POOL_DIR/packages/Xorg/xorg/xserver/xorg-kdrive/xorg-kdrive-1.10.2/Makefile
}

do_customize

