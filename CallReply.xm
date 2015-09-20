//
//  CallReply.x
//  CallReply
//
//  Created by iMokhles on 26.08.2015.
//  Copyright (c) 2015 iMokhles. All rights reserved.
//

#import "CallReplyHelper.h"
#import "CallReplyNotificationWindow.h"
#import "CallReplyNotificationView.h"
#import <AppList/ALApplicationList.h>
#import <AddressBook/AddressBook.h>

%group main

BOOL isLockScreenHere;

BOOL isTweakEnabled;
static void CallReply_Prefs() {
	NSDictionary *CallReplySettings = [NSDictionary dictionaryWithContentsOfFile:[CallReplyHelper preferencesPath]];
	NSNumber *isTweakEnabledNU = CallReplySettings[@"isTweakEnabled"];
    isTweakEnabled = isTweakEnabledNU ? [isTweakEnabledNU boolValue] : 0;
}

%hook TUCallCenter

// TUFaceTimeAudioCall - FaceTime -

- (void)handleCallStatusChanged:(TUCall *)call userInfo:(id)info
{

	if ([call status] == 4) {

		if (isLockScreenHere) {
			%orig;
		} else {
			UIImage *appImage = [UIImage imageWithData:[call contactImageDataWithFormat:kABPersonImageFormatThumbnail]];
			if (appImage == nil) {
				if ([self.incomingCall isKindOfClass:objc_getClass("TUFaceTimeAudioCall")]) {
					// Facetime Audio Call
					appImage = [[NSClassFromString(@"ALApplicationList") sharedApplicationList] iconOfSize:59 forDisplayIdentifier:@"com.apple.facetime"];
				} else if ([self.incomingCall isKindOfClass:objc_getClass("TUTelephonyCall")]) {
					// Telephony Audio Call
					appImage = [[NSClassFromString(@"ALApplicationList") sharedApplicationList] iconOfSize:59 forDisplayIdentifier:@"com.apple.mobilephone"];
				}
			}
			
			// [[CallReplyNotificationWindow sharedWindow] openWindow];
			[CallReplyNotificationView showNotificationViewWithImage:appImage title:call.displayName message:@"Incoming Call" buttons:@[@"Decline", @"Accept"]isAutoHide:NO onTouch:^{
		        [CallReplyNotificationView hideNotificationViewOnComplete:^{
				            
				}];
		    }];
		    [CallReplyNotificationView listenEventsWithBlock:^(CallReplyNotificationEvent event) {
		    	[CallReplyNotificationView hideNotificationViewOnComplete:^{
				    switch (event) {
			            case CallReplyNotificationEventTopButton:
			                // Top button
			                NSLog(@"Top Button");
			                [self disconnectCall:call];
			                break;
			                
			            case CallReplyNotificationEventBottomButton: {
			            	// Bottom button
			                NSLog(@"Bottom Button");
			                [self answerCall:call];
			                // TUCallCenterCallStatusChangedInternalNotification // Testing
			                [[NSNotificationCenter defaultCenter] postNotificationName:@"TUCallCenterCallStatusChangedInternalNotification" object:nil userInfo:nil];
			                [[NSNotificationCenter defaultCenter] postNotificationName:@"TUCallCenterCallStatusChangedNotification" object:nil userInfo:nil];
			                SpringBoard *sb = (SpringBoard *)[UIApplication sharedApplication].delegate;
			                [sb handleDoubleHeightStatusBarTap:1]; // ", call.destinationID
			                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"facetime://"]]]; // testing
			            }
			                
			                break;
			                
			            default:
			                break;
			        }    
				}];
		        
		    }];
		}
	} else if ([call status] == 6) {
		[CallReplyNotificationView hideNotificationViewOnComplete:^{
		            
		}];
		// [[CallReplyNotificationWindow sharedWindow] closeWindow];
		%orig;
	} else {
		%orig;
	}
	NSLog(@"************ %ld", (long)call.status);

}
%end

// TUCallCenterCallConnectedNotification //Connected
%hook SBLockScreenViewController
- (void)viewWillAppear:(BOOL)arg1 {
	isLockScreenHere = YES;
	%orig;
}
%end

%hook SBLockScreenManager
- (void)_finishUIUnlockFromSource:(NSInteger)source withOptions:(NSDictionary *)options {
	isLockScreenHere = NO;
	%orig;
}
%end
%end



%ctor {
	@autoreleasepool {
		[[NSNotificationCenter defaultCenter] addObserverForName:UIApplicationDidFinishLaunchingNotification object:nil queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *block) {
	        CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(), NULL, (CFNotificationCallback)CallReply_Prefs, [CallReplyHelper preferencesChanged], NULL, 0);
	        CallReply_Prefs();
	    }];
	    if ([[[NSProcessInfo processInfo] processName] isEqualToString:@"SpringBoard"]) %init(main);
	}
}
