//
//  AppDelegate.m
//  pouet
//
//  Created by joseph pinkasfeld on 5/18/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//
#include "CCPlatformConfig.h"
#import "AppControllerMac.h"
#include "CCDirectorMac.h"

#include "TVPortable/Visual/Application.h"
#include "ukn/SysUtil.h"

@implementation AppControllerMac
@synthesize window, glView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
//    cocos2d::CCDirectorMac *director = (cocos2d::CCDirectorMac*) cocos2d::CCDirectorMac::sharedDirector();
//
    
    TVPortable::Visual::Application& myApp = TVPortable::Visual::Application::App(TVPortable::Visual::ApplicationConfig::DefaultConfig());
    
    ukn::SystemInformation::DesktopMode desktopResolution = ukn::SystemInformation::EnumDesktopMode().front();
    
    NSRect rect = NSMakeRect(desktopResolution.width / 2 - myApp.getConfig().windowWidth / 2,
                             desktopResolution.height / 2 - myApp.getConfig().windowHeight / 2,
                             myApp.getConfig().windowWidth,
                             myApp.getConfig().windowHeight);
    window = [[NSWindow alloc] initWithContentRect: rect styleMask:( NSClosableWindowMask | NSTitledWindowMask) backing:NSBackingStoreBuffered defer:YES];
    
    glView = [[EAGLView alloc] initWithFrame:rect];
    [glView initWithFrame:rect];
    [window setContentView:glView];
    [window setTitle:@"TVPortable"];
    
    [window makeKeyAndOrderFront:self];

    myApp.run();

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
