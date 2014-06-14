#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-tnetstrings
PKG_REV:=22ac06ca7688dfab36841e470480750a41e3aa5b
PKG_VERSION:=1.1.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=tnetstrings.lua-$(PKG_VERSION)
PKG_SOURCE:=tnetstrings.lua-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/jsimmons/tnetstrings.lua.git
PKG_BUILD_DIR:=$(BUILD_DIR)/tnetstrings.lua-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-tnetstrings
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-tnetstrings
  URL:=https://github.com/jsimmons/tnetstrings.lua/
  DEPENDS:=+lua
endef

define Package/lua-tnetstrings/description
  A tnetstrings implementation in Lua.
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

define Package/lua-tnetstrings/install
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/tnetstrings.lua \
		$(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,lua-tnetstrings))
