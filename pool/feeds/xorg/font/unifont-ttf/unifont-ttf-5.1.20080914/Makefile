#
# Copyright (C) 2008-2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=unifont
PKG_VERSION:=5.1.20080914
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/$(PKG_NAME)-${PKG_VERSION}/
PKG_SOURCE_URL:=http://ezbox-pool.googlecode.com/svn/trunk
PKG_MD5SUM:=ae99db5125df712fe16246bab36fd95b

include $(INCLUDE_DIR)/package.mk

define Package/unifont-ttf
  SECTION:=xorg-font
  CATEGORY:=Xorg
  SUBMENU:=font
  DEPENDS:= @DISPLAY_SUPPORT
  TITLE:=unifont-ttf
  URL:=http://czyborra.com/unifont/
endef

define Build/Compile
	true
endef

define Build/Configure
	true
endef

define Package/unifont-ttf/install
	$(INSTALL_DIR) $(1)/usr/share/fonts/ttf-unifont
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/font/precompiled/*.ttf $(1)/usr/share/fonts/ttf-unifont
endef

$(eval $(call BuildPackage,unifont-ttf))

