#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-redis
PKG_VERSION:=2.0.3
PKG_RELEASE:=1

PKG_SOURCE:=redis-lua-$(PKG_VERSION)-0.tar.gz
#PKG_SOURCE_URL:=git://github.com/nrk/redis-lua.git
PKG_SOURCE_URL:=http://cloud.github.com/downloads/nrk/redis-lua/
PKG_BUILD_DIR:=$(BUILD_DIR)/redis-lua-$(PKG_VERSION)-0

include $(INCLUDE_DIR)/package.mk

define Package/lua-redis
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=redis-lua
  URL:=https://github.com/nrk/redis-lua/
  DEPENDS:=+lua +luasocket
endef

define Package/lua-redis/description
  A Lua client library for the redis key value storage system.
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

define Package/lua-redis/install
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/redis.lua \
		$(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,lua-redis))
