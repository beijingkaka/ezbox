#
# Copyright (C) 2006-2008 OpenWrt.org
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fltk
#PKG_REV:=8800
#PKG_VERSION:=r$(PKG_REV)
PKG_VERSION:=1.3.0
PKG_RELEASE:=1

#PKG_SOURCE_PROTO:=svn
#PKG_SOURCE_VERSION:=$(PKG_REV)
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-source.tar.gz
#PKG_SOURCE_URL:=http://svn.easysw.com/public/fltk/fltk/trunk
PKG_SOURCE_URL:=http://ftp.easysw.com/pub/fltk/$(PKG_VERSION)
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/$(PKG_NAME)-$(PKG_VERSION)

PKG_FIXUP:=libtool
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/fltk
  SECTION:=xorg-framework
  CATEGORY:=Xorg
  SUBMENU:=framework
  TITLE:=FLTK 1.3
  URL:=http://fltk.org/
  DEPENDS:=@DISPLAY_SUPPORT +libstdcpp +libXft +zlib +libjpeg +libpng +libgl-mesa \
  +FEATURE_drawing-backend_nxlib:nxlib \
  +FEATURE_drawing-backend_libX11:libXi
endef

define Feature/FLTK
  TARGET_NAME:=gui-toolkit
  TARGET_TITLE:=GUI Tool Kit
  NAME:=FLTK 1.3
endef

define Package/fltk/description
    FLTK (pronounced <fulltick>) is a cross-platform C++ GUI toolkit for UNIX/Linux (X11), Microsoft Windows, and MacOS X. FLTK provides modern GUI functionality without the bloat and supports 3D graphics via OpenGL and its built-in GLUT emulation.
endef

TARGET_LDFLAGS += \
	-lz \
	-lexpat \
	-lfreetype \
	-lfontconfig \
	-lX11 \
	-lXrender \
	-lXext \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -lxcb -lXau -lXdmcp, ) \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), -lnano-X, ) \

define Build/Configure
	(cd $(PKG_BUILD_DIR); autoconf );
	$(call Build/Configure/Default, --enable-jpeg --enable-zlib --enable-png --enable-gl --disable-xinerama --enable-shared --x-libraries=$(STAGING_DIR)/usr/lib --x-includes=$(STAGING_DIR)/usr/include)
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
	$(CP) $(PKG_INSTALL_DIR)/usr/include/FL $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/fltk-config $(1)/usr/bin/
	$(SED) \
		's,^\(prefix\|exec_prefix\)=.*,\1="$(STAGING_DIR)/usr",g' \
		 $(1)/usr/bin/fltk-config
	$(INSTALL_DIR) $(2)/bin
	for f in fltk-config; do \
		$(LN) ../../usr/bin/$$$$f $(2)/bin/ ; \
	done
endef

define Package/fltk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
endef

$(eval $(call Feature,FLTK))
$(eval $(call BuildPackage,fltk))
