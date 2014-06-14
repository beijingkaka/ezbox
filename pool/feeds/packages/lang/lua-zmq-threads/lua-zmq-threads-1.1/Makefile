#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lua-zmq-threads
PKG_REV:=ce2d1c7c5a6f85140a1f4909676906eb5021d82e
PKG_VERSION:=1.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/Neopallium/lua-zmq.git

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/lua-zmq-threads
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=lua-zmq-threads
  URL:=http://github.com/Neopallium/lua-zmq
  DEPENDS:=+lua +zeromq +lua-zmq +lua-llthreads
endef

define Package/lua-zmq-threads/description
 Lua bindings to zeromq2, with LuaJIT2 FFI support.
endef

#define Build/Configure
#endef

#TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS) -std=c99
TARGET_CFLAGS += $(FPIC) $(TARGET_CPPFLAGS)

TARGET_LDFLAGS += -lm -ldl -llua -lzmq

ifdef CONFIG_USE_UCLIBC
TARGET_LDFLAGS += -lcrypt
endif

MAKE_FLAGS += \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	INCS="$(TARGET_CPPFLAGS)" \
	LIBS="$(TARGET_LDFLAGS)" \
	SHFLAGS="-shared" \
	UNAME="Linux"

define Package/lua-zmq-threads/install
	$(INSTALL_DIR) $(1)/usr/share/lua/zmq
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/lua/5.1/zmq/threads.lua $(1)/usr/share/lua/zmq/
endef

$(eval $(call BuildPackage,lua-zmq-threads))
