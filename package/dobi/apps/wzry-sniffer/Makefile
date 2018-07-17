#
# Copyright (C) Hua Shao <nossiac@163.com>
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wzry-sniffer
PKG_RELEASE:=1


include $(INCLUDE_DIR)/package.mk

define Package/wzry-sniffer
  CATEGORY:=Dobi Properties
  SUBMENU:=Applications
  SECTION:=utils
  DEPENDS:=
  TITLE:=A sample program to sniffer wzry packets
endef

define Package/wzry-sniffer/description
 A sample program to sniffer wzry packets
endef

define Package/wzry-sniffer/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wzry $(1)/usr/bin/
endef

$(eval $(call BuildPackage,wzry-sniffer))
