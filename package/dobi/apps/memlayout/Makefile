#
# hua.shao@mediatek.com
#
# MTK Property Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=memlayout
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/memlayout
  CATEGORY:=Dobi Properties
  SECTION:=Applications
  SUBMENU:=Applications
  DEPENDS:=
  TITLE:=Illustrate application memory layout
endef


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Build/Configure
endef


define Package/memlayout/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/memlayout $(1)/usr/bin
endef


$(eval $(call BuildPackage,memlayout))

