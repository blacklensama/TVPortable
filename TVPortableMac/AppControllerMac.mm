//
//  AppDelegate.m
//  pouet
//
//  Created by joseph pinkasfeld on 5/18/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//
#include "CCPlatformConfig.h"
#include "AppDelegate.h"
#import "AppControllerMac.h"
#include "CCDirectorMac.h"


@implementation AppControllerMac
@synthesize window, glView;

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
//    cocos2d::CCDirectorMac *director = (cocos2d::CCDirectorMac*) cocos2d::CCDirectorMac::sharedDirector();
//    
    NSRect rect = NSMakeRect(650, 350, 480, 320);    
    window = [[NSWindow alloc] initWithContentRect: rect styleMask:( NSClosableWindowMask | NSTitledWindowMask) backing:NSBackingStoreBuffered defer:YES];
    
    glView = [[EAGLView alloc] initWithFrame:rect];
    [glView initWithFrame:rect];
    [window setContentView:glView];
    [window setTitle:@"Cocos2d-x Hello World"];
    
    [window makeKeyAndOrderFront:self];
    cocos2d::CCApplication::sharedApplication().run();
//	director->setOpenGLView(glView_);
//
//	// EXPERIMENTAL stuff.
//	// 'Effects' don't work correctly when autoscale is turned on.
//	// Use kCCDirectorResize_NoScale if you don't want auto-scaling.
//	director->setResizeMode(kCCDirectorResize_AutoScale);
//	
//	// Enable "moving" mouse event. Default no.
//	[window_ setAcceptsMouseMovedEvents:NO];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) theApplication
{
	return YES;
}

- (void)dealloc
{
	cocos2d::CCDirectorMac::sharedDirector()->end();
	[window release];
    [glView release];
	[super dealloc];
}

#pragma mark AppDelegate - IBActions

- (IBAction)toggleFullScreen: (id)sender
{
    cocos2d::CCDirectorMac *director = (cocos2d::CCDirectorMac*) cocos2d::CCDirectorMac::sharedDirector();
	director->setFullScreen(!director->isFullScreen());
}

@end
