//
//  CallReplyNotificationView.h
//  CallReply
//
//  Created by iMokhles on 26.08.2015.
//  Copyright (c) 2015 iMokhles. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <substrate.h>

typedef NS_ENUM(NSInteger, CallReplyNotificationEvent) {
    
    CallReplyNotificationEventTopButton,
    CallReplyNotificationEventBottomButton
};

typedef void (^block)(CallReplyNotificationEvent event);

@interface CallReplyNotificationView : UIToolbar
{
    void (^ _onTouch)();
    
    UIImageView *_imgIcon;
    UILabel *_lblTitle;
    UILabel *_lblMessage;
    
    NSTimer *_timerHideAuto;
}

+ (instancetype)sharedInstance;

+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons;
+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons isAutoHide:(BOOL)isAutoHide;
+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons isAutoHide:(BOOL)isAutoHide onTouch:(void (^)())onTouch;

+ (void)hideNotificationView;
+ (void)hideNotificationViewOnComplete:(void (^)())onComplete;

+ (void)listenEventsWithBlock:(block)block;
@end

