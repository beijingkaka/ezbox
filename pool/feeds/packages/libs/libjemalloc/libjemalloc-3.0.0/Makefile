# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#
# Sample usage:
#   env LD_PRELOAD="/usr/lib/libjemalloc.so" <application>

include $(TOPDIR)/rules.mk

PKG_NAME:=libjemalloc
PKG_VERSION:=3.0.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/jemalloc-$(PKG_VERSION)

PKG_SOURCE:=jemalloc-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.canonware.com/download/jemalloc/

PKG_FIXUP:=autoconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libjemalloc
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=general-purpose scalable concurrent malloc(3) implementation
  URL:=http://www.canonware.com/jemalloc/
endef

CONFIGURE_ARGS += \
	--enable-autogen \
	--with-jemalloc-prefix=je_ \
	--enable-cc-silence \
	ac_cv_path_AR=$(TARGET_CROSS)ar \
	ac_cv_path_LD=$(TARGET_CROSS)ld \
	je_cv_static_page_shift=12

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/jemalloc $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libjemalloc.{a,so*} $(1)/usr/lib/
endef

define Package/libjemalloc/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libjemalloc.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libjemalloc))
