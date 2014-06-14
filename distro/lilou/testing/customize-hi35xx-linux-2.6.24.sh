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
  # customize kernel build
  [ ! -f $POOL_DIR/customize/include/customize-rootfs-kernel-defaults.mk ] || cp -af $POOL_DIR/customize/include/customize-rootfs-kernel-defaults.mk $WORK_DIR/include/kernel-defaults.mk
  # mesa math patches
  ln -sf $POOL_DIR/customize/packages/Xorg/lib/mesa/mesa-7.9/patches/901-fix-math.patch $POOL_DIR/packages/Xorg/lib/mesa/mesa-7.9/patches/901-fix-math.patch
  ln -sf $POOL_DIR/customize/packages/Xorg/lib/mesa/mesa-7.9/patches/902-fix-pipe-atomic.patch $POOL_DIR/packages/Xorg/lib/mesa/mesa-7.9/patches/902-fix-pipe-atomic.patch
  # kdrive math patches
  ln -sf $POOL_DIR/customize/packages/Xorg/xorg/xserver/xorg-kdrive/xorg-kdrive-1.10.2/Makefile-hi35xx $POOL_DIR/packages/Xorg/xorg/xserver/xorg-kdrive/xorg-kdrive-1.10.2/Makefile
  # kernel ata-ahci driver
  cp -af $POOL_DIR/customize/package/kernel/modules/block-hi35xx-linux-2.6.24.mk $WORK_DIR/package/kernel/modules/block.mk
  # kernel ext4dev fs
  cp -af $POOL_DIR/customize/package/kernel/modules/fs-hi35xx-linux-2.6.24.mk $WORK_DIR/package/kernel/modules/fs.mk
  # kernel crypto API
  cp -af $POOL_DIR/customize/package/kernel/modules/crypto-hi35xx-linux-2.6.24.mk $WORK_DIR/package/kernel/modules/crypto.mk
  # kernel IPSec support
  cp -af $POOL_DIR/customize/package/kernel/modules/netsupport-hi35xx-linux-2.6.24.mk $WORK_DIR/package/kernel/modules/netsupport.mk
}

do_customize
