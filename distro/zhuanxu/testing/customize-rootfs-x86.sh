#!/bin/bash
ROOTFS_DIR=$1
ROOTFS_BAK_DIR=$2
ROOTFS_TARGET_DIR=$3

LINUX_VER=3.8.12

usage() {
  echo "usage: ./customize-rootfs-x86.sh <rootfs dir> <rootfs backup dir> <target rootfs dir>"
}

dbg() {
  echo "$*"
}

if [ "x$ROOTFS_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$ROOTFS_BAK_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$ROOTFS_TARGET_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "$ROOTFS_BAK_DIR" = "$ROOTFS_DIR" ] ; then
  echo "rootfs directory must not be same with rootfs backup directory!"
  exit -1
fi

if [ "$ROOTFS_TARGET_DIR" = "$ROOTFS_DIR" ] ; then
  echo "rootfs directory must not be same with target rootfs directory!"
  exit -1
fi

if [ ! -d $ROOTFS_DIR ] ; then
  echo "$ROOTFS_DIR does not exist!"
  exit -1
fi

#copy /etc/dbus-1 to /usr/share/config/dbus-1
rm -rf $ROOTFS_BAK_DIR
mv -f $ROOTFS_DIR $ROOTFS_BAK_DIR

# overwrite by predefined target rootfs stuff
cp -af $ROOTFS_TARGET_DIR/boot/ezbox_boot.cfg.dft $ROOTFS_BAK_DIR/boot/
cp -af $ROOTFS_TARGET_DIR/boot/ezbox_rootfs.cfg $ROOTFS_BAK_DIR/boot/
cp -af $ROOTFS_TARGET_DIR/lib/rcso/agents/env/action/boot $ROOTFS_BAK_DIR/lib/rcso/agents/env/action/
cp -af $ROOTFS_TARGET_DIR/lib/rcso/agents/env/action/start $ROOTFS_BAK_DIR/lib/rcso/agents/env/action/
cp -af $ROOTFS_TARGET_DIR/lib/rcso/agents/env/action/stop $ROOTFS_BAK_DIR/lib/rcso/agents/env/action/
cp -af $ROOTFS_TARGET_DIR/lib/rcso/agents/env/action/sys_stop $ROOTFS_BAK_DIR/lib/rcso/agents/env/action/
cp -af $ROOTFS_TARGET_DIR/lib/rcso/agents/env/action/sys_restart $ROOTFS_BAK_DIR/lib/rcso/agents/env/action/

mkdir -p $ROOTFS_DIR
if [ -e $ROOTFS_TARGET_DIR/boot/vmlinuz ]; then
cp -af $ROOTFS_BAK_DIR/* $ROOTFS_DIR/
cp -af $ROOTFS_TARGET_DIR/boot/vmlinuz* $ROOTFS_DIR/boot/
cp -af $ROOTFS_TARGET_DIR/boot/rootfs* $ROOTFS_DIR/boot/
else
cp -af $ROOTFS_BAK_DIR/bin $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/boot $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/init $ROOTFS_DIR/
# copy needed stuff
mkdir -p $ROOTFS_DIR/lib
cp -af $ROOTFS_BAK_DIR/lib/ld*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/lib*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/rcso $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/modules $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/sbin $ROOTFS_DIR/
rm -f $ROOTFS_DIR/sbin/lvm
mkdir -p $ROOTFS_DIR/usr
mkdir -p $ROOTFS_DIR/usr/lib
cp -af $ROOTFS_BAK_DIR/usr/lib/libcom_err.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libe2p.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libext2fs.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libintl.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libssl.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libcrypto.so* $ROOTFS_DIR/usr/lib/
mkdir -p $ROOTFS_DIR/usr/sbin
cp -af $ROOTFS_BAK_DIR/usr/sbin/cryptsetup $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/e2fsck $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/mke2fs $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/mkfs.ext* $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/tune2fs $ROOTFS_DIR/usr/sbin/
mkdir -p $ROOTFS_DIR/usr/share
cp -af $ROOTFS_BAK_DIR/usr/share/ezcfg $ROOTFS_DIR/usr/share/
cp -af $ROOTFS_BAK_DIR/usr/share/terminfo $ROOTFS_DIR/usr/share/
cp -af $ROOTFS_BAK_DIR/usr/share/alsa $ROOTFS_DIR/usr/share/
cp -af $ROOTFS_BAK_DIR/usr/share/pulseaudio $ROOTFS_DIR/usr/share/
fi

