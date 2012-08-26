//
//  Application.cpp
//  TVPortable
//
//  Created by Robert Bu on 7/22/12.
//
//

#include "Application.h"
#include "Window.h"

#include "cocos2d.h"
#include "Cocos2dScene.h"

#include "Layer.h"

TVP_NS_VISUAL_BEGIN

ApplicationConfig& ApplicationConfig::DefaultConfig() {
    static ApplicationConfig config = ApplicationConfig().setWindowWidth(800)
                                                         .setWindowHeight(600)
                                                         .setWindowTitle("Project TVPortablle")
                                                         .setResourcePath("./")
                                                         .setDisplayFps(60)
                                                         .setEnableRetina(true)
                                                         .setDisplayFps(true);
    return config;
}

class ApplicationImplCocos2dx: private cocos2d::CCApplication {
public:
    ApplicationImplCocos2dx(Application* app):
    mApplication(app) {
        assert(app);
        
        this->initInstance();
    }
    
    /**
     @brief	Implement for initialize OpenGL instance, set source path, etc...
     */
    bool initInstance() {
        bool bRet = false;
        {
            const ApplicationConfig& cfg = mApplication->getConfig();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            
            cocos2d::CCEGLView* mEGLView = new cocos2d::CCEGLView();
            bRet = mEGLView && mEGLView->Create(L"Test",
                                                 cfg.windowWidth,
                                                 cfg.windowHeight);
            
#endif  // CC_PLATFORM_WIN32
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            bRet = true;

            // OpenGLView initialized in testsAppDelegate.mm on ios platform, nothing need to do here.
            // 2.x ?
       //     cocos2d::CCDirector::sharedDirector()->setDeviceOrientation(cocos2d::kDeviceOrientationLeft);
            
#endif  // CC_PLATFORM_IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
            // OpenGLView initialized in HelloWorld/android/jni/helloworld/main.cpp
            // the default setting is to create a fullscreen view
            // if you want to use auto-scale, please enable view->create(320,480) in main.cpp
            // if the resources under '/sdcard" or other writeable path, set it.
            // warning: the audio source should in assets/
            // cocos2d::CCFileUtils::setResourcePath("/sdcard");
            
            bRet = true;

#endif  // CC_PLATFORM_ANDROID
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
            
            // Initialize OpenGLView instance, that release by CCDirector when application terminate.
            // The HelloWorld is designed as HVGA.
            CCEGLView* mEGLView = new CCEGLView();
            bRet = mEGLView && mEGLView->Create(cfg.windowTitle.c_str(),
                                                  cfg.windowWidth,
                                                  cfg.windowHeight,
                                                  cfg.windowWidth,
                                                  cfg.windowHeight);
                        
            bRet = true;

#endif  // CC_PLATFORM_LINUX
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            
            // window created by AppController here
            // looking for better solution
            bRet = true;
            
#endif
			mApplication->initTVPortableWindow();
            cocos2d::CCFileUtils::sharedFileUtils()->setResourcePath(cfg.resourcePath.c_str());

        }
        return bRet;
    }
    
    virtual bool applicationDidFinishLaunching() {
        cocos2d::CCDirector *pDirector = cocos2d::CCDirector::sharedDirector();
        
        const ApplicationConfig& cfg = mApplication->getConfig();
        
        pDirector->setOpenGLView(&cocos2d::CCEGLView::sharedOpenGLView());
        pDirector->enableRetinaDisplay(cfg.enableRetina);
        pDirector->setDisplayStats(false);
        pDirector->setAnimationInterval(1.0 / cfg.fps);
    
        mApplication->applicationDidFinishLaunching();
        
        return true;
    }
    
    virtual void applicationDidEnterBackground() {
        mApplication->applicationDidEnterBackground();
    }
    
    virtual void applicationWillEnterForeground() {
        mApplication->applicationWillEnterForeground();
    }
    
private:
    Application* mApplication;
    
};

Application::Application(const ApplicationConfig& config):
mAppConfig(config),
mMainWindow(0) {
}

Application::~Application() {
    
}

Application& Application::App(const ApplicationConfig& config) {
    static Application* app = new Application(config);
    return *app;
}

void Application::initTVPortableWindow() {
	
	mMainWindow = Window::Create(mAppConfig.windowWidth,
                                 mAppConfig.windowHeight);
    
}

Window* Application::getMainWindow() const {
    return mMainWindow;
}

void Application::run() {
    // cocos2d implementation
    new ApplicationImplCocos2dx(this);
    
    cocos2d::CCDirector::sharedDirector()->runWithScene(new Cocos2dScene());
    
    cocos2d::CCApplication::sharedApplication().run();
    
    return;
}

bool Application::isStarted() const {
    return mMainWindow != 0;
}

void Application::applicationDidEnterBackground() {
    
}

void Application::applicationWillEnterForeground() {
    
}

void Application::applicationDidFinishLaunching() {

}

void Application::terminate() {
    exit(0);
}

const ApplicationConfig& Application::getConfig() const {
    return mAppConfig;
}

TVP_NS_VISUAL_END