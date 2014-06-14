#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-md5
PKG_REV:=def7fa6343e359f2b3a2dbddf3a3463a6b9643d3
PKG_VERSION:=1.1.2
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/keplerproject/md5.git

include $(INCLUDE_DIR)/package.mk

define Package/lua-md5
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-md5
  URL:=http://www.keplerproject.org/md5/
  DEPENDS:=+lua
endef

define Package/lua-md5/description
  MD5 offers basic cryptographic facilities for Lua 5.1:
  a hash (digest) function, a pair crypt/decrypt based on MD5 and CFB,
  and a pair crypt/decrypt based on DES with 56-bit keys.
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

define Package/lua-md5/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/md5
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/*.so $(1)/usr/lib/lua/md5/
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/md5.lua $(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,lua-md5))
