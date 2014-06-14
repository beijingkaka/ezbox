# 
# Copyright (C) 2008-2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=rtai
# PKG_VERSION:=3.8.1
PKG_VERSION:=20110603
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=cvs
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
# PKG_SOURCE_URL:=http://www.rtai.org/RTAI/
PKG_SOURCE_URL:=:pserver:anonymous@cvs.gna.org:/cvs/rtai
PKG_SOURCE_SUBDIR:=magma
PKG_SOURCE_VERSION:=-D$(PKG_VERSION)

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)/magma

include $(INCLUDE_DIR)/package.mk

define Package/rtai/Default
  SUBMENU:=RTAI
  SECTION:=utils
  CATEGORY:=Real-Time
  TITLE:=Real-Time Application Interface
  URL:=http://www.rtai.org/
endef

define Package/rtai/Default/description
 RTAI - the RealTime Application Interface for Linux -
 which lets you write applications with strict timing constraints for your
 favourite operating system.
endef

define Package/rtai
$(call Package/rtai/Default)
  DEPENDS:=
  TITLE+= (interpreter)
endef

define Package/rtai/description
$(call Package/rtai/Default/description)
 This package contains the RTAI kernel modules.
endef

#TARGET_CFLAGS += -I$(LINUX_DIR)/arch/$(LINUX_KARCH)/include

#CONFIGURE_ARGS += --with-linux-dir=$(LINUX_DIR)
#CONFIGURE_ARGS += --disable-leds
#CONFIGURE_ARGS += --enable-math=m
#CONFIGURE_ARGS += --enable-math-c99=y
#CONFIGURE_ARGS += --verbose

AUTOCONF_DIR:=$(PKG_BUILD_DIR)/base/config/autoconf
KCONFIG_DIR:=$(PKG_BUILD_DIR)/base/config/kconfig
KCONFIG_FILE:=$(PKG_BUILD_DIR)/.rtai_config

CONFIGURE_PREFIX:=/usr/realtime

define Build/Configure
	$(CP) configs/defconfig-$(ARCH) $(KCONFIG_FILE)
	(cd $(PKG_BUILD_DIR); autoreconf --include=$(AUTOCONF_DIR) --install --symlink );
	(cd $(PKG_BUILD_DIR); \
		$(MAKE) -C $(KCONFIG_DIR) \
			-f $(KCONFIG_DIR)/Makefile.kconfig oldconfig \
			srctree=$(PKG_BUILD_DIR) ARCH=$(ARCH));
	$(call Build/Configure/Default, \
		--prefix=$(CONFIGURE_PREFIX) \
		--exec-prefix=$(CONFIGURE_PREFIX) \
		--bindir=$(CONFIGURE_PREFIX)/bin \
		--sbindir=$(CONFIGURE_PREFIX)/sbin \
		--libexecdir=$(CONFIGURE_PREFIX)/lib \
		--datadir=$(CONFIGURE_PREFIX)/share \
		--mandir=$(CONFIGURE_PREFIX)/man \
		--infodir=$(CONFIGURE_PREFIX)/info \
		--with-kconfig-file=$(KCONFIG_FILE) \
		--with-linux-dir="$(LINUX_DIR)" \
		--with-lxrt-inline=extern \
		--enable-shared=yes \
		--enable-static=no \
		--disable-leds \
		--enable-shm=y \
		--enable-trace=n \
		--enable-fpu=y \
		--enable-math=m \
		--enable-math-c99=n \
		--enable-netrpc=n \
		--enable-rtdm=y \
		--enable-doc=n \
		--enable-module-debug=y \
		--enable-user-debug=y \
		--enable-sim=n \
		--with-x=no \
		--verbose \
		, ARCH="$(ARCH)" \
		  CROSS_COMPILE="$(TARGET_CROSS)" \
		,)
endef

define Build/Compile
	$(SED) 's,-I/usr,-I$(TOOLCHAIN_DIR),g' \
		$(PKG_BUILD_DIR)/base/math/Makefile.kbuild
	$(SED) 's,-I/usr,-I$(STAGING_DIR)/usr,g' \
		$(PKG_BUILD_DIR)/rtai-lab/scilab5/devices/machine.h
	$(SED) 's,-I/usr,-I$(STAGING_DIR)/usr,g' \
		$(PKG_BUILD_DIR)/rtai-lab/matrixx/Makefile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		ARCH="$(ARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		all
endef

define Build/Install
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		ARCH="$(ARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/include
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/lib
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/bin
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/modules
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/include/* $(1)$(CONFIGURE_PREFIX)/include/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/lib/* $(1)$(CONFIGURE_PREFIX)/lib/
	$(SED) 's,$(STAGING_DIR),,g' $(1)$(CONFIGURE_PREFIX)/lib/liblxrt.la
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/modules/* $(1)$(CONFIGURE_PREFIX)/modules/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/bin/rtai-config $(1)$(CONFIGURE_PREFIX)/bin/
	$(SED) \
		's,DESTDIR,$(STAGING_DIR),g' \
		$(1)$(CONFIGURE_PREFIX)/bin/rtai-config
	$(INSTALL_DIR) $(2)/bin
	for f in rtai-config; do \
		$(LN) ../..$(CONFIGURE_PREFIX)/bin/$$$$f $(2)/bin/ ; \
	done
endef

define Package/rtai/install
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/bin
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/calibration
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/lib
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/modules
	$(INSTALL_DIR) $(1)$(CONFIGURE_PREFIX)/share/rtai
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/bin/* $(1)$(CONFIGURE_PREFIX)/bin/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/calibration/* $(1)$(CONFIGURE_PREFIX)/calibration/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/lib/*.so* $(1)$(CONFIGURE_PREFIX)/lib/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/modules/*.ko $(1)$(CONFIGURE_PREFIX)/modules/
	$(CP) $(PKG_INSTALL_DIR)$(CONFIGURE_PREFIX)/share/rtai/* $(1)$(CONFIGURE_PREFIX)/share/rtai/
	$(SED) 's,DESTDIR,,g' $(1)$(CONFIGURE_PREFIX)/bin/rtai-config
	$(SED) 's,$(KERNEL_BUILD_DIR),/usr/src,g' $(1)$(CONFIGURE_PREFIX)/bin/rtai-config
endef

$(eval $(call BuildPackage,rtai))

