#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-lglib
PKG_REV:=c4d50208ac71be07942bed659cf3da579b58d5e3
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=lglib-$(PKG_VERSION)
PKG_SOURCE:=lglib-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/daogangtang/lglib.git
PKG_BUILD_DIR:=$(BUILD_DIR)/lglib-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-lglib
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-lglib
  URL:=http://github.com/daogangtang/lglib
  DEPENDS:=+lua +lua-lgstring
endef

define Package/lua-lglib/description
  The basic extended lua libraries for legerobot's all products.
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

define Package/lua-lglib/install
	$(INSTALL_DIR) $(1)/usr/share/lua/lglib
	$(CP) $(PKG_BUILD_DIR)/src/*.lua $(1)/usr/share/lua/lglib/
endef

$(eval $(call BuildPackage,lua-lglib))
