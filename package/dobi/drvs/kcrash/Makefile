#
# Copyright (C) Hua Shao <nossiac@163.com>
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kcrash
PKG_VERSION:=1
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

PKG_MAINTAINER:=Hua Shao <nossiac@163.com>

include $(INCLUDE_DIR)/package.mk


define KernelPackage/kcrash
  CATEGORY:=Dobi Properties
  SUBMENU:=Drivers
  TITLE:=To crash the kernel
  FILES:=$(PKG_BUILD_DIR)/src/kcrash.ko
  AUTOLOAD:=$(call AutoLoad,80,kcrash)
  MENU:=1
endef

define Package/kcrash/description
  Crash the kernel to study
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r ./src $(PKG_BUILD_DIR)/src
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" V=1 \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)/src" \
		modules
endef

$(eval $(call KernelPackage,kcrash))
