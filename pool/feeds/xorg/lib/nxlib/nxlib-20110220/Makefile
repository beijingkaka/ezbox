#
# Copyright (C) 2010 Zeta Labs
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=nxlib
PKG_VERSION:=20110220
PKG_REV:=b42634d57c15e372ddb0afbddbc876d1981b7b07
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=git://microwindows.org/nxlib
PKG_SOURCE_PROTO:=git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/

PKG_INSTALL:=1
#PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/nxlib
  SECTION:=xorg-libraries
  CATEGORY:=Xorg
  SUBMENU:=libraries
  TITLE:=NXLIB X11 Emulation Library for Nano-X.
  DEPENDS:=+microwin @FEATURE_drawing-backend_nxlib @DISPLAY_SUPPORT
  URL:=http://www.microwindows.org/
endef

define Feature/nxlib
  TARGET_NAME:=drawing-backend
  TARGET_TITLE:=Drawing Backend
  NAME:=nxlib
endef

define Package/nxlib/description
  NXLIB build an X11 -> Nano-X conversion library,
  allowing binaries compiled for X11 to run unmodified on a
  Microwindows based system, using the Nano-X API.
endef

define Build/Configure
	touch $(PKG_BUILD_DIR)/Makefile
	mv $(PKG_BUILD_DIR)/Makefile $(PKG_BUILD_DIR)/Makefile.orig
	$(CP) ./files/nxlib-Makefile $(PKG_BUILD_DIR)/Makefile
	mkdir -p $(PKG_BUILD_DIR)/X11/extensions
	mv $(PKG_BUILD_DIR)/X11/shape.h $(PKG_BUILD_DIR)/X11/shape.h.orig
	$(CP) ./files/X11/*.h $(PKG_BUILD_DIR)/X11/
	$(CP) ./files/X11/extensions/*.h $(PKG_BUILD_DIR)/X11/extensions/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC=$(TARGET_CC) \
		AR=$(TARGET_CROSS)ar \
		MWIN_INCLUDE=$(STAGING_DIR)/usr/include/microwin \
		MWIN_LIB=$(STAGING_DIR)/usr/lib
endef

define Build/Install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/include/X11 $(PKG_INSTALL_DIR)/usr/lib
	$(MAKE) -C $(PKG_BUILD_DIR) \
		INSTALL_DIR=$(PKG_INSTALL_DIR)/usr/lib \
		CC=$(TARGET_CC) \
		AR=$(TARGET_CROSS)ar \
		MWIN_INCLUDE=$(STAGING_DIR)/usr/include/microwin \
		MWIN_LIB=$(STAGING_DIR)/usr/lib \
		install
	$(CP) $(PKG_BUILD_DIR)/nxlib.h $(PKG_INSTALL_DIR)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/X11/* $(PKG_INSTALL_DIR)/usr/include/X11/
	$(LN) libNX11.so $(PKG_INSTALL_DIR)/usr/lib/libX11.so
	$(LN) libNX11.so $(PKG_INSTALL_DIR)/usr/lib/libXext.so
	$(LN) libNX11.so $(PKG_INSTALL_DIR)/usr/lib/libXi.so
	$(LN) libNX11.so $(PKG_INSTALL_DIR)/usr/lib/libXrender.so
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/nxlib/install
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/bin $(1)/etc
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
endef

$(eval $(call Feature,nxlib))
$(eval $(call BuildPackage,nxlib))
