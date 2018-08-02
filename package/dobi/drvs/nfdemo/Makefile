# All rights reserved.
#
# Hua Shao (nossiac@163.com)

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=nfdemo
PKG_VERSION:=0.1
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_KCONFIG:=

include $(INCLUDE_DIR)/package.mk

TAR_CMD=$(HOST_TAR) -C $(1)/ $(TAR_OPTIONS)

define KernelPackage/nfdemo
  CATEGORY:=Dobi Properties
  SUBMENU:=Drivers
  TITLE:=Demo for netfilter extension
  FILES:=$(PKG_BUILD_DIR)/nfdemo.ko
endef

define KernelPackage/nfdemo/config
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" V=1 \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		modules
endef


$(eval $(call KernelPackage,nfdemo))
