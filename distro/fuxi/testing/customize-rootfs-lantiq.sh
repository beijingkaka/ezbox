#!/bin/bash
ROOTFS_DIR=$1
ROOTFS_BAK_DIR=$2

usage() {
  echo "usage: ./customize-rootfs-lantiq.sh <rootfs directory> <rootfs backup directory>"
}

dbg() {
  echo $1
}

if [ "x$ROOTFS_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$ROOTFS_BAK_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "$ROOTFS_DIR" = "$ROOTFS_BAK_DIR" ] ; then
  echo "rootfs directory must not be same with rootfs backup directory!"
  exit -1
fi

if [ ! -d $ROOTFS_DIR ] ; then
  echo "$ROOTFS_DIR does not exist!"
  exit -1
fi

#copy /etc/dbus-1 to /usr/share/config/dbus-1
rm -rf $ROOTFS_BAK_DIR
mv -f $ROOTFS_DIR $ROOTFS_BAK_DIR
mkdir -p $ROOTFS_DIR
cp -af $ROOTFS_BAK_DIR/bin $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/boot $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/init $ROOTFS_DIR/
mkdir -p $ROOTFS_DIR/lib
cp -af $ROOTFS_BAK_DIR/lib/ld*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/lib*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/rcso $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/modules $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/sbin $ROOTFS_DIR/
rm -f $ROOTFS_DIR/sbin/lvm
mkdir -p $ROOTFS_DIR/usr
cp -af $ROOTFS_BAK_DIR/usr/lib $ROOTFS_DIR/usr/
rm -f $ROOTFS_DIR/usr/lib/libchilli.so*
rm -f $ROOTFS_DIR/usr/lib/libext2fs.so*
rm -f $ROOTFS_DIR/usr/lib/libgmp.so*
mkdir -p $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/cryptsetup $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/share $ROOTFS_DIR/usr/
rm -f $ROOTFS_DIR/usr/share/opkg
