#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=grub
PKG_VERSION:=2.00
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@GNU/grub
PKG_MD5SUM:=e927540b6eda8b024fb0391eeaa4091c

HOST_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=grub2/host

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/package.mk

RSTRIP:=/bin/true
export RSTRIP

define Package/grub2
  SUBMENU:=Boot Loaders
  CATEGORY:=Utilities
  SECTION:=utils
  TITLE:=GRand Unified Bootloader
  URL:=http://www.gnu.org/software/grub/
  DEPENDS:=@(TARGET_x86||TARGET_x86_64) +libdevmapper
endef

define Package/grub2-editenv
  CATEGORY:=Utilities
  SECTION:=utils
  TITLE:=Grub2 Environment editor
  URL:=http://www.gnu.org/software/grub/
  DEPENDS:=@(TARGET_x86||TARGET_x86_64) +libdevmapper
endef

define Package/grub2-editenv/description
	Edit grub2 environment files.
endef

CONFIGURE_ARGS += \
	--target=$(REAL_GNU_TARGET_NAME) \
	--disable-grub-mkfont \
	--disable-auto-linux-mem-opt \
	--disable-werror \
	--disable-nls

HOST_CONFIGURE_ARGS += \
	--target=$(REAL_GNU_TARGET_NAME) \
	--sbindir="$(STAGING_DIR_HOST)/bin" \
	--disable-werror \
	--disable-nls

HOST_MAKE_FLAGS += \
	TARGET_RANLIB=$(TARGET_RANLIB) \
	LIBLZMA=$(STAGING_DIR_HOST)/lib/liblzma.a

define Host/Configure
	$(SED) 's,(RANLIB),(TARGET_RANLIB),' $(HOST_BUILD_DIR)/grub-core/Makefile.in
	$(Host/Configure/Default)
endef

define Package/grub2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/grub-* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/grub-* $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib/grub
	$(INSTALL_DIR) $(1)/usr/lib/grub/i386-pc
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/grub/i386-pc/* $(1)/usr/lib/grub/i386-pc/
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/usr/share/grub
	$(CP) $(PKG_INSTALL_DIR)/usr/share/grub/* $(1)/usr/share/grub/
	$(INSTALL_DIR) $(1)/etc/grub.d
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/etc/grub.d/*_* $(1)/etc/grub.d/
endef

define Package/grub2-editenv/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/grub-editenv $(1)/usr/sbin/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,grub2))
$(eval $(call BuildPackage,grub2-editenv))
