# 
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

JFFS2_BLOCKSIZE=128k
JFFS2OPTS += --little-endian --pagesize=0x800 --no-cleanmarkers --pad

define Image/BuildKernel
	$(TARGET_CROSS)objcopy -O binary -R .note -R .comment -S $(LINUX_DIR)/arch/arm/boot/compressed/vmlinux linux.bin
	mkimage -A arm -O linux -T kernel -C none -a c0800000 -e c0800000 -n "ezbox Kernel Image (Lilou)" -d linux.bin uImage

	cp uImage $(BIN_DIR)/$(IMG_PREFIX)-$(KERNEL)-uImage
endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(BIN_DIR)/$(IMG_PREFIX)-root.$(1))
endef

define Image/Build
	$(CP) $(KDIR)/root.$(1) $(BIN_DIR)/$(IMG_PREFIX)-root.$(1)
	$(call Image/Build/$(1),$(1))
endef


$(eval $(call BuildImage))
