#
# Copyright (C) 2010 Zeta Labs
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=microwin
PKG_VERSION:=20110220
PKG_REV:=ed27fcaca82916d204c831715bac392630a5daf2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=git://microwindows.org/microwin
PKG_SOURCE_PROTO:=git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
#PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk

NANOX_CONFIG=generic
ifeq ($(CONFIG_NANOX_MACHINE_VIRTUALBOX),y)
  NANOX_CONFIG=virtualbox
endif
ifeq ($(CONFIG_NANOX_MACHINE_COLINUX),y)
  NANOX_CONFIG=colinux
endif

define Package/microwin
  SECTION:=xorg-server
  CATEGORY:=Xorg
  SUBMENU:=server
  TITLE:=Nano-X Window System
  DEPENDS:=+libpthread +libpng +libfreetype +libjpeg @DISPLAY_SUPPORT
  URL:=http://www.microwindows.org/
endef

define Package/microwin/description
  Nano-X is an Open Source project aimed at bringing
  the features of modern graphical windowing environments
  to smaller devices.
endef

define Package/microwin/config
	source "$(SOURCE)/Config.in"
endef

define Build/Configure
	$(CP) ./files/configs/defconfig-$(NANOX_CONFIG) $(PKG_BUILD_DIR)/src/config
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		ARCH=LINUX-NATIVE \
		NATIVETOOLSPREFIX=$(TARGET_CROSS) \
		CC=$(TARGET_CC) \
		INCJPEG=$(STAGING_DIR)/usr/include \
		INCPNG=$(STAGING_DIR)/usr/include \
		INCFT2LIB=$(STAGING_DIR)/usr/include/freetype2 \
		LDFLAGS="-L$(PKG_BUILD_DIR)/src/lib -L$(STAGING_DIR)/usr/lib -lz -ljpeg -lpng -lfreetype"
endef

define Build/Install
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		INSTALL_PREFIX=$(PKG_INSTALL_DIR)/usr \
		INSTALL_OWNER1= \
		INSTALL_OWNER2= \
		CC=$(TARGET_CC) \
		INCJPEG=$(STAGING_DIR)/usr/include \
		INCPNG=$(STAGING_DIR)/usr/include \
		INCTIFF=$(STAGING_DIR)/usr/include \
		INCFT2LIB=$(STAGING_DIR)/usr/include/freetype2 \
		LDFLAGS="-L$(PKG_BUILD_DIR)/src/lib -L$(STAGING_DIR)/usr/lib -lz -ljpeg -lpng -lfreetype" \
		install
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/bin/nanowm $(PKG_INSTALL_DIR)/usr/bin/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/microwin/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/usr/lib
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/nano-X \
		$(1)/usr/bin/
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/lib/libnano-X.so \
		$(1)/usr/lib/
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/nanowm \
		$(1)/usr/bin/
endef

$(eval $(call BuildPackage,microwin))
