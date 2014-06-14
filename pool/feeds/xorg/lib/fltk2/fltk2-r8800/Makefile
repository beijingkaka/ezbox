#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fltk2
PKG_REV:=8800
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://svn.easysw.com/public/fltk/fltk/trunk
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)/

PKG_FIXUP:=libtool
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/fltk2
  SECTION:=xorg-framework
  CATEGORY:=Xorg
  SUBMENU:=framework
  TITLE:=Fltk2
  URL:=http://fltk.org/
  DEPENDS:=@DISPLAY_SUPPORT +libstdcpp +libXft +zlib +libjpeg +libpng \
  +FEATURE_drawing-backend_nxlib:nxlib \
  +FEATURE_drawing-backend_libX11:libXi
endef

define Feature/FLTK2
  TARGET_NAME:=gui-toolkit
  TARGET_TITLE:=GUI Tool Kit
  NAME:=FLTK 2.0
endef

define Package/fltk2/description
    FLTK (pronounced <fulltick>) is a cross-platform C++ GUI toolkit for UNIX/Linux (X11), Microsoft Windows, and MacOS X. FLTK provides modern GUI functionality without the bloat and supports 3D graphics via OpenGL and its built-in GLUT emulation.
endef

TARGET_LDFLAGS += \
	-lz \
	-lexpat \
	-lfreetype \
	-lfontconfig \
	-lX11 \
	-lXrender \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -lxcb -lXau -lXdmcp, ) \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), -lnano-X, ) \

define Build/Configure
	(cd $(PKG_BUILD_DIR); autoconf );
	$(call Build/Configure/Default, --enable-jpeg --enable-zlib --enable-png --disable-gl --disable-xinerama --enable-shared --x-libraries=$(STAGING_DIR)/usr/lib --x-includes=$(STAGING_DIR)/usr/include)
endef

define Build/Compile
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) all
endef

define Build/Install
	DESTDIR="$(PKG_INSTALL_DIR)" $(MAKE) -C $(PKG_BUILD_DIR) install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/bin $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/fltk $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/fltk2-config $(1)/usr/bin/
	$(SED) \
		's,^\(prefix\|exec_prefix\)=.*,\1="$(STAGING_DIR)/usr",g' \
		 $(1)/usr/bin/fltk2-config
	$(INSTALL_DIR) $(2)/bin
	for f in fltk2-config; do \
		$(LN) ../../usr/bin/$$$$f $(2)/bin/ ; \
	done
endef

define Package/fltk2/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
endef

$(eval $(call Feature,FLTK2))
$(eval $(call BuildPackage,fltk2))
