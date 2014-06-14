#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-llthreads
PKG_REV:=fe6f9df7de8cbaa59fb8b4fed346321cc935d003
PKG_VERSION:=1.2
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/Neopallium/lua-llthreads.git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/lua-llthreads
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-llthreads
  URL:=https://github.com/jsimmons/llthreads-lua/
  DEPENDS:=+lua +libpthread
endef

define Package/lua-llthreads/description
  Low-Level threads(pthreads or WIN32 threads) for Lua.
endef

#define Build/Configure
#endef

#TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS) -std=c99
TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS)

TARGET_LDFLAGS += -llua -lm -ldl

ifdef CONFIG_USE_UCLIBC
TARGET_LDFLAGS += -lcrypt
endif

TARGET_LDFLAGS += -Wl,-rpath-link,$(STAGING_DIR)/usr/lib

MAKE_FLAGS += \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	INCS="$(TARGET_CPPFLAGS)" \
	LIBS="$(TARGET_LDFLAGS)" \
	SHFLAGS="-shared" \
	UNAME="Linux"

#define Build/Compile
#endef

define Package/lua-llthreads/install
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lua/5.1/llthreads.so $(1)/usr/lib/lua/
endef

$(eval $(call BuildPackage,lua-llthreads))
