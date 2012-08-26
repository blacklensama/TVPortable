//
//  Cocos2dScene.cpp
//  TVPortable
//
//  Created by Robert Bu on 7/22/12.
//
//

#include "Cocos2dScene.h"

#include "Layer.h"
#include "Window.h"
#include "Application.h"

TVP_NS_VISUAL_BEGIN

Cocos2dScene::Cocos2dScene() {
    
    Layer* layer = new Layer(Application::App().getMainWindow(), 0);
    layer->loadImages("IMG_1181.JPG", 0);
    
    this->addChild(layer);
    
    cocos2d::CCSprite* spr = cocos2d::CCSprite::create("IMG_1181.JPG");
    this->addChild(spr);
}

Cocos2dScene::~Cocos2dScene() {
    
}


TVP_NS_VISUAL_END