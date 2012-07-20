//
//  EventManager.h
//  TVPortable
//
//  Created by Mili Zhang on 7/18/12.
//
//

#include <queue>

#include "../Common/Common.h"
#include "UKN/TimeUtil.h"

#ifndef TVPortable_EventManager_h
#define TVPortable_EventManager_h

// Priority/Method Definition: from Original Kirikiri Code

#define TVP_EPT_NORMAL			0x00
// (with TVP_EPT_POST only)
// the event will have normal priority.

#define TVP_EPT_IMMEDIATE		0x02
// the event will be delivered immediately

#define TVP_EPT_EXCLUSIVE		0x20
// (with TVP_EPT_POST only)
// the event is given priority and other posted events are not processed
// until the exclusive event is processed.

#define TVP_EPT_IDLE			0x40
// (with TVP_EPT_POST only)
// the event is only delivered after the system processes all other events.
// this will have a priority roughly identical to "continuous" events.

#define TVP_EV_FRAMETIME               15

TVP_NS_CORE_BEGIN

class Event
{
    
public:
    Event(tjs_uint tag, iTJSDispatch2* src, iTJSDispatch2* dst, ttstr evname,
          tTJSVariant** argv, tjs_int argc, bool discardable);
    
    ~Event();
    
    void dispatch();
    
    bool isDiscardable() {return discardable;};
    
    //TODO: Object Check & Exception Handling
    static void dispatch(iTJSDispatch2* src, iTJSDispatch2* dst,
                         ttstr evname, tTJSVariant** argv, tjs_int argc)
    {
        dst->FuncCall(NULL, evname.c_str(), evname.GetHint(), NULL, argc, argv, dst);
    }
    
    static void dispatch(Event* event)
    {
        Event::dispatch(event->src, event->dst, event->evname, &event->args, event->argc);
    }
    
protected:
    iTJSDispatch2* src;
    iTJSDispatch2* dst;
    ttstr evname;
    tTJSVariant* args;
    tjs_int argc;
    tjs_uint tag;
    bool discardable;
};

class EventManager
{
    
protected:
    EventManager();
    
public:
    
    ~EventManager();
            
    void update();
    
    void post(int prio, bool immediate, tjs_uint tag, iTJSDispatch2* src, iTJSDispatch2* dst,
                   tjs_char* evname, tTJSVariant** args, tjs_int argc, bool discardable);
    
    void remove();
    
    void clearAll();
    void clearDiscardable();
    void dispatchAll();
    
    bool isEnabled() {return enabled;};
    void setEnabled(bool enabled);
    
    static void Initialize()
    {
        instance = new EventManager();
    }
    
    static void Release()
    {
        delete instance;
        instance = 0;
    }
    
    static EventManager& Instance()
    {
        return *instance;
    }
    
private:
    
    inline void dispatchQueue(std::deque<Event*>& queue);
    
    std::deque<Event*> exclusiveQueue;
    std::deque<Event*> normalQueue;
    std::deque<Event*> idleQueue;
    
    bool enabled;
    
    static EventManager* instance;
};

TVP_NS_CORE_END

#endif
