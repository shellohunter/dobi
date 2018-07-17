#
# hua.shao@mediatek.com
#
# MTK Property Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mmtest
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_CONFIG_DEPENDS:=

include $(INCLUDE_DIR)/package.mk

define Package/oom-trigger
  CATEGORY:=Dobi Properties
  SECTION:=Applications
  SUBMENU:=Applications
  DEPENDS:=
  TITLE:=Trigger OOM-killer
endef


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Build/Configure
endef


define Package/oom-trigger/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/oom-trigger $(1)/usr/bin
endef


$(eval $(call BuildPackage,oom-trigger))

