//
//  ScriptManager.h
//  TVPortable
//
//  Created by Mili Zhang on 7/19/12.
//
//

// TODO: Work in progress.

#ifndef TVPortable_ScriptManager_h
#define TVPortable_ScriptManager_h

TVP_NS_CORE_BEGIN

class ScriptManager
{

protected:
    ScriptManager();
    
public:
    tTJSVarient* eval();
    tTJSVarient* evalStorage();
    void exec();
    void execStorage();
    
    void dump();
    
    tTJS* getTJSEngine();
    iTJSDispatch2* getGlobal();

    static void Initialize()
    {
        instance = new ScriptManager();
    }
    
    static void Release()
    {
        delete instance;
        instance = 0;
    }
    
    static ScriptManager& Instance()
    {
        return *instance;
    }
    
    
private:
    tTJS* tjsEngine;
    
    static ScriptManager* instance;
}

TVP_NS_CORE_END

#endif
