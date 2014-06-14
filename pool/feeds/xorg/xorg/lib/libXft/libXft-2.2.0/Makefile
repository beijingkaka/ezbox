#
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# blogic@openwrt.org

include $(TOPDIR)/rules.mk

PKG_NAME:=libXft
PKG_RELEASE:=1
PKG_VERSION:=2.2.0

PKG_SOURCE_URL:=http://xorg.freedesktop.org/releases/individual/lib/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/Xorg/$(PKG_NAME)-$(PKG_VERSION)/
PKG_MD5SUM:=cce3c327258116493b753f157e0360c7

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libXft
  SECTION:=xorg-libraries
  CATEGORY:=Xorg
  SUBMENU:=libraries
  DEPENDS:=+fontconfig +libfreetype \
  +FEATURE_drawing-backend_nxlib:nxlib \
  +FEATURE_drawing-backend_libX11:libXrender
  TITLE:=libXft
  URL:=http://xorg.freedesktop.org/
endef

TARGET_LDFLAGS += \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), -lnano-X, )

define Build/Configure
	(cd $(PKG_BUILD_DIR); autoconf );
	$(call Build/Configure/Default, --with-x --x-libraries=$(STAGING_DIR)/usr/lib --x-includes=$(STAGING_DIR)/usr/include --with-freetype-config=$(STAGING_DIR)/host/bin/freetype-config)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib/pkgconfig,include/X11/Xft}
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.{so*,a,la} \
		$(1)/usr/lib/
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/* \
		$(1)/usr/lib/pkgconfig/
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/include/X11/Xft/* \
		$(1)/usr/include/X11/Xft/
endef

define Package/libXft/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.so* \
		$(1)/usr/lib/
endef

$(eval $(call BuildPackage,libXft))
