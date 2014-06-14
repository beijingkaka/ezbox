# Dhid client Makefile for OpenWrt
# For www.dhis.org ddns service
# created by Philipp A. Mohrenweiser <phiamo@...com>

include $(TOPDIR)/rules.mk

PKG_NAME:=dhisd
PKG_VERSION:=5.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=ftp://ftp.dhis.org/pub/dhis/
PKG_MD5SUM:=fd0ef833db2094c0398eb406b01a0435

include $(INCLUDE_DIR)/package.mk

define Package/dhisd
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  DEPENDS:=+libgmp
  TITLE:=DHIS Server
  URL:=http://www.dhis.org
endef

define Package/dhisgenkeys
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  DEPENDS:=+libgmp
  TITLE:=DHIS Key generator
  URL:=http://www.dhis.org
endef

define Package/dhisd/conffiles
/etc/dhisd.conf
endef

MAKE_FLAGS += LFLAGS="$(TARGET_LDFLAGS)"

define Package/dhisd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/dhisd $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/dhisd.conf.sample $(1)/etc/dhisd.conf
	$(CP) $(PKG_BUILD_DIR)/dhis.db.sample $(1)/etc/dhis.db
endef

define Package/dhisgenkeys/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/dhisgenkeys $(1)/usr/sbin
endef

$(eval $(call BuildPackage,dhisd))
$(eval $(call BuildPackage,dhisgenkeys))
