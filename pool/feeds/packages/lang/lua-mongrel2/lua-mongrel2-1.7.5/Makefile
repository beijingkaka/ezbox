#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-mongrel2
PKG_REV:=f6d2f50df67c69a56be50d23690530e19e9648db
PKG_VERSION:=1.7.5
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=mongrel2-lua-$(PKG_VERSION)
PKG_SOURCE:=mongrel2-lua-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/jsimmons/mongrel2-lua.git
PKG_BUILD_DIR:=$(BUILD_DIR)/mongrel2-lua-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-mongrel2
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-mongrel2
  URL:=http://github.com/jsimmons/mongrel2-lua
  DEPENDS:=+lua +lua-zmq +luajson +lua-tnetstrings +lsqlite3
endef

define Package/lua-mongrel2/description
  Lua api for the mongrel2 zeromq handler interface.
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

define Package/lua-mongrel2/install
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(CP) $(PKG_BUILD_DIR)/mongrel2 $(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,lua-mongrel2))
