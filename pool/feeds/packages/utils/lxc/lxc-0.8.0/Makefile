#
# Copyright (C) 2012 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lxc
PKG_VERSION:=0.8.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=@SF/lxc
PKG_SOURCE_URL:=http://lxc.sourceforge.net/download/lxc/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/lxc
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=LXC userspace tools
  URL:=http://lxc.sourceforge.net/
  DEPENDS:= +libcap +apparmor
endef

define Package/lxc/description
  Userspace tools to manage linux containers (LXC)
endef

CONFIGURE_ARGS += \
	--with-linuxdir="$(LINUX_DIR)"

define Package/lxc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-attach $(1)/usr/bin/lxc-attach
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-cgroup $(1)/usr/bin/lxc-cgroup
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-checkconfig $(1)/usr/bin/lxc-checkconfig
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-checkpoint $(1)/usr/bin/lxc-checkpoint
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-clone $(1)/usr/bin/lxc-clone
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-console $(1)/usr/bin/lxc-console
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-create $(1)/usr/bin/lxc-create
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-destroy $(1)/usr/bin/lxc-destroy
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-execute $(1)/usr/bin/lxc-execute
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-freeze $(1)/usr/bin/lxc-freeze
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-info $(1)/usr/bin/lxc-info
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-kill $(1)/usr/bin/lxc-kill
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-ls $(1)/usr/bin/lxc-ls
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-monitor $(1)/usr/bin/lxc-monitor
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-netstat $(1)/usr/bin/lxc-netstat
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-ps $(1)/usr/bin/lxc-ps
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-restart $(1)/usr/bin/lxc-restart
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-setcap $(1)/usr/bin/lxc-setcap
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-setuid $(1)/usr/bin/lxc-setuid
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-start $(1)/usr/bin/lxc-start
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-stop $(1)/usr/bin/lxc-stop
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-unfreeze $(1)/usr/bin/lxc-unfreeze
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-unshare $(1)/usr/bin/lxc-unshare
	# $(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-version $(1)/usr/bin/lxc-version
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/lxc-wait $(1)/usr/bin/lxc-wait

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liblxc.so* $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/lib/lxc
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/lxc/lxc-init $(1)/usr/lib/lxc/

	$(INSTALL_DIR) $(1)/usr/lib/lxc/rootfs
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/lxc/rootfs/README $(1)/usr/lib/lxc/rootfs/

	$(INSTALL_DIR) $(1)/var/lib/lxc
endef

$(eval $(call BuildPackage,lxc))
