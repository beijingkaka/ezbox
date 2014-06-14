#
# Copyright (C) 2008-2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=freefont-ttf
PKG_VERSION:=20100919
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/freefont-${PKG_VERSION}/
PKG_SOURCE_URL:=http://ftp.gnu.org/gnu/freefont
PKG_MD5SUM:=33d96c755d0a6f585bc917f08cbddf66

include $(INCLUDE_DIR)/package.mk

define Package/freefont-ttf
  SECTION:=xorg-font
  CATEGORY:=Xorg
  SUBMENU:=font
  DEPENDS:= @DISPLAY_SUPPORT
  TITLE:=freefont-ttf
  URL:=http://www.gnu.org/software/freefont/
endef

define Build/Compile
	true
endef

define Build/Configure
	true
endef

define Package/freefont-ttf/install
	$(INSTALL_DIR) $(1)/usr/share/fonts/ttf-freefont
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/*.ttf $(1)/usr/share/fonts/ttf-freefont
endef

$(eval $(call BuildPackage,freefont-ttf))

