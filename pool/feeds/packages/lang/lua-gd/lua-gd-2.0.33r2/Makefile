#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-gd
PKG_VERSION:=2.0.33r2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=@SF/lua-gd
PKG_SOURCE_URL:=http://files.luaforge.net/releases/lua-gd/lua-gd/lua-gd-2.0.33r2forLua5.1/

include $(INCLUDE_DIR)/package.mk

define Package/lua-gd
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-gd
  URL:=http://ittner.github.com/lua-gd/
  DEPENDS:=+lua +libgd
endef

define Package/lua-gd/description
  Lua-GD is an image manipulation library for the Lua Programming Language based on Thomas Boutell's GD library.
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
	LUAPKG="lua" \
	UNAME="Linux"

#define Build/Compile
#endef

define Package/lua-gd/install
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gd.so \
		$(1)/usr/lib/lua/
endef

$(eval $(call BuildPackage,lua-gd))
