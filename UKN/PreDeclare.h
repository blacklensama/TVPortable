//
//  PreDeclare.h
//  Project Unknown
//
//  Created by Robert Bu on 11/22/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_PreDeclare_h
#define Project_Unknown_PreDeclare_h

#include "UKN/Platform.h"
#include "UKN/Ptr.h"

#include <string>

namespace ukn {
    
    template<int size, typename T>
    struct PointTemplate;
    
    typedef PointTemplate<1, float> float1;
    typedef PointTemplate<2, float> float2;
    typedef PointTemplate<3, float> float3;
    typedef PointTemplate<4, float> float4;
    
    typedef PointTemplate<1, int32> int1;
    typedef PointTemplate<2, int32> int2;
    typedef PointTemplate<3, int32> int3;
    typedef PointTemplate<4, int32> int4;
    
    typedef PointTemplate<1, uint32> uint1;
    typedef PointTemplate<2, uint32> uint2;
    typedef PointTemplate<3, uint32> uint3;
    typedef PointTemplate<4, uint32> uint4;

    struct VertexFormat;
    
    struct ContextCfg;
    
    class GraphicDevice;
    typedef SharedPtr<GraphicDevice> GraphicDevicePtr;
    
    class GraphicFactory;
    typedef SharedPtr<GraphicFactory> GraphicFactoryPtr;
    
    class WindowFactory;
    typedef SharedPtr<WindowFactory> WindowFactoryPtr;
    
    class GraphicBuffer;
    typedef SharedPtr<GraphicBuffer> GraphicBufferPtr;
    
    class FrameBuffer;
    typedef SharedPtr<FrameBuffer> FrameBufferPtr;
    
    class RenderBuffer;
    typedef SharedPtr<RenderBuffer> RenderBufferPtr;
    
    class RenderView;
    typedef SharedPtr<RenderView> RenderViewPtr;
    
    class RenderEffect;
    typedef SharedPtr<RenderEffect> RenderEffectPtr;
    
    
    class Texture;
    typedef SharedPtr<Texture> TexturePtr;
    
    class Font;
    typedef SharedPtr<Font> FontPtr;
    
    class Resource;
    typedef SharedPtr<Resource> ResourcePtr;
    
    class Texture;
    typedef SharedPtr<Texture> TexturePtr;
    
    class Stream;
    typedef SharedPtr<Stream> StreamPtr;
    
    class Camera;
    typedef SharedPtr<Camera> CameraPtr;
    
    class Window;
    typedef SharedPtr<Window> WindowPtr;
    
    class ConfigParser;
    typedef SharedPtr<ConfigParser> ConfigParserPtr;
    
    class AppInstance;
    typedef SharedPtr<AppInstance> AppPtr;
    
    class ResourceFactory;
    typedef SharedPtr<ResourceFactory> ResourceFactoryPtr;
    
    class ResourceLoader;
    typedef SharedPtr<ResourceLoader> ResourceLoaderPtr;
    
    class SpriteBatch;
    typedef SharedPtr<SpriteBatch> SpriteBatchPtr;
    
    class SceneNode;
    typedef SharedPtr<SceneNode> SceneNodePtr;
    
    class Scene;
    typedef SharedPtr<Scene> ScenePtr;
    
    class Entity;
    typedef SharedPtr<Entity> EntityPtr;
    
    class Component;
    typedef SharedPtr<Component> ComponentPtr;
    
    class Task;
    typedef SharedPtr<Task> TaskPtr;
    
    class FSMState;
    typedef SharedPtr<FSMState> FSMStatePtr;
    
    class Condition;
    typedef SharedPtr<Condition> ConditionPtr;
    
    class Operator;
    typedef SharedPtr<Operator> OperatorPtr;
    
    class Action;
    typedef SharedPtr<Action> ActionPtr;
    
    class Trigger;
    typedef SharedPtr<Trigger> TriggerPtr;
    
    class IRenderable;
    typedef SharedPtr<IRenderable> RenderablePtr;
    
    class SceneNode;
    typedef SharedPtr<SceneNode> SceneNodePtr;
    
    class SceneObject;
    typedef SharedPtr<SceneObject> SceneObjectPtr;
    
    class SceneManager;
    typedef SharedPtr<SceneManager> SceneManagerPtr;
    
    class Entity;
    typedef SharedPtr<Entity> EntityPtr;
    
    class Component;
    typedef SharedPtr<Component> ComponentPtr;
    
    class ComponentHolder;
    typedef SharedPtr<ComponentHolder> ComponentHolderPtr;
    
    class SequencialAnimation;
    typedef SharedPtr<SequencialAnimation> SequencialAnimationPtr;
    
    class RenderTarget2D;
    typedef SharedPtr<RenderTarget2D> RenderTargetPtr;
    
} // namespace ukn


#endif
