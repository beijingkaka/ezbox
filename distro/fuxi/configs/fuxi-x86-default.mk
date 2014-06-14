#
# Sample default.mk for ezbox quick start
#

# DISTRO set the ezbox distro to be build
# support [kuafu|huangdi|jingwei|qiaochui|lilou|fuxi]
DISTRO := fuxi

# BUILD_TYPE set the ezbox distro release type
# support [testing|release] 
# testing : for development
# release : for formal announcement
BUILD_TYPE := testing

# RELEASE_VERSION set distro release version string
# it takes no effect on building if BUILD_TYPE is testing.
RELEASE_VERSION := 0.1

# TARGET set the ezbox target running board
# support [x86|ar71xx|brcm47xx|s3c24xx]
TARGET := x86

# DEVICE_TYPE set the openwrt device type
# Should always be ezbox
DEVICE_TYPE := ezbox

# ARCH set the CPU arch, please see README to set correct TARGET/ARCH
# support [i386|mips|mipsel|arm]
ARCH := i386

# KERNEL_VERSION set Linux kernel version string
# support [default|2.6.32|2.6.37|...]
KERNEL_VERSION := default

# RT_TYPE set the real-time Linux type
# support [none|rtai|xenomai]
RT_TYPE := none

# DRAWING_BACKEND set the Feature_drawing-backend
# support [none|nanox|kdrive]
# none : no graphic user interface support
# nanox : use nxlib/nano-X archetecture
# kdrive : use libX11/kdrive-fbdev archetecture
DRAWING_BACKEND := none

# GUI_TOOLKIT set the Feature_gui-toolkit
# support [none|fltk2]
# none : no graphic user interface toolkit support
# fltk2 : use fltk2/(X window system) archetecture
GUI_TOOLKIT := none

# LOG_FILE set default build log file name
# default name is at $(BASE_DIR)/$(shell date --iso=seconds)-$(DISTRO)-$(TARGET)-build.log
# LOG_FILE :=

# LOG_LEVEL set openwrt build log verbose level
# Set it to 99 for debugging
LOG_LEVEL := 99

