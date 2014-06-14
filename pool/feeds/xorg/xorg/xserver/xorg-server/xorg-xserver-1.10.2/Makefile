#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xorg-server
PKG_VERSION:=1.10.2
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://xorg.freedesktop.org/releases/individual/xserver/
PKG_SOURCE:=xorg-server-$(PKG_VERSION).tar.bz2
PKG_MD5SUM:=c9ba50bd44ea70da51f13100336a5484

PKG_BUILD_DIR:=$(BUILD_DIR)/Xorg/xserver/$(PKG_NAME)-$(PKG_VERSION)/
PKG_BUILD_DEPENDS:=randrproto renderproto fixesproto damageproto xcmiscproto \
				   xextproto xproto scrnsaverproto bigreqsproto resourceproto \
				   fontsproto inputproto kbproto videoproto resourceproto \
				   calibrateproto xineramaproto \
				   compositeproto xf86dgaproto xtrans xf86driproto dri2proto

PKG_INSTALL:=1
PKG_FIXUP:=libtool
PKG_BUILD_PARALLEL:=1

TARGET_LDFLAGS+= \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib

PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_xserver-xorg CONFIG_PACKAGE_xserver-common

include $(INCLUDE_DIR)/package.mk

X_CONFIG=generic
ifeq ($(CONFIG_TARGET_olpc),y)
  X_CONFIG=olpc
endif
ifeq ($(CONFIG_TARGET_s3c24xx),y)
  X_CONFIG=om_gta02
endif
ifeq ($(CONFIG_TARGET_omap24xx_n810-gui),y)
  X_CONFIG=nokia_n810
endif

define Package/xorg/Default
  SECTION:=xorg-server
  CATEGORY:=Xorg
  SUBMENU:=server
  DEPENDS:=@!avr32
  URL:=http://xorg.freedesktop.org/
endef

define Package/xserver-xorg
$(call Package/xorg/Default)
  PROVIDES:=xserver
  TITLE:=Xorg xserver
  DEPENDS:=+xserver-common \
  +libpthread \
  +xkbcomp \
  +libxkbfile \
  +libXfont \
  +libXau \
  +libXv \
  +libfontenc \
  +pixman \
  +libopenssl \
  +xkeyboard-config \
  +libdrm \
  +libgl-mesa \
  +libgcrypt \
  +libpciaccess \
  +udev \
  @DISPLAY_SUPPORT
endef

define Package/xserver-common
$(call Package/xorg/Default)
  TITLE:=meta-package
endef

define Build/Configure
	$(call Build/Configure/Default, \
		$(if $(CONFIG_PACKAGE_xserver-kdrive-xfbdev), \
			--enable-xfbdev, \
			--disable-xfbdev) \
		$(if $(CONFIG_PACKAGE_xserver-kdrive-xephyr), \
			--enable-xephyr, \
			--disable-xephyr) \
		$(if $(CONFIG_X_TSLIB),--enable-tslib,--disable-tslib) \
		$(if $(CONFIG_X_COMPOSITE),--enable-composite,--disable-composite) \
		$(if $(CONFIG_X_XINERAMA),--enable-xinerama,--disable-xinerama) \
		$(if $(CONFIG_X_XCALIBRATE),--enable-xcalibrate,--disable-xcalibrate) \
		--disable-dependency-tracking \
		--disable-debug \
		--disable-unit-tests \
		--disable-builddocs \
		--disable-glx-tls \
		--disable-shm \
		--disable-xres \
		--disable-record \
		--enable-xv \
		--enable-xvmc \
		--disable-dga \
		--disable-screensaver \
		--enable-xdmcp \
		--disable-xdm-auth-1 \
		--disable-glx \
		--enable-dri \
		--enable-dri2 \
		--disable-xf86vidmode \
		--disable-xace \
		--disable-xselinux \
		--disable-xsecurity \
		--disable-multibuffer \
		--disable-dbe \
		--disable-xf86bigfont \
		--enable-dpms \
		--disable-config-dbus \
		--disable-config-hal \
		--disable-xfree86-utils \
		--disable-xaa \
		--enable-vgahw \
		--disable-vbe \
		--enable-int10-module \
		--enable-xorg \
		--disable-dmx \
		--disable-xvfb \
		--disable-xnest \
		--disable-xquartz \
		--disable-standalone-xpbproxy \
		--disable-xwin \
		--disable-kdrive \
		--disable-xfake \
		--disable-xfbdev \
		--disable-installsetuid \
		--disable-secure-rpc \
		--enable-silent-rules \
		--disable-shave \
		--disable-xsdl \
		--disable-kdrive-kbd \
		--disable-kdrive-mouse \
		--disable-kdrive-evdev \
		--with-default-font-path=built-ins \
	)
endef

define Package/xserver-common/install
	$(INSTALL_DIR) $(1)/etc/{init.d,config,X11}
	$(INSTALL_BIN) \
		./files/x.init \
		$(1)/etc/init.d/x11
	$(INSTALL_DATA) ./files/tmp/x11.uci \
		$(1)/etc/config/x11
	test -f ./files/config.xorg/$(X_CONFIG) && \
		$(INSTALL_DATA) \
			./files/config.xorg/$(X_CONFIG) \
			$(1)/etc/X11/xorg.conf
#	test -f ./files/config.uci/$(X_CONFIG) && \
#		$(INSTALL_DATA) \
#			./files/config.uci/$(X_CONFIG) \
#			$(1)/etc/config/x11
endef

define Package/xserver-xorg/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/bin/Xorg \
		$(1)/usr/bin/

	$(INSTALL_DIR) $(1)/usr/lib/xorg/modules/{extensions,fonts,linux,multimedia}
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/xorg/modules/*.so \
		$(1)/usr/lib/xorg/modules/

	$(foreach d,extensions fonts linux multimedia, \
		$(INSTALL_DATA) \
			$(PKG_INSTALL_DIR)/usr/lib/xorg/modules/$(d)/*.so \
			$(1)/usr/lib/xorg/modules/$(d)/; \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/share/aclocal/
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/share/aclocal/* \
		$(1)/usr/share/aclocal/

	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/* \
		$(1)/usr/lib/pkgconfig/

	$(if $(CONFIG_PACKAGE_xserver-xorg), \
		$(INSTALL_DIR) $(1)/usr/include/xorg; \
		$(INSTALL_DATA) \
			$(PKG_INSTALL_DIR)/usr/include/xorg/*.h \
			$(1)/usr/include/xorg/; \
	)
	$(INSTALL_DIR) $(1)/mk
	$(INSTALL_DATA) \
		./files/xorg-driver.mk \
		$(1)/mk/
	$(SED) 's,@VERSION@,$(PKG_VERSION),' $(1)/mk/xorg-driver.mk
endef

$(eval $(call BuildPackage,xserver-common))
$(eval $(call BuildPackage,xserver-xorg))
