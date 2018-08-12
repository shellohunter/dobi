#
# Copyright (C) 2016 Shuanglei Tao <tsl0922@gmail.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ttyd
PKG_VERSION:=master
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/tsl0922/ttyd
PKG_SOURCE_VERSION=master
#PKG_HASH=757a9b5b5dd3de801d7db8fab6035d97ea144b02a51c78bdab28a8e07bcf05d6

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Hua Shao <nossiac@163.com>
CMAKE_INSTALL:=1
HTDOCS = /www
LUA_LIBRARYDIR = /usr/lib/lua
LUCI_LIBRARYDIR = $(LUA_LIBRARYDIR)/luci
PLUGIN = $(PKG_BUILD_DIR)/openwrt/luci-app-terminal

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/ttyd-light
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Command-line tool for sharing terminal over the web
	DEPENDS:=+libwebsockets-light +libjson-c +libpthread
	URL:=https://github.com/tsl0922/ttyd
	SUBMENU:=Terminal
	MAINTAINER:=Shuanglei Tao <tsl0922@gmail.com>
endef

define Package/ttyd-light/description
ttyd-light is a command-line tool for sharing terminal over the web.
endef

define Package/ttyd-luci-plugin
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=LuCI plugin for ttyd
  SUBMENU:=Terminal
  DEPENDS:=+lua +ttyd-light +luci
  VERSION:=$(PKG_VERSION)
endef

define Package/ttyd-luci-plugin/install
	cp -pR $(PLUGIN)/luasrc/* $(1)$(LUCI_LIBRARYDIR)/; \
	if [ -d $(PLUGIN)/luasrc ]; then \
	  $(INSTALL_DIR) $(1)$(LUCI_LIBRARYDIR); \
	  cp -pR $(PLUGIN)/luasrc/* $(1)$(LUCI_LIBRARYDIR)/; \
	else true; fi
	if [ -d $(PLUGIN)/htdocs ]; then \
	  $(INSTALL_DIR) $(1)$(HTDOCS); \
	  cp -pR $(PLUGIN)/htdocs/* $(1)$(HTDOCS)/; \
	else true; fi
	if [ -d $(PLUGIN)/root ]; then \
	  $(INSTALL_DIR) $(1)/; \
	  cp -pR $(PLUGIN)/root/* $(1)/; \
	else true; fi
	if [ -d $(PLUGIN)/src ]; then \
	  $(call Build/Install/Default) \
	  $(CP) ./$(PKG_INSTALL_DIR)/* $(1)/; \
	else true; fi
endef

define Package/ttyd-light/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ttyd $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ttyd-luci-plugin))
$(eval $(call BuildPackage,ttyd-light))
