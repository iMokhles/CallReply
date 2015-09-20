//
//  CallReplyNotificationView.m
//  CallReply
//
//  Created by iMokhles on 26.08.2015.
//  Copyright (c) 2015 iMokhles. All rights reserved.
//

#import "CallReplyNotificationView.h"
#import <substrate.h>

#define NOTIFICATION_VIEW_FRAME_HEIGHT          125

#define LABEL_TITLE_FONT_SIZE                   14.0f
#define LABEL_MESSAGE_FONT_SIZE                 13.0f

#define IMAGE_VIEW_ICON_CORNER_RADIUS           3.0f
#define IMAGE_VIEW_ICON_FRAME                   CGRectMake(15.0f, 25, 50, 50)
#define LABEL_TITLE_FRAME                       CGRectMake(75, 20, [[UIScreen mainScreen] bounds].size.width - 170, 26.0f)
#define LABEL_MESSAGE_FRAME_HEIGHT              35.0f
#define LABEL_MESSAGE_FRAME                     CGRectMake(75, 42, [[UIScreen mainScreen] bounds].size.width - 150.0f, LABEL_MESSAGE_FRAME_HEIGHT)

#define NOTIFICATION_VIEW_SHOWING_DURATION                  7.0f    /// second(s)
#define NOTIFICATION_VIEW_SHOWING_ANIMATION_TIME            0.5f    /// second(s)

#define kDropdownPadding 10
#define kDropdownButtonWidth 150
#define kDropdownButtonHeight 30

/// ---------------------

@interface FBWindow : UIWindow
-(id)recursiveDescription;
@end

@interface SBWindow : FBWindow
@end

@interface SBUIController : NSObject
- (SBWindow *)window;
@end

@interface SpringBoard : UIApplication {
    SBUIController *_uiController;
}
@end

/// ---------------------

@interface CallReplyNotificationView ()
@property (nonatomic, strong) UIButton *topButton;
@property (nonatomic, strong) UIButton *bottomButton;
@property (nonatomic, copy) block internalBlock;
@end
@implementation CallReplyNotificationView

/// -------------------------------------------------------------------------------------------
#pragma mark - INIT
/// -------------------------------------------------------------------------------------------
+ (instancetype)sharedInstance
{
    static id _sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedInstance = [[[self class] alloc] init];
    });
    
    return _sharedInstance;
}

- (instancetype)init
{
    self = [super initWithFrame:CGRectMake(0.0f, 0.0f, [[UIScreen mainScreen] bounds].size.width, NOTIFICATION_VIEW_FRAME_HEIGHT)];
    if (self) {
        
        /// Enable orientation tracking
        if (![[UIDevice currentDevice] isGeneratingDeviceOrientationNotifications]) {
            [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        }
        
        /// Add Orientation notification
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationStatusDidChange:) name:UIDeviceOrientationDidChangeNotification object:nil];
        
        _topButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _topButton.titleLabel.font = [UIFont fontWithName:@"HelveticaNeue-Medium" size:13];
        [_topButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        _topButton.adjustsImageWhenHighlighted = YES;
        _topButton.backgroundColor = [UIColor clearColor];
        
        [_topButton.layer setCornerRadius:10];
        [_topButton.layer setBorderWidth:1];
        [_topButton.layer setBorderColor:[[UIColor grayColor] CGColor]];
        [_topButton.layer setMasksToBounds:YES];
        
        _bottomButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _bottomButton.titleLabel.font = [UIFont fontWithName:@"HelveticaNeue-Medium" size:13];
        [_bottomButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [_bottomButton.layer setCornerRadius:10];
        [_bottomButton.layer setBorderWidth:1];
        [_bottomButton.layer setBorderColor:[[UIColor grayColor] CGColor]];
        [_bottomButton.layer setMasksToBounds:YES];
        
        /// Set up UI
        [self setUpUI];
    }
    
    return self;
}

/// -------------------------------------------------------------------------------------------
#pragma mark - ACTIONS
/// -------------------------------------------------------------------------------------------
- (void)setUpUI
{
    if ([[[UIDevice currentDevice] systemVersion] doubleValue] >= 7.0) {
        self.barTintColor = nil;
        self.translucent = YES;
        self.barStyle = UIBarStyleBlack;
    }
    else {
        [self setTintColor:[UIColor colorWithRed:5 green:31 blue:75 alpha:1]];
    }
    
    self.layer.zPosition = MAXFLOAT;
    self.backgroundColor = [UIColor clearColor];
    self.multipleTouchEnabled = NO;
    self.exclusiveTouch = YES;
    
    self.frame = CGRectMake(0.0f, 0.0f, [[UIScreen mainScreen] bounds].size.width, NOTIFICATION_VIEW_FRAME_HEIGHT);
    
    /// Icon
    if (!_imgIcon) {
        _imgIcon = [[UIImageView alloc] init];
    }
    _imgIcon.frame = IMAGE_VIEW_ICON_FRAME;
    [_imgIcon setContentMode:UIViewContentModeScaleAspectFill];
    [_imgIcon.layer setCornerRadius:IMAGE_VIEW_ICON_CORNER_RADIUS];
    [_imgIcon setClipsToBounds:YES];
    if (![_imgIcon superview]) {
        [self addSubview:_imgIcon];
    }
    
    /// Title
    if (!_lblTitle) {
        _lblTitle = [[UILabel alloc] init];
    }
    _lblTitle.frame = LABEL_TITLE_FRAME;
    [_lblTitle setTextColor:[UIColor whiteColor]];
    [_lblTitle setFont:[UIFont fontWithName:@"HelveticaNeue-Bold" size:LABEL_TITLE_FONT_SIZE]];
    [_lblTitle setNumberOfLines:1];
    if (![_lblTitle superview]) {
        [self addSubview:_lblTitle];
    }
    
    /// Message
    if (!_lblMessage) {
        _lblMessage = [[UILabel alloc] init];
    }
    _lblMessage.frame = LABEL_MESSAGE_FRAME;
    [_lblMessage setTextColor:[UIColor whiteColor]];
    [_lblMessage setFont:[UIFont fontWithName:@"HelveticaNeue" size:LABEL_MESSAGE_FONT_SIZE]];
    [_lblMessage setNumberOfLines:2];
    _lblMessage.lineBreakMode = NSLineBreakByTruncatingTail;
    if (![_lblMessage superview]) {
        [self addSubview:_lblMessage];
    }
    
    [self fixLabelMessageSize];
    
    // New View
    UIView *newView = [[UIView alloc] initWithFrame:CGRectMake(0, 80, 300, 30)];
//    [newView setBackgroundColor:[UIColor redColor]];
    [self addSubview:newView];
    newView.center = CGPointMake(CGRectGetMidX(self.bounds), newView.center.y);
    [newView addSubview:_bottomButton];
    [newView addSubview:_topButton];
    //
    _topButton.frame = CGRectMake(0, 0, kDropdownButtonWidth, kDropdownButtonHeight);
    [_topButton addTarget:self action:@selector(topButtonTapped) forControlEvents:UIControlEventTouchUpInside];
    
    
    _bottomButton.frame = CGRectMake(_topButton.frame.origin.x + _topButton.frame.size.width + 10, 0, kDropdownButtonWidth, kDropdownButtonHeight);
    [_bottomButton addTarget:self action:@selector(bottomButtonTapped) forControlEvents:UIControlEventTouchUpInside];
    
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(notificationViewDidTap:)];
    [self addGestureRecognizer:tapGesture];
    
    _internalBlock = ^(CallReplyNotificationEvent event) {
        
    };
    
}

- (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons isAutoHide:(BOOL)isAutoHide onTouch:(void (^)())onTouch
{
    /// Invalidate _timerHideAuto
    if (_timerHideAuto) {
        [_timerHideAuto invalidate];
        _timerHideAuto = nil;
    }
    
    /// onTouch
    _onTouch = onTouch;
    
    /// Image
    if (image) {
        [_imgIcon setImage:image];
    }
    else {
        [_imgIcon setImage:nil];
    }
    
    /// Title
    if (title) {
        [_lblTitle setText:title];
    }
    else {
        [_lblTitle setText:@""];
    }
    
    /// Message
    if (message) {
        [_lblMessage setText:message];
    }
    else {
        [_lblMessage setText:@""];
    }
    [self fixLabelMessageSize];
    
    // Buttons
    
    if ([buttons objectAtIndex:0] != nil) {
        [_topButton setTitle:[buttons objectAtIndex:0] forState:UIControlStateNormal];
        
    }
    if ([buttons objectAtIndex:1] != nil) {
        [_bottomButton setTitle:[buttons objectAtIndex:1] forState:UIControlStateNormal];
        
    }

    /// Prepare frame
    CGRect frame = self.frame;
    frame.origin.y = -frame.size.height;
    self.frame = frame;
    
    /// Add to window
    SpringBoard *sb = (SpringBoard *)[UIApplication sharedApplication].delegate;
    SBUIController *_uiController = MSHookIvar<SBUIController *>(sb, "_uiController");
    _uiController.window.windowLevel = UIWindowLevelStatusBar;
    [_uiController.window addSubview:self];
    
    /// Showing animation
    [UIView animateWithDuration:NOTIFICATION_VIEW_SHOWING_ANIMATION_TIME
                          delay:0.0f
                        options:UIViewAnimationOptionCurveEaseOut
                     animations:^{
                         
                         CGRect frame = self.frame;
                         frame.origin.y += frame.size.height;
                         self.frame = frame;
                         
                     } completion:^(BOOL finished) {
                         
                     }];
    
    // Schedule to hide
    if (isAutoHide) {
        _timerHideAuto = [NSTimer scheduledTimerWithTimeInterval:NOTIFICATION_VIEW_SHOWING_DURATION
                                                          target:self
                                                        selector:@selector(hideNotificationView)
                                                        userInfo:nil
                                                         repeats:NO];
    }
}
- (void)hideNotificationView
{
    [self hideNotificationViewOnComplete:nil];
}
- (void)hideNotificationViewOnComplete:(void (^)())onComplete
{
    [UIView animateWithDuration:NOTIFICATION_VIEW_SHOWING_ANIMATION_TIME
                          delay:0.0f
                        options:UIViewAnimationOptionCurveEaseOut
                     animations:^{
                         
                         CGRect frame = self.frame;
                         frame.origin.y -= frame.size.height;
                         self.frame = frame;
                         
                     } completion:^(BOOL finished) {
                         
                         [self removeFromSuperview];
                         SpringBoard *sb = (SpringBoard *)[UIApplication sharedApplication].delegate;
                        SBUIController *_uiController = MSHookIvar<SBUIController *>(sb, "_uiController");
                        _uiController.window.windowLevel = UIWindowLevelNormal;
                         // [UIApplication sharedApplication].delegate.window.windowLevel = UIWindowLevelNormal;
                         
                         // Invalidate _timerAutoClose
                         if (_timerHideAuto) {
                             [_timerHideAuto invalidate];
                             _timerHideAuto = nil;
                         }
                         
                         if (onComplete) {
                             onComplete();
                         }
                     }];
}
- (void)notificationViewDidTap:(UIGestureRecognizer *)gesture
{
    if (_onTouch) {
        _onTouch();
    }
}

/// -------------------------------------------------------------------------------------------
#pragma mark - HELPER
/// -------------------------------------------------------------------------------------------
- (void)fixLabelMessageSize
{
    CGSize size = [_lblMessage sizeThatFits:CGSizeMake([[UIScreen mainScreen] bounds].size.width - 45.0f, MAXFLOAT)];
    CGRect frame = _lblMessage.frame;
    frame.size.height = (size.height > LABEL_MESSAGE_FRAME_HEIGHT ? LABEL_MESSAGE_FRAME_HEIGHT : size.height);
    _lblMessage.frame = frame;
}

/// -------------------------------------------------------------------------------------------
#pragma mark - ORIENTATION NOTIFICATION
/// -------------------------------------------------------------------------------------------
- (void)orientationStatusDidChange:(NSNotification *)notification
{
    [self setUpUI];
}

/// -------------------------------------------------------------------------------------------
#pragma mark - UTILITY FUNCS
/// -------------------------------------------------------------------------------------------
+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons
{
    [CallReplyNotificationView showNotificationViewWithImage:image title:title message:message buttons:buttons isAutoHide:YES onTouch:nil];
}
+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons isAutoHide:(BOOL)isAutoHide
{
    [CallReplyNotificationView showNotificationViewWithImage:image title:title message:message buttons:buttons isAutoHide:isAutoHide onTouch:nil];
}
+ (void)showNotificationViewWithImage:(UIImage *)image title:(NSString *)title message:(NSString *)message buttons:(NSArray *)buttons isAutoHide:(BOOL)isAutoHide onTouch:(void (^)())onTouch
{
    [[CallReplyNotificationView sharedInstance] showNotificationViewWithImage:image title:title message:message buttons:buttons isAutoHide: isAutoHide onTouch:onTouch];
}

+ (void)hideNotificationView
{
    [CallReplyNotificationView hideNotificationViewOnComplete:nil];
}
+ (void)hideNotificationViewOnComplete:(void (^)())onComplete
{
    [[CallReplyNotificationView sharedInstance] hideNotificationViewOnComplete:onComplete];
}

-(void)topButtonTapped {
    
    
    if (_internalBlock) {
        
        _internalBlock(CallReplyNotificationEventTopButton);
    }
}

-(void)bottomButtonTapped {
    
    
        if (_internalBlock) {
    
            _internalBlock(CallReplyNotificationEventBottomButton);
        }
}

+ (void)listenEventsWithBlock:(block)block {
    
    [CallReplyNotificationView sharedInstance].internalBlock = ^(CallReplyNotificationEvent event) {
        
        if (block) {
            
            block(event);
        }
    };
}

@end