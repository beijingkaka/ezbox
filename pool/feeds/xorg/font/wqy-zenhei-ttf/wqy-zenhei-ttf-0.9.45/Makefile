#
# Copyright (C) 2008-2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=wqy-zenhei
PKG_VERSION:=0.9.45
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/${PKG_NAME}/
PKG_SOURCE_URL:=@SF/wqy
PKG_MD5SUM:=4c6c3f4e902dd5ee0a121e8c41d040bd

include $(INCLUDE_DIR)/package.mk

define Package/wqy-zenhei-ttf
  SECTION:=xorg-font
  CATEGORY:=Xorg
  SUBMENU:=font
  DEPENDS:= @DISPLAY_SUPPORT
  TITLE:=wqy-zenhei-ttf
  URL:=http://wenq.org/
endef

define Build/Compile
	true
endef

define Build/Configure
	true
endef

define Package/wqy-zenhei-ttf/install
	$(INSTALL_DIR) $(1)/usr/share/fonts/ttf-wqy-zenhei
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/*.ttc $(1)/usr/share/fonts/ttf-wqy-zenhei
	$(INSTALL_DIR) $(1)/etc/fonts/conf.avail
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/44-wqy-zenhei.conf $(1)/etc/fonts/conf.avail
	$(INSTALL_DIR) $(1)/etc/fonts/conf.d
	ln -sf ../conf.avail/44-wqy-zenhei.conf $(1)/etc/fonts/conf.d/44-wqy-zenhei.conf
endef

$(eval $(call BuildPackage,wqy-zenhei-ttf))

