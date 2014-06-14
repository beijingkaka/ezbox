#
# Copyright (C) 2011 ezbox project
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/SMDK2410
  NAME:=Samsung SMDK2410
  PACKAGES:=
endef

define Profile/SMDK2410/Description
        Package set compatible with the Samsung SMDK2410. Contains I2C/LEDS/GPIO/Sensors support
endef
$(eval $(call Profile,SMDK2410))
