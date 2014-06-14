#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=luajson
PKG_VERSION:=1.2.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://files.luaforge.net/releases/luajson/luajson/1.2.2/

include $(INCLUDE_DIR)/package.mk

define Package/luajson
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=luajson
  URL:=http://github.com/harningt/luajson
  DEPENDS:=+lua +lua-lpeg
endef

define Package/luajson/description
  LuaJSON is a customizable JSON decoder/encoder using LPEG for parsing. 
endef

define Build/Configure
endef

TARGET_CFLAGS += -DLUA_USE_LINUX $(FPIC) -std=gnu99

ifneq ($(CONFIG_USE_EGLIBC),)   
  ifeq ($(CONFIG_EGLIBC_OPTION_EGLIBC_UTMP),)
    TARGET_CFLAGS += -DNO_GETLOGIN
  endif
endif


define Package/luajson/install
	$(INSTALL_DIR) $(1)/usr/share/lua
	$(CP) $(PKG_BUILD_DIR)/lua/* $(1)/usr/share/lua/
endef

$(eval $(call BuildPackage,luajson))
