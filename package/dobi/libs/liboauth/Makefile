#
# Copyright (C) 2015 Hua Shao (nossiac@163.com)
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=liboauth
PKG_VERSION:=2015-01-12
PKG_RELEASE:=PKG_SOURCE_VERSION

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/x42/liboauth
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_VERSION:=d245224869248f168f9fedc092cdd64e11e3faf9

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_LICENSE:=GPLv2
PKG_KCONFIG:=
PKG_INSTALL:=1
PKG_MAINTAINER:=Hua Shao <nossiac@163.com>

include $(INCLUDE_DIR)/package.mk
#include $(INCLUDE_DIR)/cmake.mk


define Package/liboauth
  CATEGORY:=Dobi Properties
  SUBMENU:=Libraries
  TITLE:=liboauth
  DEPENDS:=
  URL:=http://liboauth.sourceforge.net
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
			LDSHARED="$(TARGET_CC) -shared -Wl -soname, libauth.so" \
			CFLAGS="$(TARGET_CFLAGS) $(FPIC)" \
			aclocal; ./configure \
	);
endef


define Build/InstallDev
	$(INSTALL_DIR) $(1)
endef

define Package/liboauth/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liboauth.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,liboauth))

