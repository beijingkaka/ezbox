#
# Copyright (C) 2010 ezbox-project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mongoose
PKG_REV:=8f84a486ef
PKG_VERSION:=20101103
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=hg
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://mongoose.googlecode.com/hg/

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/mongoose/Default
  SUBMENU:=web server
  TITLE:=Mongoose web server
  URL:=http://code.google.com/p/mongoose/
endef

define Package/mongoose/Default/description
  Mongoose is an easy to use web server.
  It can be embedded into existing application to provide a web interface to it.
endef

define Package/libmongoose
  $(call Package/mongoose/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libpthread +libopenssl +libluasp-mongoose
  TITLE+= (library)
endef

define Package/libmongoose/description
$(call Package/mongoose/Default/description)
  This package contains the mongoose web server shared library, used by other
  programs.
endef

define Package/mongoose-webserver
  $(call Package/mongoose/Default)
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libmongoose
  TITLE+= (binary)
endef

define Package/mongoose-webserver/description
$(call Package/mongoose/Default/description)
  This package contains the default web server implementation.
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LIB="libmongoose.so" LIB_PATH="$(STAGING_DIR)/usr/lib" linux
endef

define Build/Install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmongoose.so* $(PKG_INSTALL_DIR)/usr/lib
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mongoose $(PKG_INSTALL_DIR)/usr/bin
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/mongoose.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libmongoose.so* $(1)/usr/lib/
endef

define Package/libmongoose/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libmongoose.so* $(1)/usr/lib/
endef

define Package/mongoose-webserver/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/mongoose $(1)/usr/bin
endef

$(eval $(call BuildPackage,libmongoose))
$(eval $(call BuildPackage,mongoose-webserver))
