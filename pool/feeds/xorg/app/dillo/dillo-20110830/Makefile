#
# Copyright (C) 2006-2010 OpenWrt.org
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dillo
PKG_REV:=1e01f4b09a63
PKG_VERSION:=20110830
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=hg
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://hg.dillo.org/dillo
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/dillo
  SECTION:=xorg-app
  CATEGORY:=Xorg
  SUBMENU:=app
  TITLE:=Dillo3
  URL:=http://www.dillo.org/
  DEPENDS:=+libXft +libstdcpp +libpthread +zlib +fltk +libjpeg +libpng $(ICONV_DEPENDS)
endef

define Package/dillo/description
  Dillo is a web browser project completely written in C.
  Dillo aims to be a multi-platform browser alternative that
  is small, stable, developer-friendly, usable, fast, and
  extensible.

  Note: This is the fltk1.3 based dillo.
endef

EXTRA_CPPFLAGS+=-I$(ICONV_PREFIX)/include
EXTRA_LDFLAGS+=-L$(ICONV_PREFIX)/lib -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

TARGET_LDFLAGS +=-ljpeg -lpng -liconv

FLTK_CONFIG := $(STAGING_DIR)/host/bin/fltk-config

CONFIGURE_ARGS += --with-fltk-config=$(FLTK_CONFIG)

define Build/Configure
	(cd $(PKG_BUILD_DIR); ./autogen.sh );
	$(call Build/Configure/Default)
endef

define Build/Install
	DESTDIR="$(PKG_INSTALL_DIR)" $(MAKE) -C $(PKG_BUILD_DIR) install
endef

define Package/dillo/install
	$(INSTALL_DIR) $(1)/etc/dillo $(1)/usr/share/applications $(1)/usr/share/icons
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/dillo/* $(1)/etc/dillo
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/dillo $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/dpid $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/dpidc $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/dillo $(1)/usr/lib/
	$(CP) ./files/dillo.png $(1)/usr/share/icons/
	$(INSTALL_DATA) ./files/dillo.desktop $(1)/usr/share/applications/
endef

$(eval $(call BuildPackage,dillo))
