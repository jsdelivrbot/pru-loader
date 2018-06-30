PRU_USER_VERSION = 1.0
PRU_USER_SITE = /home/sam/buildroot/buildroot/package/pru_user
PRU_USER_SITE_METHOD = local

 
define PRU_USER_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef
  
$(eval $(generic-package))
