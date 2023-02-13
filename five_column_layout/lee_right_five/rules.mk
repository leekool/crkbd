MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes

# pimoroni uncommented on the right side
POINTING_DEVICE_DRIVER = pimoroni_trackball

PIMORONI_TRACKBALL_ENABLE = yes
ifeq ($(strip $(PIMORONI_TRACKBALL_ENABLE)), yes)
	POINTING_DEVICE_ENABLE = yes
    SRC += drivers/sensors/pimoroni_trackball.c
    QUANTUM_LIB_SRC += i2c_master.c
endif

