//
//  FSM.h
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_FSM_h
#define Project_Unknown_FSM_h

#include "UKN/Platform.h"
#include "UKN/Function.h"

#include <map>

namespace ukn {
    
    /**
     *  A Simple FSM Based on a user defined compile-time state_type
     **/
    
    template<typename t>
    struct simple_fsm_do_nothing_one {
        void operator()(t) {
        }
    };
    
    template<typename t1, typename t2>
    struct simple_fsm_do_nothing_two {
        void operator()(t1, t2) {
            
        }
    };
    
    /**
     *  Get Null state for initial state
     *  You can disable null state by disable SORA_FSM_USE_NULL
     **/
    template<typename state_type>
    const state_type& simple_fsm_get_null_state();
    
    template<>
    inline const unsigned char& simple_fsm_get_null_state() {
        static unsigned char null = 0xff;
        return null;
    }
    
    template<>
    inline const int& simple_fsm_get_null_state() {
        static int null = 0xffffffff;
        return null;
    }
    
    template<>
    inline const ukn_string& simple_fsm_get_null_state() {
        static ukn_string null("null");
        return null;
    }
    
    template<typename state_type, typename event_type>
    struct SimpleFSM {   
        typedef state_type                                          StateType;
        typedef event_type                                          EventType;
        typedef SimpleFSM                                           MachineType;
        typedef SimpleFSM<state_type, event_type>                   SelfType;
        typedef simple_fsm_do_nothing_one<state_type>               DoNothingOp1;
        typedef simple_fsm_do_nothing_two<MachineType, state_type>  DoNothingOp2;
        
#ifdef SORA_FSM_USE_NULL
        SimpleFSM():
        mCurrState(simple_fsm_get_null_state<state_type>()) {
        }
#else
        SimpleFSM() {
        }
#endif
        
        SelfType& add(state_type state) {
            this->mStateMap.insert(std::make_pair(state, state_trans()));
            return *this;
        }
        SelfType& del(state_type state) {
            typename StateMap::iterator itState = this->mStateMap.find(state);
            if(itState != this->mStateMap.end()) {
                this->mStateMap.erase(itState);
            }
            return *this;
        }
        
        void setState(state_type state) {
            typename StateMap::iterator itState = this->mStateMap.find(state);
            if(itState == this->mStateMap.end()) {
                return;
            }
            this->mCurrState = state;
        }
        
        void clear() {
#ifdef SORA_FSM_USE_NULL
            this->mCurrState = simple_fsm_get_null_state<state_type>();
#else 
            this->mCurrState = state_type();
#endif 
            this->mStateMap.clear();
        }
        
        SelfType& defTrans(state_type state, event_type event, state_type stateto) {
            typename StateMap::iterator itState = this->mStateMap.find(state);
            if(itState == this->mStateMap.end()) {
                // should we allow this?
                state_trans trans;
                trans.mTransitions.insert(std::make_pair(event, stateto));
                this->mStateMap.insert(std::make_pair(state, trans));
            } else {
                itState->second.mTransitions.insert(std::make_pair(event, stateto));
            }
            return *this;
        }
        
        SelfType& delTrans(state_type state, event_type event) {
            typename StateMap::iterator itState = this->mStateMap.find(state);
            if(itState != this->mStateMap.end()) {
                typename StateEventMap::iterator itEvent = itState->second.mTransitions.find(event);
                if(itEvent != itState->second.mTransitions.end()) {
                    itState->second.mTransitions.erase(itEvent);
                }
            }
            return *this;
        }
        
        SelfType& procEvent(event_type event) {
            DoNothingOp2 donothing;
            procEvent(event, donothing, donothing);
            return *this;
        }
        
        template<typename event_enter_op, typename event_exit_op>
        void procEvent(event_type event, event_enter_op enterOp, event_exit_op exitOp) {
            typename StateMap::iterator itState = this->mStateMap.find(mCurrState);
            if(itState != this->mStateMap.end()) {
                typename StateEventMap::iterator itEvent = itState->second.mTransitions.find(event);
                if(itEvent != itState->second.mTransitions.end()) {
                    exitOp(*this, this->mCurrState);
                    mCurrState = itEvent->second;
                    enterOp(*this, this->mCurrState);
                }
            }
        }
        
        size_t getNumStates() const {
            return this->mStateMap.size();
        }
        
        size_t getNumTransitions(const state_type& state) const {
            typename StateMap::const_iterator itState = this->mStateMap.find(state);
            if(itState != this->mStateMap.end()) {
                return itState->second.mTransitions.size();
            }
        }
        
        const state_type& getCurrState() const {
            return this->mCurrState;
        }
        
    private:
        typedef std::map<event_type, state_type> StateEventMap;
        struct state_trans {
            StateEventMap mTransitions;
        };
        typedef std::map<state_type, state_trans> StateMap;
        StateMap mStateMap;
        
        state_type mCurrState;
    };
    
    
    class FSMManager;
    
    class FSMState {
    public:
        friend class FSMManager;
        
        FSMState(): 
        mInitiated(false), 
        mManager(NULL) {
       
        }
        
        virtual ~FSMState() {
            
        }
        
        void destroy() {
            this->onDestroy();
            
            delete this;
        }
        
        virtual void onInitiate() {}
        virtual void onUpdate() {}
        
        virtual void onEnter() {}
        virtual void onLeave() {}
        
        virtual void onDestroy() {}
        
        FSMManager* getManager() const {
            return this->mManager;
        }
        
    private:
        bool mInitiated;
        FSMManager* mManager;
    };
    
    static void fsm_do_nothing(FSMState&) {
            
    }
        
    static Function<void(FSMState&)> fsm_do_nothing_func = fsm_do_nothing;
    
    class FunctionFSMState: public FSMState {
    public:
        friend class FSMManager;
        
        typedef Function<void(FSMState&)> InitiateFunc;
        typedef Function<void(FSMState&)> UpdateFunc;
        typedef Function<void(FSMState&)> EnterFunc;
        typedef Function<void(FSMState&)> LeaveFunc;
        typedef Function<void(FSMState&)> DestroyFunc;
        
    public:
        FunctionFSMState():
        mOnInit(fsm_do_nothing_func),
        mOnUpdate(fsm_do_nothing_func),
        mOnEnter(fsm_do_nothing_func),
        mOnLeave(fsm_do_nothing_func),
        mOnDestroy(fsm_do_nothing_func) {
        
        }
        
        FunctionFSMState(const InitiateFunc& init = fsm_do_nothing_func,
                         const UpdateFunc& update = fsm_do_nothing_func,
                         const EnterFunc& enter = fsm_do_nothing_func,
                         const LeaveFunc& leave = fsm_do_nothing_func,
                         const DestroyFunc& destroy = fsm_do_nothing_func):
        mOnInit(init),
        mOnUpdate(update),
        mOnEnter(enter),
        mOnLeave(leave),
        mOnDestroy(destroy) {
            
        }
        
        virtual ~FunctionFSMState() {
            
        }
        
        virtual void onInitiate() {
            if(mOnInit != fsm_do_nothing_func)
                mOnInit(*this);
        }
        
        virtual void onUpdate() {
            if(mOnUpdate != fsm_do_nothing_func)
                mOnUpdate(*this);
        }
        
        virtual void onEnter() {
            if(mOnEnter != fsm_do_nothing_func)
                mOnEnter(*this);
        }
        
        virtual void onLeave() {
            if(mOnLeave != fsm_do_nothing_func)
                mOnLeave(*this);
        }
        
        virtual void onDestroy() {
            if(mOnDestroy != fsm_do_nothing_func)
                mOnDestroy(*this);
        }
        
    private:
        InitiateFunc    mOnInit;
        UpdateFunc      mOnUpdate;
        EnterFunc       mOnEnter;
        LeaveFunc       mOnLeave;
        DestroyFunc     mOnDestroy;
    };
    
    class FSMManager {
    public:
        typedef int EventType;
        
        FSMManager(): 
        mGlobalState(0),
        mCurrentState(0),
        mPreviousState(0) {
        }
        
        ~FSMManager() {
            FSMStateMap::iterator itState = this->mStates.begin();
            while(itState != mStates.end()) {
                itState->second->destroy();
                itState->second->mManager = 0;
                ++itState;
            }
        }
        
        FSMManager& defState(FSMState* state, const ukn_string& name)  {
            this->mStates.insert(std::make_pair(name, state));
            state->onInitiate();
            state->mManager = this;
            return *this;
        }
        
        void delState(const ukn_string& name) {
            FSMStateMap::iterator itState = this->mStates.find(name);
            if(itState != this->mStates.end()) {
                itState->second->destroy();
                itState->second->mManager = NULL;
                this->mStates.erase(itState);
            }
        }
        
        bool defTrans(const ukn_string& state1, const EventType& event, const ukn_string& state2) {
            FSMStateMap::iterator itState1 = this->mStates.find(state1);
            FSMStateMap::iterator itState2 = this->mStates.find(state2);
            if(itState1 != this->mStates.end() &&
               itState2 != this->mStates.end()) {
                mTransitions[itState1->second].EventMap.insert(std::make_pair(event, itState2->second));
                return true;
            }
            return false;
        }
            
        void delTrans(const ukn_string& state, const EventType& event) {
            FSMStateMap::iterator itState = this->mStates.find(state);
            if(itState != this->mStates.end()) {
                StateTransitionMap::iterator itTrans = this->mTransitions.find(itState->second);
                if(itTrans != this->mTransitions.end()) {
                    StateEventMap::iterator itEvent = itTrans->second.EventMap.find(event);
                    if(itEvent != itTrans->second.EventMap.end()) {
                        itTrans->second.EventMap.erase(itEvent);
                    }
                }
            }
        }
        
        void postEvent(const EventType& event) {
            StateTransitionMap::iterator itTrans = this->mTransitions.find(mCurrentState);
            if(itTrans != this->mTransitions.end()) {
                StateEventMap::iterator itEvent = itTrans->second.EventMap.find(event);
                if(itEvent != itTrans->second.EventMap.end()) {
                    switchToState(itEvent->second);
                }
            }
        }
        
        void switchToState(const ukn_string& name) {
            FSMStateMap::iterator itState = this->mStates.find(name);
            if(itState != this->mStates.end()) {
                switchToState(itState->second);
            }
        }
        
        void setGlobalState(FSMState* state) {
            this->mGlobalState = state;
        }
        
        FSMState* getCurrentState() const {
            return this->mCurrentState;
        }
        FSMState* getPreviousState() const {
            return this->mPreviousState;
        }
        FSMState* getGlobalState() const {
            return this->mGlobalState;
        }
        
        void onUpdate() {
            if(this->mCurrentState)
                this->mCurrentState->onUpdate();
            if(this->mGlobalState)
                this->mGlobalState->onUpdate();
        }
                
        bool returnToPreviousState() {
            if(this->mPreviousState != NULL) {
                this->mCurrentState->onLeave();
                this->mCurrentState = mPreviousState;
                this->mCurrentState->onEnter();
                return true;
            }
            return false;
        }
                
        FSMState* operator[](const ukn_string& name) const {
            FSMStateMap::const_iterator itState = this->mStates.find(name);
            if(itState != this->mStates.end()) {
                return itState->second;
            }
            return NULL;
        }
        
    private:
        void switchToState(FSMState* state) {
            ukn_assert(state);
            
            if(this->mCurrentState != NULL) {
                this->mCurrentState->onLeave();
                this->mPreviousState = this->mCurrentState;
            }
            this->mCurrentState = state;
            if(this->mCurrentState)
                this->mCurrentState->onEnter();
        }
        
        typedef std::map<ukn_string, FSMState*> FSMStateMap;
        FSMStateMap mStates;
        
        typedef std::map<EventType, FSMState*> StateEventMap;
        struct EventTrans {
            StateEventMap EventMap;
        };
        typedef std::map<FSMState*, EventTrans> StateTransitionMap;
        StateTransitionMap mTransitions;
        
        FSMState* mGlobalState;
        FSMState* mCurrentState;
        FSMState* mPreviousState;
    };
    
} // namespace ukn  

#endif
