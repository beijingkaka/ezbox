#!/bin/bash
ROOTFS_DIR=$1
ROOTFS_BAK_DIR=$2
ROOTFS_TARGET_DIR=$3

LINUX_VER=3.3.8

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
mkdir -p $ROOTFS_DIR
cp -af $ROOTFS_BAK_DIR/bin $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/boot $ROOTFS_DIR/
cp -af $ROOTFS_BAK_DIR/init $ROOTFS_DIR/
mkdir -p $ROOTFS_DIR/lib
cp -af $ROOTFS_BAK_DIR/lib/ld*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/lib*.so* $ROOTFS_DIR/lib/
cp -af $ROOTFS_BAK_DIR/lib/rcso $ROOTFS_DIR/lib/

mkdir -p $ROOTFS_DIR/lib/modules/$LINUX_VER
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/3c59x.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/8139cp.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/8139too.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/8390.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/aead.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/aes_generic.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/aes-i586.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ahci.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/aoe.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/arc4.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/b44.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/cbc.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/chainiv.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/crc-ccitt.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/crypto_algapi.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/crypto_blkcipher.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/crypto_hash.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/cryptomgr.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/crypto_wq.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/dm-crypt.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/dm-log.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/dm-mirror.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/dm-mod.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/dm-region-hash.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/e1000e.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/e1000.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/e100.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ecb.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ehci-hcd.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/eseqiv.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ip_queue.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/iptable_filter.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/iptable_mangle.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/iptable_nat.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/iptable_raw.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ip_tables.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_ah.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_ECN.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_LOG.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_MASQUERADE.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_NETMAP.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_REDIRECT.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_REJECT.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ipt_ULOG.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/krng.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/libahci.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/natsemi.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ne2k-pci.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_conntrack_broadcast.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_conntrack_ipv4.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_conntrack.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_conntrack_proto_gre.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_defrag_ipv4.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nf_nat.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/nfnetlink.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ohci-hcd.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/pcnet32.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/r8169.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/rng.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/sha1_generic.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/sis900.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/sit.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/sky2.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/ssb.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/tg3.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/tun.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/uhci-hcd.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/usb-common.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/usbcore.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/usb-storage.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/via-rhine.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/via-velocity.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/x_tables.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_CLASSIFY.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_comment.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_connbytes.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_connmark.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_conntrack.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/
cp -af $ROOTFS_BAK_DIR/lib/modules/$LINUX_VER/xt_CT.ko $ROOTFS_DIR/lib/modules/$LINUX_VER/

cp -af $ROOTFS_BAK_DIR/sbin $ROOTFS_DIR/
rm -f $ROOTFS_DIR/sbin/lvm
mkdir -p $ROOTFS_DIR/usr
mkdir -p $ROOTFS_DIR/usr/lib
cp -af $ROOTFS_BAK_DIR/usr/lib/libblkid.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libcharset.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libcrypto.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libcryptsetup.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libcurses.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libdevmapper.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libgcrypt.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libgpg-error.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libiconv.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libintl.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libip4tc.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libipq.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libiptc.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libncurses.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libpopt.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libreadline.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libssl.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libuuid.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libxtables.so* $ROOTFS_DIR/usr/lib/
cp -af $ROOTFS_BAK_DIR/usr/lib/libz.so* $ROOTFS_DIR/usr/lib/

mkdir -p $ROOTFS_DIR/usr/lib/iptables
cp -af $ROOTFS_BAK_DIR/usr/lib/iptables/libipt_*.so $ROOTFS_DIR/usr/lib/iptables/
cp -af $ROOTFS_BAK_DIR/usr/lib/iptables/libxt_*.so $ROOTFS_DIR/usr/lib/iptables/

mkdir -p $ROOTFS_DIR/usr/sbin
cp -af $ROOTFS_BAK_DIR/usr/sbin/cryptsetup $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/iptables $ROOTFS_DIR/usr/sbin/
cp -af $ROOTFS_BAK_DIR/usr/sbin/iptables-* $ROOTFS_DIR/usr/sbin/

mkdir -p $ROOTFS_DIR/usr/share
cp -af $ROOTFS_BAK_DIR/usr/share/ezcfg $ROOTFS_DIR/usr/share/
cp -af $ROOTFS_BAK_DIR/usr/share/terminfo $ROOTFS_DIR/usr/share/

# overwrite by predefined target rootfs stuff
cp -af $ROOTFS_TARGET_DIR/boot/ezbox_boot.cfg $ROOTFS_DIR/boot/
cp -af $ROOTFS_TARGET_DIR/boot/ezbox_rootfs.cfg $ROOTFS_DIR/boot/
