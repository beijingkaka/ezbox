#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=cx9461x
BOARDNAME:=Conexant CX9461X
FEATURES:=jffs2
CFLAGS:=-O2 -pipe -march=armv5te -mtune=arm1026ej-s -fno-caller-saves
SUBTARGETS:=generic

LINUX_VERSION:=2.6.32.32

define Target/Description
	CX9461x arm
endef

include $(INCLUDE_DIR)/target.mk

$(eval $(call BuildTarget))
