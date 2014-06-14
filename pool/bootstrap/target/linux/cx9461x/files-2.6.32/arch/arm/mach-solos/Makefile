#
# Makefile for the linux kernel.
#
obj-y			:= arch.o
obj-m			:=
obj-n			:=
obj-			:=

# Build the correct initialisation code for the board.
obj-$(CONFIG_MACH_SOLOS_AEP) += board_aep.o
obj-$(CONFIG_MACH_SOLOS_GALLUSBU) += board_gabu.o

# If the system was built with crypto, provide H/W support
#obj-$(CONFIG_CRYPTO) += solos_crypto.o
obj-$(CONFIG_SOLOS_SPI) += solos_spi.o

# GPIO library routines.

ifdef CONFIG_USB_EHCI_HCD
obj-y	 += solosw_gpio.o solosw_gpio_if.o
endif


