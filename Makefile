#
# ezbox project Makefile
#

# language settings
LC_ALL:=C
LANG:=C
export LC_ALL LANG

# base dir
BASE_DIR:=${CURDIR}/..

ifneq ($(wildcard $(BASE_DIR)/default.mk),)
  include $(BASE_DIR)/default.mk
endif

# build info
DISTRO ?= huangdi
BUILD_TYPE ?= testing
RELEASE_VERSION ?= 0.1
TARGET ?= x86
DEVICE_TYPE ?= ezbox
ARCH ?= i386
KERNEL_VERSION ?= default
RT_TYPE ?= none
DRAWING_BACKEND ?= none
GUI_TOOLKIT ?= none
export DEVICE_TYPE

# log settings
LOG_FILE ?= $(BASE_DIR)/$(shell date --iso=seconds)-$(DISTRO)-$(TARGET)-build.log
LOG_LEVEL ?= 0

# initialize suffix
SUFFIX:=$(TARGET)

# setting linux kernel version suffix
ifneq ($(KERNEL_VERSION),default)
SUFFIX:=$(SUFFIX)-linux-$(KERNEL_VERSION)
endif

# setting realtime suffix
ifneq ($(RT_TYPE),none)
SUFFIX:=$(SUFFIX)-$(RT_TYPE)
endif

# setting drawing backend suffix
ifneq ($(DRAWING_BACKEND),none)
SUFFIX:=$(SUFFIX)-$(DRAWING_BACKEND)
endif

# setting gui toolkit suffix
ifneq ($(GUI_TOOLKIT),none)
SUFFIX:=$(SUFFIX)-$(GUI_TOOLKIT)
endif

# basic directories
CUR_DIR:=${CURDIR}
# workspace directories
WK_DIR:=$(CUR_DIR)/bootstrap.$(DISTRO)-$(SUFFIX)
FEEDS_DIR:=$(WK_DIR)/package/feeds
CUSTOMIZE_DIR:=$(WK_DIR)/customize
####################### 
POOL_DIR:=$(CUR_DIR)/pool
DISTRO_DIR:=$(CUR_DIR)/distro/$(DISTRO)
DL_DIR:=$(BASE_DIR)/dl
SCRIPTS_DIR:=$(CUR_DIR)/scripts
CONF_DIR:=$(DISTRO_DIR)/configs
# set packages symbol links list directory
ifeq ($(BUILD_TYPE),testing)
PKGLIST_DIR:=$(DISTRO_DIR)/testing
#BOOTSTRAP_DIR:=$(DISTRO_DIR)/bootstrap
BOOTSTRAP_DIR:=$(POOL_DIR)/bootstrap
PKGS_DIR:=$(POOL_DIR)/feeds/packages
XORG_DIR:=$(POOL_DIR)/feeds/xorg
RT_PKGS_DIR:=$(POOL_DIR)/feeds/realtime
endif
ifeq ($(BUILD_TYPE),release)
PKGLIST_DIR:=$(DISTRO_DIR)/release/$(RELEASE_VERSION)
BOOTSTRAP_DIR:=$(POOL_DIR)/bootstrap
PKGS_DIR:=$(POOL_DIR)/feeds/packages
XORG_DIR:=$(POOL_DIR)/feeds/xorg
RT_PKGS_DIR:=$(POOL_DIR)/feeds/realtime
endif
# set realtime symbol links directory
RT_DIR:=$(POOL_DIR)/realtime/$(RT_TYPE)

default: $(DISTRO)

all: $(DISTRO)-all

$(DISTRO)-all: $(DISTRO)-distclean $(DISTRO)

build-info:
	echo "DISTRO=$(DISTRO)"
	echo "BUILD_TYPE=$(BUILD_TYPE)"
	echo "RELEASE_VERSION=$(RELEASE_VERSION)"
	echo "TARGET=$(TARGET)"
	echo "DEVICE_TYPE=$(DEVICE_TYPE)"
	echo "ARCH=$(ARCH)"
	echo "KERNEL_VERSION=$(KERNEL_VERSION)"
	echo "RT_TYPE=$(RT_TYPE)"
	echo "DRAWING_BACKEND=$(DRAWING_BACKEND)"
	echo "GUI_TOOLKIT=$(GUI_TOOLKIT)"


prepare-workspace:
	mkdir -p $(WK_DIR)
	cp -af bootstrap/* $(WK_DIR)/
	# feeds directory
	mkdir -p $(FEEDS_DIR)
	# customize directory
	mkdir -p $(CUSTOMIZE_DIR)
	mkdir -p $(CUSTOMIZE_DIR)/backup
	mkdir -p $(CUSTOMIZE_DIR)/target
	mkdir -p $(CUSTOMIZE_DIR)/tools

prepare-bootstrap:
	[ ! -f $(PKGLIST_DIR)/bootstrap-list.txt ] || $(SCRIPTS_DIR)/symbol-link.sh $(BOOTSTRAP_DIR) $(WK_DIR) $(PKGLIST_DIR)/bootstrap-list.txt
	[ ! -f $(PKGLIST_DIR)/target-$(TARGET)-list.txt ] || $(SCRIPTS_DIR)/copy-list.sh $(BOOTSTRAP_DIR) $(WK_DIR) $(PKGLIST_DIR)/target-$(TARGET)-list.txt

clean-bootstrap-links:
	[ ! -f $(PKGLIST_DIR)/bootstrap-list.txt ] || $(SCRIPTS_DIR)/clean-link.sh $(WK_DIR) $(PKGLIST_DIR)/bootstrap-list.txt

prepare-customize:
	[ ! -f $(PKGLIST_DIR)/customize-rootfs-$(SUFFIX).sh ] || cp -f $(PKGLIST_DIR)/customize-rootfs-$(SUFFIX).sh $(CUSTOMIZE_DIR)/tools/customize-rootfs.sh
	[ ! -d $(PKGLIST_DIR)/rootfs-$(TARGET) ] || ( rm -rf $(CUSTOMIZE_DIR)/target/rootfs ; cp -rf $(PKGLIST_DIR)/rootfs-$(TARGET) $(CUSTOMIZE_DIR)/target/rootfs )

#ifeq ($(BUILD_TYPE),testing)
#prepare-packages:
#	cp -f $(CONF_DIR)/feeds.conf $(WK_DIR)/feeds.conf
#	cd $(WK_DIR) && ./scripts/feeds update -a
#	cd $(WK_DIR) && ./scripts/feeds install -a
#	sleep 3

#clean-packages-links:
#	echo "do nothing for clean testing packages links"
#endif

#ifeq ($(BUILD_TYPE),release)
prepare-packages:
	[ ! -f $(PKGLIST_DIR)/packages-list.txt ] || $(SCRIPTS_DIR)/symbol-link.sh $(PKGS_DIR) $(FEEDS_DIR)/packages $(PKGLIST_DIR)/packages-list.txt
	[ ! -f $(PKGLIST_DIR)/xorg-list.txt ] || $(SCRIPTS_DIR)/symbol-link.sh $(XORG_DIR) $(FEEDS_DIR)/xorg $(PKGLIST_DIR)/xorg-list.txt

clean-packages-links:
	[ ! -f $(PKGLIST_DIR)/packages-list.txt ] || $(SCRIPTS_DIR)/clean-link.sh $(FEEDS_DIR)/packages $(PKGLIST_DIR)/packages-list.txt
	[ ! -f $(PKGLIST_DIR)/xorg-list.txt ] || $(SCRIPTS_DIR)/clean-link.sh $(FEEDS_DIR)/xorg $(PKGLIST_DIR)/xorg-list.txt
#endif

prepare-download:
	[ ! -e $(DL_DIR) ] || mkdir -p $(DL_DIR)
	rm -rf $(WK_DIR)/dl
	ln -s $(DL_DIR) $(WK_DIR)/dl

prepare-realtime: prepare-bootstrap
	[ ! -f $(PKGLIST_DIR)/realtime-packages-list.txt ] || $(SCRIPTS_DIR)/symbol-link.sh $(RT_PKGS_DIR) $(FEEDS_DIR)/realtime $(PKGLIST_DIR)/realtime-packages-list.txt
	find $(WK_DIR)/target/linux/$(TARGET) -iname config-* |xargs rm -f
	[ ! -f $(PKGLIST_DIR)/realtime-target-$(TARGET)-list.txt ] || $(SCRIPTS_DIR)/symbol-link.sh $(RT_DIR) $(WK_DIR) $(PKGLIST_DIR)/realtime-target-$(TARGET)-list.txt

clean-realtime-links:
	[ ! -f $(PKGLIST_DIR)/realtime-packages-list.txt ] || $(SCRIPTS_DIR)/clean-link.sh $(FEEDS_DIR)/realtime $(PKGLIST_DIR)/realtime-packages-list.txt


prepare-special-kernel:
	[ ! -f $(PKGLIST_DIR)/linux-$(KERNEL_VERSION)-$(TARGET)-list.txt ] || $(SCRIPTS_DIR)/copy-list.sh $(BOOTSTRAP_DIR) $(WK_DIR) $(PKGLIST_DIR)/linux-$(KERNEL_VERSION)-$(TARGET)-list.txt


prepare-basic-structure: prepare-workspace prepare-bootstrap prepare-customize
# prepare realtime config
ifneq ($(RT_TYPE),none)
prepare-basic-structure: prepare-realtime
endif
# prepare special kerner version config
ifneq ($(KERNEL_VERSION),default)
prepare-basic-structure: prepare-special-kernel
endif
# finally we config packages and download
prepare-basic-structure: prepare-packages prepare-download


ifneq ($(RT_TYPE),none)
clean-symbol-links: clean-realtime-links
endif
clean-symbol-links: clean-packages-links clean-bootstrap-links


prepare-build: clean-symbol-links prepare-basic-structure
	[ ! -f $(PKGLIST_DIR)/customize-$(SUFFIX).sh ] || $(PKGLIST_DIR)/customize-$(SUFFIX).sh $(POOL_DIR) $(WK_DIR)
	echo "prepare-build OK!"

clean-build:
	echo "start to clean build!!!"
	rm -rf $(WK_DIR)
	echo "clean-build is finished!"

quick-build:
	echo "start to quick build..."
	cd $(WK_DIR) && make ARCH=$(ARCH) oldconfig
	cd $(WK_DIR) && make V=$(LOG_LEVEL) 2>&1 | tee $(LOG_FILE)
	echo "quick-build is finished!"

quick-clean:
	echo "start to quick clean!!!"
	cd $(WK_DIR) && make clean
	echo "quick-clean is finished!"

generate-config:
	rm -rf $(WK_DIR)/tmp
	cp $(CONF_DIR)/defconfig-$(SUFFIX) $(WK_DIR)/.config
	cd $(WK_DIR) && make ARCH=$(ARCH) oldconfig

$(DISTRO): build-info prepare-build generate-config
	cd $(WK_DIR) && make V=$(LOG_LEVEL) 2>&1 | tee $(LOG_FILE)

$(DISTRO)-clean:
	cd $(WK_DIR) && make clean

$(DISTRO)-distclean:
	rm -rf $(WK_DIR)

.PHONY: all dummy
.PHONY: $(DISTRO) $(DISTRO)-all $(DISTRO)-clean $(DISTRO)-distclean
.PHONY: clean-bootstrap-links clean-packages-links clean-realtime-links
.PHONY: clean-realtime-links
.PHONY: clean-symbol-links
.PHONY: prepare-workspace prepare-bootstrap prepare-packages prepare-download
.PHONY: prepare-realtime
.PHONY: prepare-special-kernel
.PHONY: prepare-basic-structure
.PHONY: generate-config
.PHONY: build-info prepare-build clean-build
.PHONY: quick-build quick-clean
