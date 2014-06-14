# Dhid client Makefile for OpenWrt
# For www.dhis.org ddns service
# created by Philipp A. Mohrenweiser <phiamo@...com>

include $(TOPDIR)/rules.mk

PKG_NAME:=dhid
PKG_VERSION:=5.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=ftp://ftp.dhis.org/pub/dhis/
PKG_MD5SUM:=47d1a54a87653aef424ef0f8ab9a0b20

include $(INCLUDE_DIR)/package.mk

define Package/dhid
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  DEPENDS:=+libgmp
  TITLE:=DHIS Client
  URL:=http://www.dhis.org
endef

define Package/dhid/conffiles
/etc/dhid.conf
endef

MAKE_FLAGS += LFLAGS="$(TARGET_LDFLAGS)"

define Package/dhid/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/dhid $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/dhid.conf.sample $(1)/etc/dhid.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/dhid.init $(1)/etc/init.d/dhid
endef

$(eval $(call BuildPackage,dhid))
