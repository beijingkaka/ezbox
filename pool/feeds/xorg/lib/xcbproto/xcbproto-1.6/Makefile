#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xcb-proto
PKG_VERSION:=1.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://xcb.freedesktop.org/dist/
PKG_MD5SUM:=04313e1d914b44d0e457f6c494fc178b
PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=python

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

STAMP_CONFIGURED:=$(STAMP_CONFIGURED)_$(call confvar,PACKAGE_python PACKAGE_python-mini)

define Package/xcbproto
  SECTION:=xorg-libraries
  CATEGORY:=Xorg
  SUBMENU:=libraries
  BUILDONLY:=1
  TITLE:=xcbproto
  URL:=http://xcb.freedesktop.org/
endef

define InstallPy
	$(INSTALL_DIR) $(1)/host/lib/$(PYTHON)/dist-packages/xcbgen
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/$(PYTHON)/site-packages/xcbgen/*.py \
		$(1)/host/lib/$(PYTHON)/dist-packages/xcbgen/
	$(INSTALL_DIR) $(1)/usr/lib/$(PYTHON)/dist-packages/xcbgen
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/$(PYTHON)/site-packages/xcbgen/*.py \
		$(1)/usr/lib/$(PYTHON)/dist-packages/xcbgen/
endef

define Build/InstallDev
	$(call InstallPy,$(1),$(2))
	$(INSTALL_DIR) \
		$(1)/usr/lib/pkgconfig \
		$(1)/host/share/xcb

	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/share/xcb/* \
		$(1)/host/share/xcb/

	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/* \
		$(1)/usr/lib/pkgconfig/

	$(SED) 's,datadir=/usr,datadir=$(STAGING_DIR)/host,g' $(1)/usr/lib/pkgconfig/xcb-proto.pc
endef

$(eval $(call BuildPackage,xcbproto))
