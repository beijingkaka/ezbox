#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=emc2
PKG_VERSION:=20110710
PKG_REV:=7857400f1aeb9fa93b076508cda0b2b8c48b1d0d
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=git://git.linuxcnc.org/git/emc2.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)

PKG_INSTALL:=1
PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/emc2
  SECTION:=utils
  CATEGORY:=Real-Time
  TITLE:=EMC (the Enhanced Machine Control)
  DEPENDS:=+rtai +glib2 +tcl +tk \
  +libreadline +libncurses \
  +USE_UCLIBC:librpc \
  +FEATURE_gui-toolkit_FLTK:fltk \
  +FEATURE_gui-toolkit_FLTK2:fltk2 \
  +FEATURE_gui-toolkit_GTK2:gtk2 \
  +FEATURE_drawing-backend_nxlib:nxlib \
  +FEATURE_drawing-backend_libX11:libX11 \
  +FEATURE_drawing-backend_libX11:libXaw
  URL:=http://www.linuxcnc.org/
endef

define Package/emc2/description
  EMC (the Enhanced Machine Control) is a software system for
  computer control of machine tools such as milling machines
  and lathes.
endef

#CONFIGURE_VARS += \
#	ac_cv_lib_readline_readline=yes
CONFIGURE_VARS += \
	ac_cv_path_AR="$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS)ar"

define Build/Configure
	(cd $(PKG_BUILD_DIR)/src; autoreconf --install --symlink );
	$(call Build/Configure/Default, \
		--disable-simulator \
		$(if $(CONFIG_FEATURE_gui-toolkit_FLTK2), --enable-fltk2, --disable-fltk2) \
		$(if $(CONFIG_FEATURE_gui-toolkit_GTK2), --enable-gtk, --disable-gtk) \
		--disable-build-documentation \
		--disable-python \
		--with-realtime="$(STAGING_DIR)/usr/realtime" \
		--with-module-dir="/usr/realtime/modules" \
		--with-tclConfig="$(STAGING_DIR)/usr/lib/tclConfig.sh" \
		--with-tkConfig="$(STAGING_DIR)/usr/lib/tkConfig.sh" \
		$(if $(CONFIG_FEATURE_drawing-backend_libX11), --with-x="yes", --with-x="no") \
		, AR="$(TARGET_CROSS)ar" \
		,src)
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include

TARGET_EXTRA_LIBS:= -lpng -lz -lexpat -lintl $(LIBRPC)
FLTK2_EXTRA_LIBS:= 
GTK2_EXTRA_LIBS:= -lpixman-1

TARGET_EXTRA_LIBS += -lncurses

TARGET_EXTRA_LIBS += -L$(STAGING_DIR)/usr/realtime/lib -llxrt -lpthread

TARGET_LDFLAGS += \
	$(TARGET_EXTRA_LIBS) \
	$(if $(CONFIG_FEATURE_gui-toolkit_FLTK2), $(FLK2_EXTRA_LIBS), ) \
	$(if $(CONFIG_FEATURE_gui-toolkit_GTK2), $(GTK2_EXTRA_LIBS), ) \
	$(if $(CONFIG_FEATURE_drawing-backend_nxlib), -lX11 -lnano-X, ) \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -lX11 -lXau -lXdmcp -lxcb, )

TARGET_LDFLAGS += \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -lXaw -lXext -lXmu -lXt -lXpm, ) \
	$(if $(CONFIG_FEATURE_drawing-backend_libX11), -luuid -lICE -lSM, )

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		V=1 \
		BUILD_VERBOSE=1 \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		TARGET_CXXFLAGS="$(TARGET_CFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		CC="$(TARGET_CC)" \
		AR="$(TARGET_CROSS)ar"
endef

define Build/Install
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		V=1 \
		BUILD_VERBOSE=1 \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		TARGET_CXXFLAGS="$(TARGET_CFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		CC="$(TARGET_CC)" \
		AR="$(TARGET_CROSS)ar" \
		install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/emc2/install
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/bin $(1)/etc
	$(INSTALL_DIR) $(1)/usr/realtime/modules/emc2
	$(INSTALL_DIR) $(1)/usr/share
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/etc/* $(1)/etc/
	$(CP) $(PKG_INSTALL_DIR)/usr/realtime/modules/emc2/*.ko $(1)/usr/realtime/modules/emc2/
	$(CP) $(PKG_INSTALL_DIR)/usr/share/* $(1)/usr/share/
endef

$(eval $(call BuildPackage,emc2))
