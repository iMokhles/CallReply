//
//  CallReplyListController.m
//  CallReply
//
//  Created by iMokhles on 26.08.2015.
//  Copyright (c) 2015 iMokhles. All rights reserved.
//

#import "CallReplyHelper.h"

@implementation CallReplyHelper

// Preferences
+ (NSString *)preferencesPath {
	return @"/User/Library/Preferences/com.imokhles.callreply.plist";
}

+ (CFStringRef)preferencesChanged {
	return (__bridge CFStringRef)@"com.imokhles.callreply.preferences-changed";
}

// UIWindow to present your elements
// u can show/hide it using ( setHidden: NO/YES )
+ (UIWindow *)mainCallReplyWindow {
	return nil;
}

+ (UIViewController *)mainCallReplyViewController {
	return nil;
}

// Checking App Version
+ (BOOL)isAppVersionGreaterThanOrEqualTo:(NSString *)appversion {
	return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] compare:appversion options:NSNumericSearch] != NSOrderedAscending;
}
+ (BOOL)isAppVersionLessThanOrEqualTo:(NSString *)appversion {
	return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] compare:appversion options:NSNumericSearch] != NSOrderedDescending;
}

// Checking OS Version
+ (BOOL)isIOS83_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 8.3;
}
+ (BOOL)isIOS80_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0;
}
+ (BOOL)isIOS70_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0;
}
+ (BOOL)isIOS60_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0;
}
+ (BOOL)isIOS50_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 5.0;
}
+ (BOOL)isIOS40_OrGreater {
	return [[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0;
}

// Checking Device Type
+ (BOOL)isIPhone6_Plus {
	return [self isIPhone] && [self screenMaxLength] == 736.0;
}
+ (BOOL)isIPhone6 {
	return [self isIPhone] && [self screenMaxLength] == 667.0;
}
+ (BOOL)isIPhone5 {
	return [self isIPhone] && [self screenMaxLength] == 568.0;
}
+ (BOOL)isIPhone4_OrLess {
	return [self isIPhone] && [self screenMaxLength] < 568.0;
}

// Checking Device Interface
+ (BOOL)isIPad {
	return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
}
+ (BOOL)isIPhone {
	return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
}

// Checking Device Retina
+ (BOOL)isRetina {
	if ([self isIOS80_OrGreater]) {
        return [UIScreen mainScreen].nativeScale>=2;
    }
	return [[UIScreen mainScreen] scale] >= 2.0;
}

// Checking UIScreen sizes
+ (CGFloat)screenWidth {
	return [[UIScreen mainScreen] bounds].size.width;
}
+ (CGFloat)screenHeight {
	return [[UIScreen mainScreen] bounds].size.height;
}

+ (CGFloat)screenMaxLength {
    return MAX([self screenWidth], [self screenHeight]);
}

+ (CGFloat)screenMinLength {
    return MIN([self screenWidth], [self screenHeight]);
}


// testing
// Custom Methods
+ (void)createLocalNotificationFromCall:(TUCall *)call {

    NSString *fullMessage = [NSString stringWithFormat:@"%@: %@", call.displayName, @"incomingCall"];
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    notification.fireDate = [NSDate dateWithTimeIntervalSinceNow:0];
    notification.alertBody = fullMessage;
    notification.timeZone = [NSTimeZone defaultTimeZone];
    notification.soundName = UILocalNotificationDefaultSoundName;
    UIApplication *currentApp = [UIApplication sharedApplication];
    [currentApp presentLocalNotificationNow:notification];
}
@end
