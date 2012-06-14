//
//  Layer.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/14/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Layer.h"

namespace TVPortable {
    
    namespace Visual {
        
        using namespace cocos2d;
        
        void Layer::visit() {
            // quick return if not visible
            if (!getIsVisible()) {
                return;
            }
            
            glPushMatrix(); 
            glEnable(GL_SCISSOR_TEST);
                        
            CCEGLView::sharedOpenGLView().setScissorInPoints(mClippingRect.origin.x + getPosition().x, 
                                                             mClippingRect.origin.y + getPosition().y,
                                                             mClippingRect.size.width,
                                                             mClippingRect.size.height);
            
            CCNode::visit();
            glDisable(GL_SCISSOR_TEST);
            glPopMatrix();
        }
        
        Layer* Layer::LayerWithClippingRect(const cocos2d::CCRect& rc) {
            Layer* layer = new Layer();
            if(layer) {
                layer->setClippingRect(rc);
                layer->autorelease();
                return layer;
            }
            
            return NULL;
        }
    }
    
}
