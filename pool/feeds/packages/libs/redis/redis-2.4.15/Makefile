#
# Copyright (C) ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=redis
PKG_VERSION:=2.4.15
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://redis.googlecode.com/files/

include $(INCLUDE_DIR)/package.mk

define Package/redis-cli
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libjemalloc
  TITLE:=Command Line Interface (CLI) to Redis databases
  URL:=http://redis.io/
  SUBMENU:=database
endef

define Package/redis-cli/description
Command Line Interface (CLI) to Redis databases.
endef

define Package/redis-server
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libjemalloc
  TITLE:=Redis database server
  URL:=http://redis.io/
  SUBMENU:=database
endef

define Package/redis-server/description
  Redis is a key-value database in a similar vein to memcache but the dataset is non-volatile.
  Redis additionally provides native support for atomically manipulating and
querying data structures such as lists and sets.
  The dataset is stored entirely in memory and periodically flushed to disk. 
endef

define Build/Configure
endef

ifdef CONFIG_USE_UCLIBC
TARGET_CFLAGS += -D__UCLIBC__
endif

TARGET_CFLAGS += $(FPIC)

# for libjemalloc
JEMALLOC_CFLAGS=$(CONFIGURE_ARGS)
JEMALLOC_CFLAGS+=\
	--enable-autogen \
	ac_cv_path_AR=$(TARGET_CROSS)ar \
	ac_cv_path_LD=$(TARGET_CROSS)ld \
	je_cv_static_page_shift=12

MAKE_FLAGS+=JEMALLOC_CFLAGS='$(JEMALLOC_CFLAGS)'
MAKE_FLAGS+=PREFIX=/usr

MAKE_PATH:=src

# because ARCH means something else in the project Makefile
#ARCH:=

define Build/Install
	$(call Build/Install/Default, install)
endef

define Package/redis-cli/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/redis-cli $(1)/usr/bin/
endef

define Package/redis-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/redis-server $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/redis-benchmark $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/redis-check-aof $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/redis-check-dump $(1)/usr/bin/
	$(INSTALL_DATA) ./files/etc/redis.conf $(1)/etc/redis.conf
endef

define Package/redis-server/conffiles
/etc/redis.conf
endef

$(eval $(call BuildPackage,redis-cli))
$(eval $(call BuildPackage,redis-server))
