#!/bin/bash

ROOTFS_DIR=$1

usage() {
  echo "usage: ./customize-rootfs.sh <rootfs directory>"
}

dbg() {
  echo $1
}

if [ "x$ROOTFS_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ ! -d $ROOTFS_DIR ] ; then
  echo "$ROOTFS_DIR does not exist!"
  exit -1
fi

# libintl symbol link
rm -rf $ROOTFS_DIR/usr/lib/libintl.so.8
ln -sf libintl.so.8.1.1 $ROOTFS_DIR/usr/lib/libintl.so.8
rm -rf $ROOTFS_DIR/lib/libintl.so.8
ln -sf ../usr/lib/libintl.so.8 $ROOTFS_DIR/lib/libintl.so.8

# clean /etc
rm -rf $ROOTFS_DIR/etc
mkdir -p $ROOTFS_DIR/etc

# remove unnecessary kernel modules
mv -f $ROOTFS_DIR/lib/modules $ROOTFS_DIR/usr/lib/
mkdir -p $ROOTFS_DIR/lib/modules/2.6.24
# hi35xx drivers
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/hi35xx_*.ko $ROOTFS_DIR/lib/modules/2.6.24/
# ethernet driver depends
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/libphy.ko $ROOTFS_DIR/lib/modules/2.6.24/
# SATA driver
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/ahci.ko $ROOTFS_DIR/lib/modules/2.6.24/
# EXT4 filesystem drivers
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/ext4dev.ko $ROOTFS_DIR/lib/modules/2.6.24/
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/jbd2.ko $ROOTFS_DIR/lib/modules/2.6.24/
mv -f $ROOTFS_DIR/usr/lib/modules/2.6.24/mbcache.ko $ROOTFS_DIR/lib/modules/2.6.24/
# remove the usr/lib/modules directory
rm -rf $ROOTFS_DIR/usr/lib/modules

# clean MPP samples
rm -rf $ROOTFS_DIR/usr/share/mpp/sample/hifb
rm -rf $ROOTFS_DIR/usr/share/mpp/sample/vdec
rm -rf $ROOTFS_DIR/usr/share/mpp/sample/mixer
rm -rf $ROOTFS_DIR/usr/share/mpp/sample/tde
