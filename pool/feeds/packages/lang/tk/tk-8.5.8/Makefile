#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tk
PKG_VERSION:=8.5.8
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)$(PKG_VERSION)-src.tar.gz
PKG_SOURCE_URL:=@SF/tcl
#PKG_MD5SUM:=7f123e53b3daaaba2478d3af5a0752e3

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(PKG_VERSION)

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_LDFLAGS += \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), -lnano-X, )

define Package/tk
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=TK graphical user interface toolkit
  URL:=http://www.tcl.tk
  DEPENDS:=+tcl +fontconfig +libfreetype \
  +FEATURE_drawing-backend_nxlib:nxlib \
  +FEATURE_drawing-backend_libX11:libX11
endef

define Package/tk/description
Tk is a graphical user interface toolkit that takes developing desktop 
applications to a higher level than conventional approaches. Tk is the 
standard GUI not only for Tcl, but for many other dynamic languages, 
and can produce rich, native applications that run unchanged across 
Windows, Mac OS X, Linux and more. 
endef

TARGET_LDFLAGS += \
	-lz \
	-lexpat \
	-lfreetype \
	-lfontconfig

TARGET_LDFLAGS += \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -lXau -lXdmcp, )

CONFIGURE_PATH := unix

CONFIGURE_VARS += \
	ac_cv_c_tclconfig=$(STAGING_DIR)/usr/lib

CONFIGURE_ARGS += \
	--x-includes=$(STAGING_DIR)/usr/include \
	--x-libraries=$(STAGING_DIR)/usr/lib \
	--enable-threads \
	--disable-xss \
	--with-tcl=$(STAGING_DIR)/usr/bin \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), --with-x="yes", \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), --with-x="yes", --with-x="no"))

XFT_CONFIG := $(STAGING_DIR)/host/bin/xft-config

CONFIGURE_ARGS += \
	$(if $(CONFIG_PACKAGE_libXft), \
		--enable-xft --with-xft-config=$(XFT_CONFIG), \
		--disable-xft)

MAKE_PATH := unix

TARGET_LIB_RUNTIME_DIR := $(STAGING_DIR)/usr/lib

CONFIGURE_ARGS += \
	--with-target-lib-runtime-dir=$(TARGET_LIB_RUNTIME_DIR)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/*.h $(1)/usr/include/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libtk*.{a,so*} $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/tkConfig.sh $(1)/usr/lib/
	$(SED) 's,-I/usr/include,-I$(STAGING_DIR)/usr/include,g' $(1)/usr/lib/tkConfig.sh
	$(SED) 's,-L/usr/lib,-L$(STAGING_DIR)/usr/lib,g' $(1)/usr/lib/tkConfig.sh
endef

define Package/tk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so $(1)/usr/lib

	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,tk))
