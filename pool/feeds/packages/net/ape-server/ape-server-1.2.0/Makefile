#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ape-server
PKG_REV:=19495fc33e88c3df6e77544646eb4d7b4bde3508
PKG_VERSION:=1.2.0
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/APE-Project/APE_Server.git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ape-server
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Web Servers/Proxies
  TITLE:=APE Comet server
  URL:=http://github.com/APE-Project/APE_Server
  DEPENDS:=+udns-libs +js $(CXX_DEPENDS)
  MENU:=1
endef

define Package/ape-server/description
  APE is a full-featured OpenSource solution designed for Ajax Push.
  It includes a comet server and a Javascript Framework.
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/udns -I$(STAGING_DIR)/usr/include/js
TARGET_LDFLAGS += -ldl -lm -ludns -lmozjs185

MAKE_FLAGS += CFLAGS="$(TARGET_CFLAGS)"
MAKE_FLAGS += LFLAGS="$(TARGET_LDFLAGS)"
MAKE_FLAGS += prefix="/usr"

define Build/Configure
	echo "#define USE_EPOLL_HANDLER" > $(PKG_BUILD_DIR)/src/configure.h
	echo "LINUX_BUILD = 1" > $(PKG_BUILD_DIR)/modules/plateform.mk
	# check mysql
	echo "HAS_MYSQL = 0" > $(PKG_BUILD_DIR)/modules/mysql.mk
	echo "#undef _USE_MYSQL" >> $(PKG_BUILD_DIR)/src/configure.h
endef

define Build/Compile
	$(call Build/Compile/Default,)
	$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH)/modules \
		$(MAKE_FLAGS)
endef

define Package/ape-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/aped $(1)/usr/bin/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/bin/ape.conf $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/ape-server/modules
	$(CP) $(PKG_BUILD_DIR)/modules/lib/*.so $(1)/usr/lib/ape-server/modules/
endef

$(eval $(call BuildPackage,ape-server))
