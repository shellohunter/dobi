#
# Copyright (C) 2010-2015 Hua Shao (nossiac@163.com)
#
#
include $(TOPDIR)/rules.mk

PKG_NAME:=kpfs
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/shellohunter/kpfs
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_VERSION:=575b4273a17e30ba0b1a3eb3a7bc0f7e822f7c95


PKG_LICENCE:=GPLv2
PKG_MAINTAINER:=Hua Shao <nossiac@163.com>

include $(INCLUDE_DIR)/package.mk

define Package/kpfs
  CATEGORY:=Dobi Properties
  SUBMENU:=Applications
  TITLE:=kpfs (linux-fuse filesystem for kuaipan)
  DEPENDS:=@TARGET_ramips +libfuse +kmod-fuse +libcurl +liboauth
  URL:=https://code.google.com/p/kpfs
endef

define Package/kpfs/description
endef


TARGET_CFLAGS += \
        $(foreach c, $(PKG_KCONFIG),$(if $(CONFIG_$c),-DCONFIG_$(c)))

MAKE_FLAGS += \
        CFLAGS="$(TARGET_CFLAGS) -D_FILE_OFFSET_BITS=64"

define Package/kpfs/install
        $(INSTALL_DIR) $(1)/usr/bin
        $(INSTALL_BIN) $(PKG_BUILD_DIR)/hellofs $(1)/usr/bin
endef

$(eval $(call BuildPackage,kpfs))

