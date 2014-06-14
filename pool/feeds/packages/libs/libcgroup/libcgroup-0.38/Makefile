#
# Copyright (C) 2008-2013 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libcgroup
PKG_VERSION:=0.38
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/libcg
PKG_MD5SUM:=f0f7d4060bf36ccc19d75dbf4f1695db

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libcgroup/Default
  SECTION:=libs
  CATEGORY:=Libraries
  URL:=http://sourceforge.net/projects/libcg/
endef

define Package/libcgroup
$(call Package/libcgroup/Default)
  TITLE:=Control Group Configuration (library)
  DEPENDS:=+libpthread
endef

define Package/libcgroup/description
  Control groups, a new kernel feature in Linux 2.6.24 provides a file system interface to manipulate and control the details on task grouping including creation of new task groups (control groups), permission handling and task assignment.
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--enable-debug \
	--disable-pam \


define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libcgroup.h $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libcgroup $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcgroup.{a,la,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libcgroup.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libcgroup/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcgroup.so.* $(1)/usr/lib/
endef

define Package/cgroup-bin
$(call Package/libcgroup/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Control Group Configuration (utils)
  DEPENDS:=+libcgroup +librt
endef

define Package/cgroup-bin/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/* $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,libcgroup))
$(eval $(call BuildPackage,cgroup-bin))
