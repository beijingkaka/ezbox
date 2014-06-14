#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mongrel2
PKG_VERSION:=1.7.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://mongrel2.org/static/downloads/
#PKG_MD5SUM:=53105bbe3ac9319db54d9eb46119695b

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/mongrel2
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Web Servers/Proxies
  TITLE:=Mongrel2 web server
  URL:=http://mongrel2.org/
  DEPENDS:=+libsqlite3 +zeromq $(CXX_DEPENDS)
  MENU:=1
endef

define Package/mongrel2/description
 Mongrel2 is a modern web server that is protocol and language agnostic.
endef

TARGET_CFLAGS += -Isrc -pthread -rdynamic -DNDEBUG -D_FILE_OFFSET_BITS=64
TARGET_LDFLAGS += -lzmq -ldl -lsqlite3

ifdef CONFIG_USE_UCLIBC
TARGET_CFLAGS += -D__UCLIBC__=1
endif

MAKE_FLAGS += PREFIX="/usr"
MAKE_FLAGS += LIBS="-lzmq -ldl -lsqlite3"

define Package/mongrel2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/mongrel2 $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/m2sh $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/mongrel2/config_modules
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/mongrel2/config_modules/*.so $(1)/usr/lib/mongrel2/config_modules/
	$(INSTALL_DIR) $(1)/usr/lib/mongrel2/filters
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/mongrel2/filters/*.so $(1)/usr/lib/mongrel2/filters/
endef

$(eval $(call BuildPackage,mongrel2))
