#
# Copyright (C) Hua Shao <nossiac.com>
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mysky
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mysky
  CATEGORY:=Dobi Properties
  SUBMENU:=Applications
  TITLE:=mysky tool
  DEPENDS:=+libjpeg +libv4l +curl
  MENU:=1
endef

define Package/mysky/description
  mysky tool
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(call Build/Prepare/Default)
endef

define Build/InstallDev
endef


define Package/mysky/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mysky $(1)/usr/bin
endef


$(eval $(call BuildPackage,mysky))

