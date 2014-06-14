#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=hisilicon-hi35xx-mpp
#PKG_VERSION:=0.1
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(INCLUDE_DIR)/package.mk

define Package/hisilicon-hi35xx-mpp/Default
  SECTION:=base
  CATEGORY:=Base system
  MAINTAINER:=Zeta Labs <zetalabs@gmail.com>
  DEPENDS:=@TARGET_hi35xx
  TITLE:=Proprietary Hisilicon hi35xx MPP(Media Process Platform) tools
endef

define Package/hi3515-mpp-sample
$(call Package/hisilicon-hi35xx-mpp/Default)
  TITLE+= (hi3515 sample)
endef

define Package/hi3515-mpp-sample/description
 This package contains the proprietary MPP samples.
endef

define Package/hi3515-mpp-sample/install
	$(INSTALL_DIR) $(1)/usr/share/mpp
	$(CP) $(PKG_BUILD_DIR)/hi3515/sample $(1)/usr/share/mpp/
endef

define Package/hi3515-mpp-libs
$(call Package/hisilicon-hi35xx-mpp/Default)
  TITLE+= (hi3515 libs)
endef

define Package/hi3515-mpp-libs/description
 This package contains the proprietary MPP libraries.
endef

define Package/hi3515-mpp-libs/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/hi3515/lib/lib*.* $(1)/usr/lib/
endef

define Package/hi3515-mpp-rcso
$(call Package/hisilicon-hi35xx-mpp/Default)
  TITLE+= (hi3515 rc .so)
endef

define Package/hi3515-mpp-rcso/description
 This package contains the proprietary MPP rc .so.
endef

define Package/hi3515-mpp-rcso/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/hi3515/lib/*.sh $(1)/usr/lib/
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/hi3515/include
	mkdir -p $(PKG_BUILD_DIR)/hi3515/lib
	mkdir -p $(PKG_BUILD_DIR)/hi3515/sample
endef

define Build/Compile
	$(CP) files/hi3515/include/*.h $(PKG_BUILD_DIR)/hi3515/include/
	$(CP) files/hi3515/lib/lib*.* $(PKG_BUILD_DIR)/hi3515/lib/
	$(CP) files/hi3515/lib/*.sh $(PKG_BUILD_DIR)/hi3515/lib/
	$(CP) files/hi3515/sample/* $(PKG_BUILD_DIR)/hi3515/sample/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/mpp
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/hi3515/include/*.h $(1)/usr/include/mpp/
	$(CP) $(PKG_BUILD_DIR)/hi3515/lib/lib*.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,hi3515-mpp-sample))
$(eval $(call BuildPackage,hi3515-mpp-libs))
$(eval $(call BuildPackage,hi3515-mpp-rcso))
