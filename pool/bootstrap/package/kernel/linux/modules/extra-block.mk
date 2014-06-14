#
# Copyright (C) 2008-2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

BLOCK_MENU:=Block Devices


define KernelPackage/pata-sis
  TITLE:=SiS PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_SIS
  FILES=$(LINUX_DIR)/drivers/ata/pata_sis.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-sis,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-sis/description
  Kernel module for the SiS PATA controllers
endef

$(eval $(call KernelPackage,pata-sis))


define KernelPackage/pata-ali
  TITLE:=ALi PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_ALI
  FILES=$(LINUX_DIR)/drivers/ata/pata_ali.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-ali,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-ali/description
  Kernel module for the ALi ATA interfaces found on the many ALi chipsets
endef

$(eval $(call KernelPackage,pata-ali))


define KernelPackage/pata-amd
  TITLE:=AMD/NVidia PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_AMD
  FILES=$(LINUX_DIR)/drivers/ata/pata_amd.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-amd,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-amd/description
  Kernel module for AMD and NVidia PATA interfaces found on the chipsets for Athlon/Athlon64
endef

$(eval $(call KernelPackage,pata-amd))


#define KernelPackage/pata-artop
#  TITLE:=ARTOP 6210/6260 PATA support
#  DEPENDS:=@PCI_SUPPORT
#  KCONFIG:=CONFIG_PATA_ARTOP
#  FILES=$(LINUX_DIR)/drivers/ata/pata_artop.ko
#  AUTOLOAD:=$(call AutoLoad,30,pata-artop,1)
#  $(call AddDepends/ata)
#endef

#define KernelPackage/pata-artop/description
#  Kernel module for ARTOP PATA controllers
#endef

#$(eval $(call KernelPackage,pata-artop))


define KernelPackage/pata-atp867x
  TITLE:=ARTOP/Acard ATP867X PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_ATP867X
  FILES=$(LINUX_DIR)/drivers/ata/pata_atp867x.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-atp867x,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-atp867x/description
  Kernel module for ARTOP/Acard ATP867X PATA controllers
endef

$(eval $(call KernelPackage,pata-atp867x))


define KernelPackage/pata-atiixp
  TITLE:=ATI PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_ATIIXP
  FILES=$(LINUX_DIR)/drivers/ata/pata_atiixp.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-atiixp,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-atiixp/description
  Kernel module for the ATI ATA interfaces found on the many ATI chipsets
endef

$(eval $(call KernelPackage,pata-atiixp))


define KernelPackage/pata-cmd64x
  TITLE:=CMD64x PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_CMD64X
  FILES=$(LINUX_DIR)/drivers/ata/pata_cmd64x.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-cmd64x,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-cmd64x/description
  Kernel module for the CMD64x series chips except for the CMD640.
endef

$(eval $(call KernelPackage,pata-cmd64x))


define KernelPackage/pata-cs5520
  TITLE:=CS5510/5520 PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_CS5520
  FILES=$(LINUX_DIR)/drivers/ata/pata_cs5520.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-cs5520,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-cs5520/description
  Kernel module for the Cyrix 5510/5520
  companion chip used with the MediaGX/Geode processor family
endef

$(eval $(call KernelPackage,pata-cs5520))


define KernelPackage/pata-cs5530
  TITLE:=CS5530 PATA support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_CS5530
  FILES=$(LINUX_DIR)/drivers/ata/pata_cs5530.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-cs5530,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-cs5530/description
  Kernel module for the Cyrix/NatSemi/AMD CS5530
  companion chip used with the MediaGX/Geode processor family
endef

$(eval $(call KernelPackage,pata-cs5530))


define KernelPackage/pata-efar
  TITLE:=EFAR SLC90E66 support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_EFAR
  FILES=$(LINUX_DIR)/drivers/ata/pata_efar.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-efar,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-efar/description
  Kernel module for the EFAR SLC90E66
  IDE controller found on some older machines
endef

$(eval $(call KernelPackage,pata-efar))


define KernelPackage/pata-mpiix
  TITLE:=Intel PATA MPIIX support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_MPIIX
  FILES=$(LINUX_DIR)/drivers/ata/pata_mpiix.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-mpiix,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-mpiix/description
  Kernel module for MPIIX PATA support
endef

$(eval $(call KernelPackage,pata-mpiix))


define KernelPackage/pata-oldpiix
  TITLE:=Intel PATA old PIIX support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_PATA_OLDPIIX
  FILES=$(LINUX_DIR)/drivers/ata/pata_oldpiix.ko
  AUTOLOAD:=$(call AutoLoad,30,pata-oldpiix,1)
  $(call AddDepends/ata)
endef

define KernelPackage/pata-oldpiix/description
  Kernel module for early PIIX PATA support
endef

$(eval $(call KernelPackage,pata-oldpiix))

