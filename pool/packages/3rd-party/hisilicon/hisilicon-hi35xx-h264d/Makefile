#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hisilicon-hi35xx-h264d
#PKG_VERSION:=0.1
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(INCLUDE_DIR)/package.mk

define Package/hisilicon-hi35xx-h264d/Default
  SECTION:=kernel
  CATEGORY:=Kernel modules
  DEPENDS:=@PACKAGE_kmod-hi35xx-h264d
  SUBMENU:=Proprietary HI35xx H.264 Decode driver
  SUBMENUDEP:=@TARGET_hi35xx
endef

define KernelPackage/hi35xx-h264d/Default
  $(call Package/hisilicon-hi35xx-h264d/Default)
  SECTION:=kernel
  DEPENDS:=@TARGET_hi35xx
  TITLE:=Kernel driver for HI35xx chipsets
  FILES:=$(PKG_BUILD_DIR)/driver$(1)/hi35xx_h264d.ko
  AUTOLOAD:=$(call AutoLoad,30,hi35xx_h264d)
endef

define KernelPackage/hi35xx-h264d/Default/description
 This package conth264dns the proprietary H.264 Decode driver for the Hisilicon
 HI35xx chipset.
endef

define KernelPackage/hi3515-h264d
$(call KernelPackage/hi35xx-h264d/Default,-hi3515)
  TITLE+= (hi3515)
endef

define KernelPackage/hi3515-h264d/description
$(call KernelPackage/hi35xx-h264d/Default/description)
endef

MAKE_KMOD := $(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		PATH="$(TARGET_PATH)" \
		SUBDIRS="$(PKG_BUILD_DIR)/kmod" \

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/driver-hi3515
endef

define Build/Compile
	# Compile the kernel part
	# $(MAKE_KMOD) \
		SUBDIRS="$(PKG_BUILD_DIR)/driver" \
		MODFLAGS="-DMODULE -mlong-calls" \
		modules
	$(CP) files/hi3515_h264d.ko $(PKG_BUILD_DIR)/driver-hi3515/hi35xx_h264d.ko
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
endef

$(eval $(call KernelPackage,hi3515-h264d))
