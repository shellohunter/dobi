#
# Copyright (C) Hua Shao <nossiac@163.com>
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=fapi
PKG_VERSION:=1
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

PKG_MAINTAINER:=Hua Shao <nossiac@163.com>

include $(INCLUDE_DIR)/package.mk


define KernelPackage/fapi
  CATEGORY:=Dobi Properties
  SUBMENU:=Drivers
  TITLE:=To study the kernel's file operations
  FILES:=$(PKG_BUILD_DIR)/src/fapi.ko
  AUTOLOAD:=$(call AutoLoad,80,fapi)
  MENU:=1
endef

define Package/fapi/description
  study kernel's file operations 
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

$(eval $(call KernelPackage,fapi))
