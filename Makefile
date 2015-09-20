
THEOS_DEVICE_IP = 192.168.2.109

ARCHS = armv7 armv7s arm64

TARGET = iphone:clang:latest:8.0

THEOS_BUILD_DIR = Packages

include theos/makefiles/common.mk

TWEAK_NAME = CallReply
CallReply_CFLAGS = -fobjc-arc
CallReply_FILES = CallReply.xm CallReplyHelper.m CallReplyNotificationView.mm 
CallReply_FRAMEWORKS = Foundation UIKit CoreGraphics AVFoundation QuartzCore
CallReply_PRIVATE_FRAMEWORKS = FrontBoard SpringBoardUI BulletinBoard AppSupport AssertionServices ChatKit CoreTelephony XPCKit IMCore BackBoardServices GraphicsServices
CallReply_LIBRARIES = substrate applist

include $(THEOS_MAKE_PATH)/tweak.mk

after-install::
	install.exec "killall -9 backboardd"
