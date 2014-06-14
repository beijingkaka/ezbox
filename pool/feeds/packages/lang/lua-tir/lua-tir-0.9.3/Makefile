#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-tir
PKG_VERSION:=0.9.3
PKG_RELEASE:=1

PKG_SOURCE:=tir-$(PKG_VERSION)-3.tar.gz
PKG_SOURCE_URL:=http://tir.mongrel2.org/downloads/
PKG_BUILD_DIR:=$(BUILD_DIR)/tir-$(PKG_VERSION)-3

include $(INCLUDE_DIR)/package.mk

define Package/lua-tir
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Tir
  URL:=http://tir.mongrel2.org/
  DEPENDS:=+lua +luaposix +lua-zmq +lua-md5 +luajson +lsqlite3
endef

define Package/lua-tir/description
  Tir is a framework for doing Web applications in Lua with the Mongrel2 web server.
endef

define Build/Configure
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

define Package/lua-tir/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/tir $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/share/lua/tir
	$(CP) $(PKG_BUILD_DIR)/tir/* $(1)/usr/share/lua/tir/
endef

$(eval $(call BuildPackage,lua-tir))
