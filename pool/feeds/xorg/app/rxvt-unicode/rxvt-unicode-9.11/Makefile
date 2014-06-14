#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rxvt-unicode
PKG_RELEASE:=1
PKG_VERSION:=9.11

PKG_SOURCE_URL:=http://dist.schmorp.de/rxvt-unicode/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/
PKG_MD5SUM:=1bed5bfeed026e0bfafa0e9e4f62aa37

PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk

PKG_INSTALL:=1

CONFIGURE_ARGS+= \
	--x-includes=$(STAGING_DIR)/usr/include \
	--x-libraries=$(STAGING_DIR)/usr/lib \
	--disable-perl \
	--disable-utmp \
	--disable-wtmp \
	--disable-lastlog \
	--with-x \

#TARGET_LDFLAGS+=-Wl,-rpath-link=$(STAGING_DIR)/usr/lib

TARGET_LDFLAGS +=-lz -lexpat -lXau -lXdmcp -lxcb

TARGET_LDFLAGS += \
	$(if $(CONFIG_PACKAGE_libXft), -lfreetype -lfontconfig, )

define Package/rxvt-unicode
  SECTION:=xorg-app
  CATEGORY:=Xorg
  SUBMENU:=app
  DEPENDS:=+libX11 +libXext +libXau
  TITLE:=rxvt-unicode
  URL:=http://software.schmorp.de/pkg/rxvt-unicode.html
endef

define Package/rxvt-unicode/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/usr/share/applications $(1)/usr/share/icons
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/* \
		$(1)/usr/bin/
	$(INSTALL_DATA) \
		./files/rxvt.desktop \
		$(1)/usr/share/applications/rxvt.desktop
	$(INSTALL_DATA) \
		./files/icon.png \
		$(1)/usr/share/icons/rxvt.png
endef

$(eval $(call BuildPackage,rxvt-unicode))
