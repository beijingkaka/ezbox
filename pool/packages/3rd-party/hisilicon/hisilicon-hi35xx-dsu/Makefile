#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hisilicon-hi35xx-dsu
#PKG_VERSION:=0.1
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(INCLUDE_DIR)/package.mk

define Package/hisilicon-hi35xx-dsu/Default
  SECTION:=kernel
  CATEGORY:=Kernel modules
  DEPENDS:=@PACKAGE_kmod-hi35xx-dsu
  SUBMENU:=Proprietary HI35xx DSU driver
  SUBMENUDEP:=@TARGET_hi35xx
endef

define KernelPackage/hi35xx-dsu/Default
  $(call Package/hisilicon-hi35xx-dsu/Default)
  SECTION:=kernel
  DEPENDS:=@TARGET_hi35xx
  TITLE:=Kernel driver for HI35xx chipsets
  FILES:=$(PKG_BUILD_DIR)/driver$(1)/hi35xx_dsu.ko
  AUTOLOAD:=$(call AutoLoad,30,hi35xx_dsu)
endef

define KernelPackage/hi35xx-dsu/Default/description
 This package contdsuns the proprietary DSU driver for the Hisilicon
 HI35xx chipset.
endef

define KernelPackage/hi3515-dsu
$(call KernelPackage/hi35xx-dsu/Default,-hi3515)
  TITLE+= (hi3515)
endef

define KernelPackage/hi3515-dsu/description
$(call KernelPackage/hi35xx-dsu/Default/description)
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
	$(CP) files/hi3515_dsu.ko $(PKG_BUILD_DIR)/driver-hi3515/hi35xx_dsu.ko
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
endef

$(eval $(call KernelPackage,hi3515-dsu))
