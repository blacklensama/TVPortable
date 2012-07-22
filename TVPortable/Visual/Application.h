//
//  Application.h
//  TVPortable
//
//  Created by Robert Bu on 7/22/12.
//
//

#ifndef __TVPortable__Application__
#define __TVPortable__Application__

#include "Application.h"
#include "Prerequisites.h"

#include <string>

TVP_NS_VISUAL_BEGIN

class Window;
class Layer;
class MenuItem;

struct ApplicationConfig {
    /* width & height only useful on desktop platforms */
    /* will adjust automatically on mobile platforms */
    size_t windowWidth;
    size_t windowHeight;
    
    std::string windowTitle;
    std::string resourcePath;
    
    /* ios retina display support */
    bool enableRetina;
    bool displayFPS;
    
    size_t fps;
    
    ApplicationConfig& setWindowWidth(size_t width) { windowWidth = width; return *this; }
    ApplicationConfig& setWindowHeight(size_t height) { windowHeight = height; return *this; }
    ApplicationConfig& setWindowTitle(const std::string& title) { windowTitle = title; return *this; }
    ApplicationConfig& setResourcePath(const std::string& path) { resourcePath = path; return *this; }
    ApplicationConfig& setDisplayFps(bool flag) { displayFPS = flag; return *this; }
    ApplicationConfig& setEnableRetina(bool flag) { enableRetina = flag; return *this; }
    
    static ApplicationConfig& DefaultConfig();
};

/* Global application object */
class Application {
protected:
    Application(const ApplicationConfig& config);
    ~Application();

public:
    static Application& App(const ApplicationConfig& config = ApplicationConfig::DefaultConfig());
    
    /* start the application, will return false if a application is already started */
    void run();
    void terminate();
    
    bool isStarted() const;
    
    Window* getMainWindow() const;
    
    const ApplicationConfig& getConfig() const;
    
private:
    /* ios & osx*/
    friend class ApplicationImplCocos2dx;
    
    void applicationDidEnterBackground();
    void applicationWillEnterForeground();
    void applicationDidFinishLaunching();
    
    Window* mMainWindow;
    ApplicationConfig mAppConfig;
};


TVP_NS_VISUAL_END

#endif /* defined(__TVPortable__Application__) */
