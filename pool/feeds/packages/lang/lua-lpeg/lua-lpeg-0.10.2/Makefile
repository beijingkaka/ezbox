#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-lpeg
PKG_VERSION:=0.10.2
PKG_RELEASE:=1

PKG_SOURCE:=lpeg-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.inf.puc-rio.br/~roberto/lpeg/
PKG_BUILD_DIR:=$(BUILD_DIR)/lpeg-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-lpeg
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=LPeg
  URL:=http://www.inf.puc-rio.br/~roberto/lpeg/
  DEPENDS:=+lua
endef

define Package/lua-lpeg/description
  LPeg is a new pattern-matching library for Lua, based on Parsing Expression Grammars (PEGs).
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

define Package/lua-lpeg/install
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lpeg.so $(1)/usr/lib/lua/
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/re.lua $(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,lua-lpeg))
