#
# Copyright (C) 2014-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libwebsockets-light
PKG_VERSION:=2.4.1
PKG_RELEASE:=2

PKG_SOURCE:=libwebsockets-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=libwebsockets-$(PKG_VERSION)
PKG_SOURCE_URL:=https://codeload.github.com/warmcat/libwebsockets/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=29414be4f79f6abc0e6aadccd09a4da0f0c431e3b5691f496acd081ae6a8240c


PKG_SOURCE_VERSION:=v$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_SOURCE_SUBDIR)

PKG_LICENSE:=LGPL-2.1+exception
PKG_LICENSE_FILES:=LICENSE

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += -DLWS_IPV6=$(if $(CONFIG_IPV6),ON,OFF)
CMAKE_OPTIONS += -DLWS_WITHOUT_TESTAPPS=ON

# other options worth noting
# CMAKE_OPTIONS += -DLWS_WITHOUT_SERVER=ON
CMAKE_OPTIONS += -DLWS_WITHOUT_CLIENT=ON
# CMAKE_OPTIONS += -DLWS_WITHOUT_EXTENSIONS=ON
CMAKE_OPTIONS += -DLWS_WITHOUT_DAEMONIZE=ON
CMAKE_OPTIONS += -DLWS_WITHOUT_DEBUG=ON
CMAKE_OPTIONS += -DLWS_WITH_STATIC=OFF

CMAKE_OPTIONS += -DLWS_OPENSSL_SUPPORT=OFF
CMAKE_OPTIONS += -DLWS_WITH_SSL=OFF
CMAKE_OPTIONS += -DLWS_WITH_LIBUV=OFF
CMAKE_OPTIONS += -DLWS_WITH_PLUGINS=OFF
CMAKE_OPTIONS += -DLWS_WITH_SERVER_STATUS=ON
CMAKE_OPTIONS += -DLWS_WITH_ACCESS_LOG=ON
CMAKE_OPTIONS += -DLWS_WITH_CGI=OFF

define Package/libwebsockets-light
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=libwebsockets (light)
	DEPENDS:=+zlib +libcap
	URL:=https://libwebsockets.org
	MAINTAINER:=Hua Shao <nossiac@163.com>
endef


define Package/libwebsockets-light/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libwebsockets.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libwebsockets-light))

