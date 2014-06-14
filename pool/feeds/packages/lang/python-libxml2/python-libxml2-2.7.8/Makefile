#
# Copyright (C) 2008-2013 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-libxml2
PKG_VERSION:=2.7.8
PKG_RELEASE:=1

PKG_SOURCE:=libxml2-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://gd.tuwien.ac.at/languages/libxml/ \
        http://xmlsoft.org/sources/ \
        ftp://fr.rpmfind.net/pub/libxml/
PKG_MD5SUM:=8127a65e8c3b08856093099b52599c86

PKG_BUILD_DIR:=$(BUILD_DIR)/libxml2-$(PKG_VERSION)

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DEPENDS:=python

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1


include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/python-libxml2
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Python interface to Gnome XML library
  URL:=http://xmlsoft.org/
  DEPENDS:=+python-mini +libxml2 +libpthread +zlib
endef

define Package/python-libxml2/description
 This package contains a Python XML library module based on the libxml2
endef

define PyPackage/python-libxml2/filespec
+|$(PYTHON_PKG_DIR)/libxml2.py
+|$(PYTHON_PKG_DIR)/libxml2.so
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--enable-rebuild-docs=no \
	--with-c14n \
	--without-catalog \
	--with-debug \
        --without-docbook \
        --with-html \
        --without-ftp \
        --without-http \
        --without-iconv \
        --without-iso8859x \
        --without-legacy \
        --with-output \
        --without-pattern \
        --without-push \
        --with-python \
        --with-reader \
        --without-readline \
        --without-regexps \
        --with-sax1 \
        --with-schemas \
        --with-threads \
        --with-tree \
        --with-valid \
        --with-writer \
        --with-xinclude \
        --with-xpath \
        --with-xptr \
        --with-zlib \


define Build/Compile
	$(if $(Build/Compile/PyMod),,@echo Python packaging code not found.; false)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		INCLUDES="-I./include -I$(PYTHON_INC_DIR)" \
		all
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR="$(PKG_INSTALL_DIR)" install
endef

$(eval $(call PyPackage,python-libxml2))
$(eval $(call BuildPackage,python-libxml2))
