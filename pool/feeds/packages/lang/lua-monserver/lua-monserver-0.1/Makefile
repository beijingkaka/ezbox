#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-monserver
PKG_REV:=bff602eb763c205b87bca897988f75eb1c7db2b6
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=monserver-lua-$(PKG_VERSION)
PKG_SOURCE:=monserver-lua-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/daogangtang/monserver-lua.git
PKG_BUILD_DIR:=$(BUILD_DIR)/monserver-lua-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/lua-monserver
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-monserver
  URL:=http://github.com/daogangtang/monserver-lua
  DEPENDS:=+lua +lua-zmq +luajson +lua-tnetstrings
endef

define Package/lua-monserver/description
  The lua driver for monserver.
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

define Package/lua-monserver/install
	$(INSTALL_DIR) $(1)/usr/share/lua/monserver
	$(CP) $(PKG_BUILD_DIR)/src/*.lua $(1)/usr/share/lua/monserver/
endef

$(eval $(call BuildPackage,lua-monserver))
