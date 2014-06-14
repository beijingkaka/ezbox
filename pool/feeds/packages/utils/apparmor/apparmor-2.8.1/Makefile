#
# Copyright (C) 2013 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=apparmor
PKG_VERSION:=2.8.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://launchpad.net/apparmor/2.8/2.8.1/+download/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/apparmor
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=AppArmor userspace tools
  URL:=http://wiki.apparmor.net/
endef

define Package/apparmor/description
  Userspace tools to manage AppArmor, an effective and easy-to-use Linux application security system
endef

#CONFIGURE_ARGS += \
#	--with-linuxdir="$(LINUX_DIR)"

define Build/Configure
	(cd $(PKG_BUILD_DIR)/libraries/libapparmor ; \
		aclocal; \
		autoreconf --install --symlink --force; \
		libtoolize --automake; \
		automake -ac);
	$(call Build/Configure/Default, \
		, \
		, \
		libraries/libapparmor )
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/libraries/libapparmor \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		install
endef

define Build/Install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/aalogparse \
		$(1)/usr/include/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/sys \
		$(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/libapparmor.{a,so*} \
		$(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libapparmor.pc \
		$(1)/usr/lib/pkgconfig/
endef

define Package/apparmor/install
	$(INSTALL_DIR) $(1)/usr/bin

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libapparmor.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,apparmor))
