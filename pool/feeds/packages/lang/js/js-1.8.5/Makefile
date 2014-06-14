#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=js
PKG_VERSION:=1.8.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)185-1.0.0.tar.gz
PKG_SOURCE_URL:=http://ftp.mozilla.org/pub/mozilla.org/js
PKG_MD5SUM:=a4574365938222adca0a6bd33329cb32

#PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/js/Default
  SUBMENU:=JavaScript
  SECTION:=lang
  CATEGORY:=Languages
  URL:=http://developer.mozilla.org/en/SpiderMonkey
endef

define Package/js
  $(call Package/js/Default)
  TITLE:=SpiderMonkey JavaScript engine
endef

export CROSS_COMPILE=1

ifeq ($(ARCH),i386)
#TARGET_CFLAGS += -DHAVE_VA_LIST_AS_ARRAY
HOST_CFLAGS += -DFORCE_LITTLE_ENDIAN
endif
ifeq ($(ARCH),x86_64)
TARGET_CFLAGS += -DHAVE_VA_LIST_AS_ARRAY
HOST_CFLAGS += -DFORCE_LITTLE_ENDIAN
endif
ifeq ($(ARCH),mipsel)
#TARGET_CFLAGS += -DHAVE_VA_LIST_AS_ARRAY
HOST_CFLAGS += -DFORCE_LITTLE_ENDIAN
endif


#CONFIGURE_ARGS += --disable-compile-environment

# Note:
#   In Mozilla, we use the names $target, $host and $build incorrectly, but are
#   too far gone to back out now. See Bug 475488:
#     - When we say $target, we mean $host, that is, the system on which
#       Mozilla will be run.
#     - When we say $host, we mean $build, that is, the system on which Mozilla
#       is built.
#     - $target (in its correct usage) is for compilers who generate code for a
#       different platform than $host, so it would not be used by Mozilla.

#CONFIGURE_ARGS += \
#	--target=$(GNU_TARGET_NAME) \
#	--host=$(GNU_HOST_NAME)

HOSTCXX:=g++

CONFIGURE_VARS += HOST_CC=$(HOSTCC)
CONFIGURE_VARS += HOST_CFLAGS="$(HOST_CFLAGS)"
CONFIGURE_VARS += HOST_LDFLAGS="$(HOST_LDFLAGS)"
CONFIGURE_VARS += HOST_CXX=$(HOSTCXX)
CONFIGURE_VARS += HOST_CXXFLAGS="$(HOST_CFLAGS)"

CONFIGURE_PATH:=js/src

#define Build/Configure
#endef

MAKE_PATH:=js/src

#define Build/Compile
#	$(call Build/Compile/Default,all)
#endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/js
	$(CP) $(PKG_INSTALL_DIR)/usr/include/js/*.h $(1)/usr/include/js/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/js/*.tbl $(1)/usr/include/js/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.{a,so*} $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/mozjs185.pc $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/bin $(2)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/js-config $(1)/usr/bin/
	$(SED) \
		's,^\(prefix\|exec_prefix\)=.*,\1=$(STAGING_DIR)/usr,g' \
		$(1)/usr/bin/js-config
	for f in js-config; do \
		$(LN) ../../usr/bin/$$$$f $(2)/bin/ ; \
	done
endef

define Package/js/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,js))
