#include "HelloWorldScene.h"

#include "../Visual/Layer.h"
#include "../Visual/MenuItem.h"
#include "../Visual/Window.h"

#include "InputEventTest.h"
#include "GestureRecognizerTest.h"

#include "extensions/CCMenuItemProgressTimer.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();
    
    TVPortable::Visual::Window* mainWindow = TVPortable::Visual::Window::MainWindow();
    mainWindow->setCaption("TVPortable!");

    TVPortable::Visual::Layer* c_layer = new TVPortable::Visual::Layer(mainWindow, 0);
    
   // c_layer->loadImages("HelloWorld.png", 0xFFFFFFFF);
    c_layer->focus();
   
    TVPortable::Visual::MenuItem* mainMenuItem = mainWindow->getMenu();
    
     if(mainMenuItem) {
        TVPortable::Visual::MenuItem* fileMenu = new TVPortable::Visual::MenuItem(mainWindow, "File");
         TVPortable::Visual::MenuItem* fileMenuX = new TVPortable::Visual::MenuItem(mainWindow, "Open");
         fileMenuX->setRadio(true);
         TVPortable::Visual::MenuItem* fileMenuY = new TVPortable::Visual::MenuItem(mainWindow, "Close");
         fileMenuY->setRadio(true);
         
         TVPortable::Visual::MenuItem* fileMenuZ = new TVPortable::Visual::MenuItem(mainWindow, "Open2");
         fileMenuZ->setRadio(true);
         fileMenuZ->setGroup(1);
         TVPortable::Visual::MenuItem* fileMenuK = new TVPortable::Visual::MenuItem(mainWindow, "Close2");
         fileMenuK->setRadio(true);
         fileMenuK->setGroup(1);
      //   fileMenuX->setChecked(true);
         
         fileMenu->add(fileMenuX);
         fileMenu->add(fileMenuY);
         
         fileMenu->add(fileMenuZ);
         fileMenu->add(fileMenuK);

      //   fileMenuX->add(fileMenuY);
        mainMenuItem->add(fileMenu);
        
    }
    
    CCMenu* menu = CCMenu::node();
    CCMenuItemProgressTimer* ptimer = CCMenuItemProgressTimer::itemFromSpriteAndTime(CCSprite::spriteWithFile("mask.png"), CCSprite::spriteWithFile("CloseNormal.png"), 5.f, kCCProgressTimerTypeVerticalBarBT);
    
    
    ptimer->setPosition(ccp(100, 110));
    menu->setIsTouchEnabled(true);
    
    layer->setIsTouchEnabled(true);
    
    
    c_layer->addChild(new MyInputListener());
    c_layer->addChild(new MyGestureRecognizer());
    menu->addChild(ptimer, 1);
    
    layer->addChild(menu);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	//this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Arial", 24);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

	// add the label as a child to this layer
	//this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	//this->addChild(pSprite, 0);
	
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
