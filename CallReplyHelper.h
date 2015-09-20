//
//  CallReplyListController.h
//  CallReply
//
//  Created by iMokhles on 26.08.2015.
//  Copyright (c) 2015 iMokhles. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <substrate.h>

typedef NS_ENUM(NSUInteger, BKSProcessAssertionReason)
{
    kProcessAssertionReasonAudio = 1,
    kProcessAssertionReasonLocation,
    kProcessAssertionReasonExternalAccessory,
    kProcessAssertionReasonFinishTask,
    kProcessAssertionReasonBluetooth,
    kProcessAssertionReasonNetworkAuthentication,
    kProcessAssertionReasonBackgroundUI,
    kProcessAssertionReasonInterAppAudioStreaming,
    kProcessAssertionReasonViewServices
};

typedef NS_ENUM(NSUInteger, ProcessAssertionFlags)
{
    ProcessAssertionFlagNone = 0,
    ProcessAssertionFlagPreventSuspend = 1 << 0,
    ProcessAssertionFlagPreventThrottleDownCPU = 1 << 1,
    ProcessAssertionFlagAllowIdleSleep = 1 << 2,
    ProcessAssertionFlagWantsForegroundResourcePriority = 1 << 3
};

@interface BKSProcessAssertion
- (id)initWithPID:(int)arg1 flags:(unsigned int)arg2 reason:(unsigned int)arg3 name:(id)arg4 withHandler:(id)arg5;
- (id)initWithBundleIdentifier:(id)arg1 flags:(unsigned int)arg2 reason:(unsigned int)arg3 name:(id)arg4 withHandler:(id)arg5;
- (void)invalidate;
@property(readonly, nonatomic) BOOL valid;
@end

@interface FBWindowContextHostManager
- (id)hostViewForRequester:(id)arg1 enableAndOrderFront:(BOOL)arg2;
- (void)resumeContextHosting;
- (id)_hostViewForRequester:(id)arg1 enableAndOrderFront:(BOOL)arg2;
- (id)snapshotViewWithFrame:(CGRect)arg1 excludingContexts:(id)arg2 opaque:(BOOL)arg3;
- (id)visibleContexts;
- (void)orderRequesterFront:(id)arg1;
- (void)enableHostingForRequester:(id)arg1 orderFront:(BOOL)arg2;
- (void)enableHostingForRequester:(id)arg1 priority:(int)arg2;
- (void)disableHostingForRequester:(id)arg1;
- (void)_updateHostViewFrameForRequester:(id)arg1;
- (void)invalidate;
@end

@interface FBSSceneSettings : NSObject <NSCopying, NSMutableCopying>
{
    CGRect _frame;
    CGPoint _contentOffset;
    float _level;
    int _interfaceOrientation;
    BOOL _backgrounded;
    BOOL _occluded;
    BOOL _occludedHasBeenCalculated;
    NSSet *_ignoreOcclusionReasons;
    NSArray *_occlusions;
    //BSSettings *_otherSettings;
    //BSSettings *_transientLocalSettings;
}

+ (BOOL)_isMutable;
+ (id)settings;
@property(readonly, copy, nonatomic) NSArray *occlusions; // @synthesize occlusions=_occlusions;
@property(readonly, nonatomic, getter=isBackgrounded) BOOL backgrounded; // @synthesize backgrounded=_backgrounded;
@property(readonly, nonatomic) int interfaceOrientation; // @synthesize interfaceOrientation=_interfaceOrientation;
@property(readonly, nonatomic) float level; // @synthesize level=_level;
@property(readonly, nonatomic) CGPoint contentOffset; // @synthesize contentOffset=_contentOffset;
@property(readonly, nonatomic) CGRect frame; // @synthesize frame=_frame;
- (id)valueDescriptionForFlag:(int)arg1 object:(id)arg2 ofSetting:(unsigned int)arg3;
- (id)keyDescriptionForSetting:(unsigned int)arg1;
- (id)description;
- (BOOL)isEqual:(id)arg1;
- (unsigned int)hash;
- (id)_descriptionOfSettingsWithMultilinePrefix:(id)arg1;
- (id)transientLocalSettings;
- (BOOL)isIgnoringOcclusions;
- (id)ignoreOcclusionReasons;
- (id)otherSettings;
- (BOOL)isOccluded;
- (CGRect)bounds;
- (void)dealloc;
- (id)init;
- (id)initWithSettings:(id)arg1;

@end

@interface FBSMutableSceneSettings : FBSSceneSettings
{
}

+ (BOOL)_isMutable;
- (id)mutableCopyWithZone:(struct _NSZone *)arg1;
- (id)copyWithZone:(struct _NSZone *)arg1;
@property(copy, nonatomic) NSArray *occlusions;
- (id)transientLocalSettings;
- (id)ignoreOcclusionReasons;
- (id)otherSettings;
@property(nonatomic, getter=isBackgrounded) BOOL backgrounded;
@property(nonatomic) int interfaceOrientation;
@property(nonatomic) float level;
@property(nonatomic) struct CGPoint contentOffset;
@property(nonatomic) struct CGRect frame;

@end

@interface FBScene
-(FBWindowContextHostManager*) contextHostManager;
@property(readonly, retain, nonatomic) FBSMutableSceneSettings *mutableSettings; // @synthesize mutableSettings=_mutableSettings;
- (void)updateSettings:(id)arg1 withTransitionContext:(id)arg2;
- (void)_applyMutableSettings:(id)arg1 withTransitionContext:(id)arg2 completion:(id)arg3;
@end

@interface FBProcessManager : NSObject
- (id)createApplicationProcessForBundleID:(id)arg1 withExecutionContext:(id)arg2;
- (id)createApplicationProcessForBundleID:(id)arg1;
@end

@interface SBApplication
-(FBScene*) mainScene;
- (void)activate;
- (void)processDidLaunch:(id)arg1;
- (void)processWillLaunch:(id)arg1;
- (void)resumeForContentAvailable;
- (void)resumeToQuit;
- (void)_sendDidLaunchNotification:(_Bool)arg1;
- (void)notifyResumeActiveForReason:(long long)arg1;

@property(readonly, nonatomic) int pid;
@end

@interface UIApplication (PVT)
- (BOOL)launchApplicationWithIdentifier:(NSString *)identifier suspended:(BOOL)suspended;  
@end

@interface SpringBoard : UIApplication 
- (BOOL)handleDoubleHeightStatusBarTap:(int)arg1;
@end

@interface SpringBoard (LastApp)
- (void)lastApp_switchToLastApp;
@end

@interface SBApplicationController
+(id) sharedInstance;
- (SBApplication *)applicationWithDisplayIdentifier:(NSString *)identifier;
-(SBApplication*) applicationWithBundleIdentifier:(NSString*)identifier;
-(SBApplication*)applicationWithPid:(int)arg1;
@end

@interface UIConcreteLocalNotification : UILocalNotification
@end

typedef NS_ENUM(NSUInteger, NSNotificationSuspensionBehavior) {
    NSNotificationSuspensionBehaviorDrop = 1,
    // The server will not queue any notifications with this name and object until setSuspended:NO is called.
    NSNotificationSuspensionBehaviorCoalesce = 2,
    // The server will only queue the last notification of the specified name and object; earlier notifications are dropped.  In cover methods for which suspension behavior is not an explicit argument, NSNotificationSuspensionBehaviorCoalesce is the default.
    NSNotificationSuspensionBehaviorHold = 3,
    // The server will hold all matching notifications until the queue has been filled (queue size determined by the server) at which point the server may flush queued notifications.
    NSNotificationSuspensionBehaviorDeliverImmediately = 4
    // The server will deliver notifications matching this registration irrespective of whether setSuspended:YES has been called.  When a notification with this suspension behavior is matched, it has the effect of first flushing
    // any queued notifications.  The effect is somewhat as if setSuspended:NO were first called if the app is suspended, followed by
    // the notification in question being delivered, followed by a transition back to the previous suspended or unsuspended state.
};

@interface NSDistributedNotificationCenter : NSNotificationCenter
@property (assign) BOOL suspended; 
+(id)notificationCenterForType:(id)arg1 ;
+(id)defaultCenter;
-(void)addObserver:(id)arg1 selector:(SEL)arg2 name:(id)arg3 object:(id)arg4 suspensionBehavior:(NSNotificationSuspensionBehavior)arg5 ;
-(id)addObserverForName:(id)arg1 object:(id)arg2 suspensionBehavior:(unsigned long long)arg3 queue:(id)arg4 usingBlock:(/*^block*/id)arg5 ;
-(void)postNotificationName:(id)arg1 object:(id)arg2 userInfo:(id)arg3 options:(unsigned long long)arg4 ;
-(void)postNotificationName:(id)arg1 object:(id)arg2 userInfo:(id)arg3 deliverImmediately:(BOOL)arg4 ;
-(BOOL)suspended;
-(void)addObserver:(id)arg1 selector:(SEL)arg2 name:(id)arg3 object:(id)arg4 ;
-(id)init;
-(void)removeObserver:(id)arg1 name:(id)arg2 object:(id)arg3 ;
-(void)postNotificationName:(id)arg1 object:(id)arg2 ;
-(void)postNotificationName:(id)arg1 object:(id)arg2 userInfo:(id)arg3 ;
-(id)addObserverForName:(id)arg1 object:(id)arg2 queue:(id)arg3 usingBlock:(/*^block*/id)arg4 ;
-(void)postNotification:(id)arg1 ;
-(void)setSuspended:(BOOL)arg1 ;
@end

@interface BBColor : NSObject
+(id)colorWithRed:(CGFloat)arg1 green:(CGFloat)arg2 blue:(CGFloat)arg3 alpha:(CGFloat)arg4 ;
@end

// @interface BBBulletin : NSObject
// @property(retain, nonatomic) NSDictionary *context;
// @property(retain, nonatomic) NSMutableDictionary* actions;
// @property(copy, nonatomic) NSArray* buttons;
// @end

// @interface BBBulletinRequest : BBBulletin
// @end

@interface CKComposition : NSObject
- (id)compositionByAppendingComposition:(id)arg1;
- (id)initWithText:(id)arg1 subject:(id)arg2;
- (NSAttributedString *)text;
- (BOOL)hasContent;
+ (instancetype)composition;
@end

@interface CKMessageEntryContentView : UIScrollView
- (void)setPlaceholderHeight:(float)arg1;
- (void)setPlaceholderText:(id)arg1;
@end

@interface CKMessageEntryView : UIView
@property (nonatomic,retain) CKMessageEntryContentView * contentView;
@property (assign, nonatomic) BOOL shouldShowSendButton;
@property (assign, nonatomic) BOOL shouldShowSubject;
@property (assign, nonatomic) BOOL shouldShowPhotoButton;
@property (assign, nonatomic) BOOL shouldShowCharacterCount;
@property (retain, nonatomic) UIButton *sendButton;
@property (retain, nonatomic) UIButton *photoButton;
@property (retain, nonatomic) CKComposition *composition;
// - (CKComposition *)composition;
@end

@interface CKManualUpdater : NSObject
- (void)setNeedsUpdate;
- (void)updateIfNeeded;
@end

@interface CKScheduledUpdater : CKManualUpdater
@end

@protocol CKMessageEntryViewDelegate <NSObject>
@required
- (void)messageEntryViewDidChange:(CKMessageEntryView *)entryView;
- (BOOL)messageEntryViewShouldBeginEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewDidBeginEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewDidEndEditing:(CKMessageEntryView *)entryView;
- (void)messageEntryViewRecordingDidChange:(CKMessageEntryView *)entryView;
- (BOOL)messageEntryView:(CKMessageEntryView *)entryView shouldInsertMediaObjects:(NSArray *)mediaObjects;
- (void)messageEntryViewSendButtonHit:(CKMessageEntryView *)entryView;
- (void)messageEntryViewSendButtonHitWhileDisabled:(CKMessageEntryView *)entryView;
- (void)messageEntryViewRaiseGestureAutoSend:(CKMessageEntryView *)entryView;
@optional
- (BOOL)getContainerWidth:(double*)arg1 offset:(double*)arg2;
@end

@protocol NCInteractiveNotificationHostInterface
@required
-(void)dismissWithContext:(id)arg1 ;
- (void)_dismissWithContext:(NSDictionary *)context;
- (void)_requestPreferredContentHeight:(CGFloat)height;
- (void)_setActionEnabled:(BOOL)enabled atIndex:(NSUInteger)index;
- (void)_requestProximityMonitoringEnabled:(BOOL)enabled;
@end

@interface NCInteractiveNotificationHostViewController : UIViewController <NCInteractiveNotificationHostInterface>
@end

@protocol NCInteractiveNotificationServiceInterface
@required
- (void)_setContext:(NSDictionary *)context;
- (void)_getInitialStateWithCompletion:(id)completion;
- (void)_setMaximumHeight:(CGFloat)maximumHeight;
- (void)_setModal:(BOOL)modal;
- (void)_interactiveNotificationDidAppear;
- (void)_proximityStateDidChange:(BOOL)state;
- (void)_didChangeRevealPercent:(CGFloat)percent;
- (void)_willPresentFromActionIdentifier:(NSString *)identifier;
- (void)_getActionContextWithCompletion:(id)completion;
- (void)_getActionTitlesWithCompletion:(id)completion;
- (void)_handleActionAtIndex:(NSUInteger)index;
- (void)_handleActionIdentifier:(NSString *)identifier;
@end

@interface NCInteractiveNotificationViewController : UIViewController <NCInteractiveNotificationServiceInterface>
@property (copy, nonatomic) NSDictionary *context;
@property (assign, nonatomic) CGFloat maximumHeight;
- (CGFloat)preferredContentHeight;
- (void)requestPreferredContentHeight:(CGFloat)height;
- (void)requestProximityMonitoringEnabled:(BOOL)enabled;
@end

@interface CKInlineReplyViewController : NCInteractiveNotificationViewController <CKMessageEntryViewDelegate>
@property (retain, nonatomic) CKMessageEntryView *entryView;
@property (retain, nonatomic) CKScheduledUpdater *typingUpdater;
- (UITextView *)viewForTyping;
- (void)setupConversation;
- (void)setupView;
- (void)interactiveNotificationDidAppear;
- (void)updateSendButton;
- (void)updateTyping;
- (void)sendMessage;
- (void)dismissWithContext:(id)arg1;
// - (void)getCameraRollImages;
// - (void)setupTableView_waqr;
// - (NSMutableArray *)getUserChatMessages_WAQr;
// - (NSString *)waqr_UserID;
// - (void)scrollToBottom_WAQR;
@end

@interface CallReplyInlineReplyViewController: CKInlineReplyViewController
@end

@interface BBObserver : NSObject
@end

@interface BBAppearance : NSObject
@property (copy, nonatomic) NSString *title;
@property (nonatomic,copy) BBColor * titleColor;
@property (nonatomic,copy) BBColor * color; 
@property (nonatomic,copy) NSString * viewClassName;
+ (instancetype)appearanceWithTitle:(NSString *)title;
@end

@interface BBAction : NSObject
@property (copy, nonatomic) NSString *identifier;
@property (assign, nonatomic) NSInteger actionType;
@property (copy, nonatomic) BBAppearance *appearance;
@property (copy, nonatomic) NSString *launchBundleID;
@property (copy, nonatomic) NSURL *launchURL;
@property (copy, nonatomic) NSString *remoteServiceBundleIdentifier;
@property (copy, nonatomic) NSString *remoteViewControllerClassName;
@property (assign, nonatomic) BOOL canBypassPinLock;
@property (assign, nonatomic) BOOL launchCanBypassPinLock;
@property (assign, nonatomic) NSUInteger activationMode;
@property (assign ,nonatomic, getter=isAuthenticationRequired) BOOL authenticationRequired;
@property (nonatomic,copy) id internalBlock;
+ (instancetype)action;
+ (instancetype)actionWithIdentifier:(NSString *)identifier;
+ (instancetype)actionWithLaunchBundleID:(NSString *)bundleID;
+ (instancetype)actionWithLaunchURL:(id)arg1 callblock:(/*^block*/id)arg2;
+ (instancetype)actionWithLaunchBundleID:(id)arg1 callblock:(/*^block*/id)arg2;
- (void)setAuthenticationRequired:(BOOL)arg1;
- (void)setShouldDismissBulletin:(BOOL)arg1;
- (void)setCallblock:(/*^block*/id)arg1;
- (void)setIdentifier:(NSString *)arg1;
- (void)setLaunchBundleID:(NSString *)arg1;
- (void)setInternalBlock:(id)arg1;
@end

@interface BBLaunchAction : BBAction
+ (id)launchActionWithURL:(id)arg1 callblock:(/*^block*/id)arg2 ;
+ (id)launchActionWithBundleID:(id)arg1 callblock:(/*^block*/id)arg2 ;
@end

@interface BBButton : NSObject
+ (id)buttonWithTitle:(id)arg1 action:(id)arg2 identifier:(id)arg3;
@end

@interface SBUIBannerAction : NSObject
@property (nonatomic,copy) id actionBlock;
+(id)actionWithBlock:(/*^block*/id)arg1 ;
@end

@interface BBContent : NSObject
@property (nonatomic,copy) NSString * title;                 //@synthesize title=_title - In the implementation block
@property (nonatomic,copy) NSString * subtitle;              //@synthesize subtitle=_subtitle - In the implementation block
@property (nonatomic,copy) NSString * message;               //@synthesize message=_message - In the implementation block
+(id)contentWithTitle:(id)arg1 subtitle:(id)arg2 message:(id)arg3 ;
@end

@interface BBBulletin : NSObject
@property (copy, nonatomic) NSString *bulletinID;
@property (copy, nonatomic) NSString *sectionID;
@property (copy, nonatomic) NSString *recordID;
@property (copy, nonatomic) NSString *publisherBulletinID;
@property (copy, nonatomic) NSString *title;
@property (copy, nonatomic) NSString *subtitle;
@property (copy, nonatomic) NSString *message;
@property (nonatomic, retain) NSDate *date;
@property (retain, nonatomic) NSDictionary *context;
@property (copy, nonatomic) NSDictionary *actions;
@property (nonatomic,copy) NSArray * buttons; 
@property (retain, nonatomic) NSMutableDictionary *supplementaryActionsByLayout;
@property (copy, nonatomic) BBAction *defaultAction;
@property (copy, nonatomic) BBAction *alternateAction;
@property (copy, nonatomic) BBAction *acknowledgeAction;
@property (nonatomic,retain) BBContent * content; 
- (NSArray *)_allActions;
- (NSArray *)_allSupplementaryActions;
- (NSArray *)supplementaryActions;
- (NSArray *)supplementaryActionsForLayout:(NSInteger)layout;
-(/*^block*/id)actionBlockForAction:(id)arg1;
-(/*^block*/id)actionBlockForButton:(id)arg1;
-(void)setContent:(BBContent *)arg1 ;
@end

@interface BBBulletinRequest : BBBulletin
// @property (nonatomic,copy) NSString * sectionID;
// @property (nonatomic,copy) NSString * title; 
// @property (nonatomic,copy) NSString * subtitle; 
// @property (nonatomic,copy) NSString * message;
- (void)setContextValue:(id)value forKey:(NSString *)key;
- (void)setSupplementaryActions:(NSArray *)actions;
- (void)setSupplementaryActions:(NSArray *)actions forLayout:(NSInteger)layout;
- (void)generateNewBulletinID;
// Todo ( add actions later )
@end

@interface SBBulletinListSection : NSObject
@property(copy, nonatomic) NSString *sectionID; // @synthesize sectionID=_sectionID;
@end

@interface SBBBItemInfo : NSObject
- (float)heightForReusableViewForBulletinViewController:(id)arg1 layoutMode:(int)arg2 bulletinLocation:(int)arg3;
- (void)populateReusableView:(id)arg1;
- (id)reusableViewIdentifier;
@property(readonly, nonatomic) NSString *identifier;
@end

@interface SBBBSectionInfo : SBBBItemInfo
{
    int _sectionCategory;
}

- (id)identifier;
@property(readonly, nonatomic) NSString *listSectionIdentifier;
@property(readonly, nonatomic, getter=isWidgetSection) BOOL widgetSection;
@property(readonly, nonatomic) int sectionCategory;
@property(readonly, nonatomic) SBBulletinListSection *representedListSection;

@end

@interface SBBBBulletinInfo : SBBBItemInfo

@property(readonly, nonatomic) NSString *originalSectionIdentifier;
- (id)identifier;
@property(readonly, nonatomic) BBBulletin *representedBulletin;

@end


@interface SBBannerContainerViewController : UIViewController
- (BOOL)_hasActions;
- (id)_bannerItem;
-(void)setBannerContext:(id)arg1 withReplaceReason:(int)arg2 completion:(/*^block*/id)arg3 ;
@end

@interface SBBannerButtonViewController : UIViewController
{
    NSArray *_buttons;
}

- (void)_buttonPressed:(id)arg1;
- (id)_buttonForActionTitle:(id)arg1;
- (id)_buttonView;
- (struct CGSize)preferredContentSize;
- (void)setButtonTitles:(id)arg1;
@end

@interface SBBulletinViewController : UITableViewController
@end

@interface SBUIBannerItem : NSObject
-(BBBulletin *)pullDownNotification;
@end

@interface SBUIBannerContext : NSObject
@property (nonatomic,retain,readonly) SBUIBannerItem * item;
@property (nonatomic,retain,readonly) id source;
@property (nonatomic,retain,readonly) id target;
@property (assign,nonatomic) BOOL requestsModalPresentation;
@property (nonatomic,copy) NSString * presentingActionIdentifier;
-(id)initWithItem:(SBUIBannerItem *)arg1 source:(id)arg2 target:(id)arg3 presentingActionIdentifier:(id)arg4 requestModalPresentation:(BOOL)arg5 ;
-(void)setPresentingActionIdentifier:(NSString *)arg1 ;
@end


@interface SBBulletinBannerController : NSObject
+ (id)sharedInstanceIfExists;
+ (id)sharedInstance;
+ (id)_sharedInstanceCreateIfNecessary:(BOOL)arg1;
- (void)observer:(id)arg1 removeBulletin:(id)arg2;
- (void)observer:(id)observer addBulletin:(BBBulletinRequest *)bulletin forFeed:(unsigned long long)feed;
- (void)observer:(id)arg1 addBulletin:(BBBulletinRequest *)bulletin forFeed:(unsigned int)arg3 playLightsAndSirens:(BOOL)arg4 withReply:(id)arg5; // iOS8.3 and above
- (void)removeCachedBannerForBulletinID:(id)arg1;
- (void)observer:(id)arg1 removeBulletin:(id)arg2;
-(SBUIBannerContext *)_bannerContextForBulletin:(BBBulletin *)bulletin;
- (void)_showTestBanner:(id)arg1;
@end

@interface SBLockScreenViewControllerBase : UIViewController
@end

@interface SBLockScreenViewController : SBLockScreenViewControllerBase
@end

@interface SBLockScreenNotificationListController : UIViewController
-(void)handleLockScreenActionWithContext:(id)arg1 ;
- (void)observer:(id)observer addBulletin:(BBBulletinRequest *)bulletin forFeed:(unsigned long long)feed;
- (void)observer:(id)arg1 addBulletin:(BBBulletinRequest *)bulletin forFeed:(unsigned int)arg3 playLightsAndSirens:(BOOL)arg4 withReply:(id)arg5; // iOS8.3 and above
- (void)_removeCachedBannerForBulletinID:(id)arg1;
- (void)observer:(id)arg1 removeBulletin:(id)arg2;
@end

@interface SBLockScreenNotificationListView : UIView
@property (assign,nonatomic)  SBLockScreenNotificationListController *delegate;
@end

@interface SBMutableLockScreenActionContext : NSObject

@property (nonatomic,retain) NSString * identifier; 
@property (nonatomic,retain) NSString * lockLabel; 
@property (nonatomic,retain) NSString * shortLockLabel; 
@property (nonatomic,copy) id action; 
@property (assign,nonatomic) BOOL requiresUIUnlock; 
@property (assign,nonatomic) BOOL deactivateAwayController; 
@property (assign,nonatomic) BOOL canBypassPinLock; 
@property (assign,nonatomic) BOOL requiresAuthentication; 
@property (assign,nonatomic) BBBulletin * bulletin; 
@end

@interface SBLockScreenManager : NSObject
@property(readonly, nonatomic) SBLockScreenViewController *lockScreenViewController; // @synthesize lockScreenViewController=_lockScreenViewController;
+ (id)sharedInstanceIfExists;
@end


@interface TUCall : NSObject
@property (nonatomic,copy,readonly) NSString * callUUID;
@property (copy) NSString * suggestedDisplayName;
-(NSString *)displayName;
-(BOOL)isVideo;
-(BOOL)isOutgoing;
-(int)status;
-(NSString *)destinationID;
-(id)contactImageDataWithFormat:(int)arg1;
@end

@interface TUCallCenter : NSObject
@property (nonatomic,retain,readonly) TUCall * incomingCall;
-(void)answerCall:(id)arg1;
-(void)disconnectCall:(id)arg1;
-(id)callWithCallUUID:(id)arg1;
-(void)handleCallConnected:(id)arg1;
@end

@interface PhoneApplication : UIApplication
- (void)registerForNotification;
@end

@interface FaceTimeApplication : PhoneApplication
@end

@interface MobilePhoneApplication : PhoneApplication
@end

@interface CallReplyHelper : NSObject

// Preferences
+ (NSString *)preferencesPath;
+ (CFStringRef)preferencesChanged;

// UIWindow to present your elements
// u can show/hide it using ( setHidden: NO/YES )
+ (UIWindow *)mainCallReplyWindow;
+ (UIViewController *)mainCallReplyViewController;

// Checking App Version
+ (BOOL)isAppVersionGreaterThanOrEqualTo:(NSString *)appversion;
+ (BOOL)isAppVersionLessThanOrEqualTo:(NSString *)appversion;

// Checking OS Version
+ (BOOL)isIOS83_OrGreater;
+ (BOOL)isIOS80_OrGreater;
+ (BOOL)isIOS70_OrGreater;
+ (BOOL)isIOS60_OrGreater;
+ (BOOL)isIOS50_OrGreater;
+ (BOOL)isIOS40_OrGreater;

// Checking Device Type
+ (BOOL)isIPhone6_Plus;
+ (BOOL)isIPhone6;
+ (BOOL)isIPhone5;
+ (BOOL)isIPhone4_OrLess;

// Checking Device Interface
+ (BOOL)isIPad;
+ (BOOL)isIPhone;

// Checking Device Retina
+ (BOOL)isRetina;

// Checking UIScreen sizes
+ (CGFloat)screenWidth;
+ (CGFloat)screenHeight;

// Custom Methods
+ (void)createLocalNotificationFromCall:(TUCall *)call;
@end
