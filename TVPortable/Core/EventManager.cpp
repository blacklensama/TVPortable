//
//  EventManager.cpp
//  TVPortable
//
//  Created by Mili Zhang on 7/19/12.
//
//

#include "EventManager.h"

TVP_NS_CORE_BEGIN

Event::Event(tjs_uint tag, iTJSDispatch2* src, iTJSDispatch2* dst, ttstr evname,
             tTJSVariant** argv, tjs_int argc, bool discardable)
{
    this->tag = tag;
    
    this->src = src;
    src->AddRef();
    
    
    this->dst = dst;
    dst->AddRef();
    
    this->evname = evname;
    this->argc = argc;
    
    
    this->args = new tTJSVariant[argc];
    
    //Copy all the arguments to local
    for(int i=0; i<argc; i++)
    {
        this->args[i] = *argv[i];
    }

    this->discardable = discardable;
}

Event::~Event()
{
    src->Release();
    dst->Release();
    delete[] args;
}

void Event::dispatch()
{
    Event::dispatch(this);
}

EventManager::EventManager()
{
    //TODO: further initialize if needed, write logs.
    enabled = true;
}

EventManager::~EventManager()
{
    
}

void EventManager::post(int prio, bool immediate, tjs_uint tag, TJS::iTJSDispatch2 *src, TJS::iTJSDispatch2 *dst,
                        tjs_char *evname, TJS::tTJSVariant **argv, tjs_int argc, bool discardable)
{
    // Discard discardable events when event handling is disabled.
    if (enabled == false && discardable == true) return;
    
    if (immediate == true && enabled == true)
    {
        Event::dispatch(src, dst, evname, argv, argc);
    }
    else
    {
        Event* ev = new Event(tag, src, dst, evname, argv, argc, discardable);
        switch (prio)
        {
            case TVP_EPT_EXCLUSIVE:
                if (immediate == true) exclusiveQueue.push_front(ev);
                exclusiveQueue.push_back(ev);
                break;
                
            case TVP_EPT_IDLE:
                if (immediate == true) idleQueue.push_front(ev);
                idleQueue.push_back(ev);
                break;
                
            case TVP_EPT_NORMAL:
            default:
                if (immediate == true) normalQueue.push_front(ev);
                normalQueue.push_back(ev);
                break;
        }
    }
}

// Unlike dispatchAll(), update() should control the time used on event dispatching each frame,
//   to keep the engine responding to OS message & repaint the graphics correctly.
// To achieve this, a time limit is set for each frame. In case the time limit is exceeded,
//   the remaining events will be delayed to next frame for processing.
//
void EventManager::update()
{
    if (enabled == false) return;
    
    ukn::Timestamp time;
    
    while(!exclusiveQueue.empty())
    {
        dispatchQueue(exclusiveQueue);
        if (time.isElapsed(TVP_EV_FRAMETIME)==true)
            return;
    }
    
    
    while(!normalQueue.empty())
    {
        dispatchQueue(normalQueue);
        if (time.isElapsed(TVP_EV_FRAMETIME)==true)
            return;
    }
    
    
    while(!idleQueue.empty())
    {
        dispatchQueue(idleQueue);
        if (time.isElapsed(TVP_EV_FRAMETIME)==true)
            return;
    }
}

void EventManager::dispatchAll()
{
    if (enabled == false) return;
    
    while(!exclusiveQueue.empty())
    {
        dispatchQueue(exclusiveQueue);
    }
    
    while(!normalQueue.empty())
    {
        dispatchQueue(normalQueue);
    }
    
    while(!idleQueue.empty())
    {
        dispatchQueue(idleQueue);
    }
}

void EventManager::clearAll()
{
    while(!exclusiveQueue.empty())
    {
        delete exclusiveQueue.front();
        exclusiveQueue.pop_front();
    }
    
    while(!normalQueue.empty())
    {
        delete normalQueue.front();
        normalQueue.pop_front();
    }
    
    while(!idleQueue.empty())
    {
        delete idleQueue.front();
        idleQueue.pop_front();
    }
}

void EventManager::clearDiscardable()
{
    std::deque<Event*>::iterator queueIter = exclusiveQueue.begin();
    while(queueIter!=exclusiveQueue.end())
    {
        if((*queueIter)->isDiscardable())
        {
            delete *queueIter;
            queueIter = exclusiveQueue.erase(queueIter);
        }
        else
        {
            queueIter++;
        }
    }
    
    queueIter = normalQueue.begin();
    while(queueIter!=normalQueue.end())
    {
        if((*queueIter)->isDiscardable())
        {
            delete *queueIter;
            queueIter = normalQueue.erase(queueIter);
        }
    }
    
    queueIter = idleQueue.begin();
    while(queueIter!=idleQueue.end())
    {
        if((*queueIter)->isDiscardable())
        {
            delete *queueIter;
            queueIter = idleQueue.erase(queueIter);
        }
    }

}

void EventManager::setEnabled(bool enabled)
{
    if (this->enabled == enabled)
    {
        return;
    }
    else
    {
        this->enabled = enabled;
        if(enabled == false)
        {
            clearDiscardable();
        }
    }
}

inline void EventManager::dispatchQueue(std::deque<Event*>& queue)
{
    Event* ev = queue.front();
    ev->dispatch();
    delete ev;
    
    queue.pop_front();
}

TVP_NS_CORE_END
