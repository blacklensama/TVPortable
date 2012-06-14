//
//  AppDelegate.h
//  pouet
//
//  Created by joseph pinkasfeld on 5/18/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#import "cocos2d.h"
#import "EAGLView_mac.h"
@interface AppControllerMac : NSObject <NSApplicationDelegate>
{
    NSWindow    *window;
    EAGLView	*glView;
}

@property (nonatomic, retain) NSWindow *window;
@property (nonatomic, retain) EAGLView *glView;

- (IBAction)toggleFullScreen:(id)sender;

@end
