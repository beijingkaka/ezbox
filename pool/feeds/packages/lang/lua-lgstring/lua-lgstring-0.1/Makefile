#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-lgstring
PKG_REV:=15c6d760e17936691a777c75e7374d11a0b7a0a7
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=lgstring-$(PKG_VERSION)
PKG_SOURCE:=lgstring-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/daogangtang/lgstring.git
PKG_BUILD_DIR:=$(BUILD_DIR)/lgstring-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-lgstring
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-lgstring
  URL:=http://github.com/daogangtang/lgstring
  DEPENDS:=+lua
endef

define Package/lua-lgstring/description
  lglib's string part's C implementation.
endef

define Build/Configure
endef

#TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS) -std=c99
TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS)

TARGET_LDFLAGS += -llua

MAKE_FLAGS += \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	INCS="$(TARGET_CPPFLAGS)" \
	LIBS="$(TARGET_LDFLAGS)" \
	SHFLAGS="-shared" \
	UNAME="Linux"

#define Build/Compile
#endef

define Package/lua-lgstring/install
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lgstring.so $(1)/usr/lib/lua/
endef

$(eval $(call BuildPackage,lua-lgstring))
