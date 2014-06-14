#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=qemu
PKG_VERSION:=1.1.1
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://wiki.qemu.org/download/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-1.tar.bz2

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
#include $(INCLUDE_DIR)/kernel.mk
ifeq ($(CONFIG_BUILD_NLS),y)
include $(INCLUDE_DIR)/nls.mk
endif

define Package/qemu/Default
  SECTION:=utils
  CATEGORY:=Utilities
  URL:=http://wiki.qemu.org/
endef

define Package/qemu
  $(call Package/qemu/Default)
  MENU:=1
  DEPENDS:= +glib2
  TITLE:=QEMU
endef

define Package/qemu/description
  QEMU is a generic and open source machine emulator and virtualizer.
endef

define Package/qemu-system-alpha
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for alpha
endef

define Package/qemu-system-arm
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for ARM
endef

define Package/qemu-system-cris
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for Cris
endef

define Package/qemu-system-i386
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for i386
endef

define Package/qemu-system-lm32
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for LM32
endef

define Package/qemu-system-m68k
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for M68K
endef

define Package/qemu-system-microblaze
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for microblaze
endef

define Package/qemu-system-microblazeel
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for microblazeel
endef

define Package/qemu-system-mips
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for MIPS
endef

define Package/qemu-system-mips64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for MIPS64
endef

define Package/qemu-system-mips64el
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for MIPS64EL
endef

define Package/qemu-system-mipsel
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for MIPSEL
endef

define Package/qemu-system-ppc
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for PPC
endef

define Package/qemu-system-ppc64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for PPC64
endef

define Package/qemu-system-ppcemb
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for PPCEMB
endef

define Package/qemu-system-s390x
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for S390
endef

define Package/qemu-system-sh4
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for SH4
endef

define Package/qemu-system-sh4eb
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for SH4EB
endef

define Package/qemu-system-sparc
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for SPARC
endef

define Package/qemu-system-sparc64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for SPARC64
endef

define Package/qemu-system-x86_64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for X86_64
endef

define Package/qemu-system-xtensa
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for XTENSA
endef

define Package/qemu-system-xtensaeb
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU System Emulator for XTENSAEB
endef

define Package/qemu-user-alpha
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for alpha
endef

define Package/qemu-user-arm
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for ARM
endef

define Package/qemu-user-armeb
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for ARMEB
endef

define Package/qemu-user-cris
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for CRIS
endef

define Package/qemu-user-i386
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for i386
endef

define Package/qemu-user-m68k
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for m68k
endef

define Package/qemu-user-microblaze
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for microblaze
endef

define Package/qemu-user-microblazeel
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for microblazeel
endef

define Package/qemu-user-mips
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for MIPS
endef

define Package/qemu-user-mipsel
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for MIPSEL
endef

define Package/qemu-user-ppc
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for PPC
endef

define Package/qemu-user-ppc64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for PPC64
endef

define Package/qemu-user-ppc64abi32
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for PPC64 ABI32
endef

define Package/qemu-user-s390x
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for S390
endef

define Package/qemu-user-sh4
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for SH4
endef

define Package/qemu-user-sh4eb
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for SH4EB
endef

define Package/qemu-user-sparc
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for SPARC
endef

define Package/qemu-user-sparc32plus
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for SPARC32PLUS
endef

define Package/qemu-user-sparc64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for SPARC64
endef

define Package/qemu-user-x86_64
  $(call Package/qemu/Default)
  DEPENDS:=qemu
  TITLE:=QEMU User Emulator for X86_64
endef

CONFIGURE_ARGS += --disable-unknown-option-check
#CONFIGURE_ARGS += --extra-cflags="$(TARGET_CFLAGS)"
CONFIGURE_ARGS += --extra-ldflags="-lrt -lm"

define Build/Configure
	$(call Build/Configure/Default)
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/qemu/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-ga $(1)/usr/bin/qemu-ga
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-img $(1)/usr/bin/qemu-img
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-io $(1)/usr/bin/qemu-io
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-nbd $(1)/usr/bin/qemu-nbd
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-unicore32 $(1)/usr/bin/qemu-unicore32

	$(INSTALL_DIR) $(1)/usr/libexec/
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/* $(1)/usr/libexec/

	$(INSTALL_DIR) $(1)/usr/share/
	$(CP) $(PKG_INSTALL_DIR)/usr/share/qemu $(1)/usr/share/
endef

define Package/qemu-system-alpha/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-alpha $(1)/usr/bin/qemu-system-alpha
endef

define Package/qemu-system-arm/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-arm $(1)/usr/bin/qemu-system-arm
endef

define Package/qemu-system-cris/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-cris $(1)/usr/bin/qemu-system-cris
endef

define Package/qemu-system-i386/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-i386 $(1)/usr/bin/qemu-system-i386
endef

define Package/qemu-system-lm32/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-lm32 $(1)/usr/bin/qemu-system-lm32
endef

define Package/qemu-system-m68k/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-m68k $(1)/usr/bin/qemu-system-m68k
endef

define Package/qemu-system-microblaze/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-microblaze $(1)/usr/bin/qemu-system-microblaze
endef

define Package/qemu-system-microblazeel/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-microblazeel $(1)/usr/bin/qemu-system-microblazeel
endef

define Package/qemu-system-mips/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-mips $(1)/usr/bin/qemu-system-mips
endef

define Package/qemu-system-mips64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-mips64 $(1)/usr/bin/qemu-system-mips64
endef

define Package/qemu-system-mips64el/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-mips64el $(1)/usr/bin/qemu-system-mips64el
endef

define Package/qemu-system-mipsel/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-mipsel $(1)/usr/bin/qemu-system-mipsel
endef

define Package/qemu-system-ppc/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-ppc $(1)/usr/bin/qemu-system-ppc
endef

define Package/qemu-system-ppc64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-ppc64 $(1)/usr/bin/qemu-system-ppc64
endef

define Package/qemu-system-ppcemb/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-ppcemb $(1)/usr/bin/qemu-system-ppcemb
endef

define Package/qemu-system-s390x/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-s390x $(1)/usr/bin/qemu-system-s390x
endef

define Package/qemu-system-sh4/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-sh4 $(1)/usr/bin/qemu-system-sh4
endef

define Package/qemu-system-sh4eb/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-sh4eb $(1)/usr/bin/qemu-system-sh4eb
endef

define Package/qemu-system-sparc/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-sparc $(1)/usr/bin/qemu-system-sparc
endef

define Package/qemu-system-sparc64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-sparc64 $(1)/usr/bin/qemu-system-sparc64
endef

define Package/qemu-system-x86_64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-x86_64 $(1)/usr/bin/qemu-system-x86_64
endef

define Package/qemu-system-xtensa/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-xtensa $(1)/usr/bin/qemu-system-xtensa
endef

define Package/qemu-system-xtensaeb/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-system-xtensaeb $(1)/usr/bin/qemu-system-xtensaeb
endef

define Package/qemu-user-alpha/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-alpha $(1)/usr/bin/qemu-alpha
endef

define Package/qemu-user-arm/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-arm $(1)/usr/bin/qemu-arm
endef

define Package/qemu-user-armeb/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-armeb $(1)/usr/bin/qemu-armeb
endef

define Package/qemu-user-cris/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-cris $(1)/usr/bin/qemu-cris
endef

define Package/qemu-user-i386/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-i386 $(1)/usr/bin/qemu-i386
endef

define Package/qemu-user-m68k/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-m68k $(1)/usr/bin/qemu-m68k
endef

define Package/qemu-user-microblaze/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-microblaze $(1)/usr/bin/qemu-microblaze
endef

define Package/qemu-user-microblazeel/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-microblazeel $(1)/usr/bin/qemu-microblazeel
endef

define Package/qemu-user-mips/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-mips $(1)/usr/bin/qemu-mips
endef

define Package/qemu-user-mipsel/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-mipsel $(1)/usr/bin/qemu-mipsel
endef

define Package/qemu-user-ppc/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-ppc $(1)/usr/bin/qemu-ppc
endef

define Package/qemu-user-ppc64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-ppc64 $(1)/usr/bin/qemu-ppc64
endef

define Package/qemu-user-ppc64abi32/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-ppc64abi32 $(1)/usr/bin/qemu-ppc64abi32
endef

define Package/qemu-user-s390x/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-s390x $(1)/usr/bin/qemu-s390x
endef

define Package/qemu-user-sh4/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-sh4 $(1)/usr/bin/qemu-sh4
endef

define Package/qemu-user-sh4eb/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-sh4eb $(1)/usr/bin/qemu-sh4eb
endef

define Package/qemu-user-sparc/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-sparc $(1)/usr/bin/qemu-sparc
endef

define Package/qemu-user-sparc32plus/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-sparc32plus $(1)/usr/bin/qemu-sparc32plus
endef

define Package/qemu-user-sparc64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-sparc64 $(1)/usr/bin/qemu-sparc64
endef

define Package/qemu-user-x86_64/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qemu-x86_64 $(1)/usr/bin/qemu-x86_64
endef

$(eval $(call BuildPackage,qemu))
$(eval $(call BuildPackage,qemu-system-alpha))
$(eval $(call BuildPackage,qemu-system-arm))
$(eval $(call BuildPackage,qemu-system-cris))
$(eval $(call BuildPackage,qemu-system-i386))
$(eval $(call BuildPackage,qemu-system-lm32))
$(eval $(call BuildPackage,qemu-system-m68k))
$(eval $(call BuildPackage,qemu-system-microblaze))
$(eval $(call BuildPackage,qemu-system-microblazeel))
$(eval $(call BuildPackage,qemu-system-mips))
$(eval $(call BuildPackage,qemu-system-mips64))
$(eval $(call BuildPackage,qemu-system-mips64el))
$(eval $(call BuildPackage,qemu-system-mipsel))
$(eval $(call BuildPackage,qemu-system-ppc))
$(eval $(call BuildPackage,qemu-system-ppc64))
$(eval $(call BuildPackage,qemu-system-ppcemb))
$(eval $(call BuildPackage,qemu-system-s390x))
$(eval $(call BuildPackage,qemu-system-sh4))
$(eval $(call BuildPackage,qemu-system-sh4eb))
$(eval $(call BuildPackage,qemu-system-sparc))
$(eval $(call BuildPackage,qemu-system-sparc64))
$(eval $(call BuildPackage,qemu-system-x86_64))
$(eval $(call BuildPackage,qemu-system-xtensa))
$(eval $(call BuildPackage,qemu-system-xtensaeb))
$(eval $(call BuildPackage,qemu-user-alpha))
$(eval $(call BuildPackage,qemu-user-arm))
$(eval $(call BuildPackage,qemu-user-armeb))
$(eval $(call BuildPackage,qemu-user-cris))
$(eval $(call BuildPackage,qemu-user-i386))
$(eval $(call BuildPackage,qemu-user-m68k))
$(eval $(call BuildPackage,qemu-user-microblaze))
$(eval $(call BuildPackage,qemu-user-microblazeel))
$(eval $(call BuildPackage,qemu-user-mips))
$(eval $(call BuildPackage,qemu-user-mipsel))
$(eval $(call BuildPackage,qemu-user-ppc))
$(eval $(call BuildPackage,qemu-user-ppc64))
$(eval $(call BuildPackage,qemu-user-ppc64abi32))
$(eval $(call BuildPackage,qemu-user-s390x))
$(eval $(call BuildPackage,qemu-user-sh4))
$(eval $(call BuildPackage,qemu-user-sh4eb))
$(eval $(call BuildPackage,qemu-user-sparc))
$(eval $(call BuildPackage,qemu-user-sparc32plus))
$(eval $(call BuildPackage,qemu-user-sparc64))
$(eval $(call BuildPackage,qemu-user-x86_64))
