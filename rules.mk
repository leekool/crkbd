MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
CAPS_WORD_ENABLE = yes

POINTING_DEVICE_DRIVER = pimoroni_trackball

PIMORONI_TRACKBALL_ENABLE = yes
ifeq ($(strip $(PIMORONI_TRACKBALL_ENABLE)), yes)
	POINTING_DEVICE_ENABLE = yes
    SRC += drivers/sensors/pimoroni_trackball.c
    QUANTUM_LIB_SRC += i2c_master.c
endif

