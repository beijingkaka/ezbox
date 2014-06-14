#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=monserver
PKG_REV:=9095825e237f478d6111a72749fa9791c691fa9b
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/daogangtang/monserver.git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/monserver
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Web Servers/Proxies
  TITLE:=monserver web server
  URL:=http://github.com/daogangtang/monserver
  DEPENDS:=+zeromq +lua
  MENU:=1
endef

define Package/monserver/description
  A web server based on mongrel2.
endef

TARGET_CFLAGS += -Isrc -pthread -rdynamic -DNDEBUG -D_FILE_OFFSET_BITS=64
TARGET_LDFLAGS += -lzmq -ldl -llua
TARGET_LIBS += -lzmq -ldl -llua

ifdef CONFIG_USE_UCLIBC
TARGET_CFLAGS += -D__UCLIBC__=1
TARGET_LDFLAGS += -lcrypt
TARGET_LIBS += -lcrypt
endif

MAKE_FLAGS += PREFIX="/usr"
MAKE_FLAGS += LIBS="$(TARGET_LIBS)"

define Package/monserver/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/monserver $(1)/usr/bin/
endef

$(eval $(call BuildPackage,monserver))
