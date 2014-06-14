#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=hisilicon-hi35xx-btools
#PKG_VERSION:=0.1
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(INCLUDE_DIR)/package.mk

define Package/hisilicon-hi35xx-btools/Default
  SECTION:=base
  CATEGORY:=Base system
  MAINTAINER:=Zeta Labs <zetalabs@gmail.com>
  DEPENDS:=@TARGET_hi35xx
  TITLE:=Proprietary Hisilicon hi35xx board tools
endef

define Package/hi3515-btools
$(call Package/hisilicon-hi35xx-btools/Default)
  TITLE+= (hi3515)
endef

define Package/hi3515-btools/description
 This package contains the proprietary board tools (btools) for configuring the 
 proprietary Hisilicon HI3515 drivers.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/hi3515
endef

define Build/Compile
	$(CP) files/hi3515_btools $(PKG_BUILD_DIR)/hi3515/btools
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
endef

define Package/hi3515-btools/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hi3515/btools $(1)/sbin/
endef

$(eval $(call BuildPackage,hi3515-btools))
