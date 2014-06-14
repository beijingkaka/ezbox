#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-bamboo
PKG_REV:=e994ecce9065bf924c6c170b0509aabe26ad01fc
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=bamboo-$(PKG_VERSION)
PKG_SOURCE:=bamboo-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/daogangtang/bamboo.git
PKG_BUILD_DIR:=$(BUILD_DIR)/bamboo-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-bamboo
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Bamboo
  URL:=http://github.com/daogangtang/bamboo
  DEPENDS:=+lua +lua-monserver +lua-lglib
endef

define Package/lua-bamboo/description
  Bamboo is the web framework of Lua based on Mongrel2, ZeroMQ and NoSQL database.
endef

define Build/Configure
	mv -f $(PKG_BUILD_DIR)/src/bin $(PKG_BUILD_DIR)/
endef

#TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS) -std=c99
TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS)

TARGET_LDFLAGS += -llua

MAKE_FLAGS += \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	INCS="$(TARGET_CPPFLAGS)" \
	LIBS="$(TARGET_LDFLAGS)" \
	SHFLAGS="-shared" \
	UNAME="Linux"

define Build/Compile
endef

define Package/lua-bamboo/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/bamboo $(1)/usr/bin/bamboo
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/bamboo_handler $(1)/usr/bin/bamboo_handler
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/shell.lua $(1)/usr/bin/bamboo_shell.lua
	$(INSTALL_DIR) $(1)/usr/share/lua/bamboo
	$(CP) $(PKG_BUILD_DIR)/src/* $(1)/usr/share/lua/bamboo/
endef

$(eval $(call BuildPackage,lua-bamboo))
