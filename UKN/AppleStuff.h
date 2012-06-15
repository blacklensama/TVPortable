//
//  StringUtilApple.h
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_StringUtilApple_h
#define Project_Unknown_StringUtilApple_h

#include "UKN/Platform.h"
#include "UKN/SysUtil.h"

namespace ukn {
    
    // ios and osx wrapper functions
     
    ukn_wstring ukn_apple_string_to_wstring(const ukn_string& str);
    ukn_string ukn_apple_wstring_to_string(const ukn_wstring& str);
    
    bool ukn_apple_file_exists(const ukn_wstring& file);
    ukn_wstring ukn_apple_application_path();
    ukn_wstring ukn_apple_documents_path();
    ukn_wstring ukn_apple_resource_path();
    
    uint64 ukn_apple_get_processor_speed();
    uint64 ukn_apple_get_memory_size();
    ukn_string ukn_apple_get_os_version();
    
    MessageBoxButton ukn_apple_message_box(const ukn_string& mssg, const ukn_string& title, int option);
    MessageBoxButton ukn_apple_message_box(const ukn_wstring& mssg, const ukn_wstring& title, int option);
    
    void ukn_apple_enum_desktop_modes(Array<SystemInformation::DesktopMode>& arr);
    
#ifdef UKN_OS_OSX
    int ukn_get_os_osx_version();

#elif defined(UKN_OS_IOS)
    
    enum iOSDeviceType {
        IDT_Old,    // "old" devices such as ipod touch 3rd gen or iphone 3gs, resolution 480*320
        IDT_Retina, // newer devices with a retina display, resolution 960 * 640
        IDT_iPad,   // iPad with 1024 * 768 resolution
    };
    
    iOSDeviceType ukn_get_ios_device_type();
    
    bool ukn_ios_is_ipad();
    bool ukn_ios_is_retina_display();
    
    uint32 ukn_ios_get_screen_width();
    uint32 ukn_ios_get_screen_height();
    
    float ukn_ios_get_scale_factor();
    
    void ukn_ios_vabrite_device();
    
    // must be called before initiliaze video device
    // if use retina on a retina display device, content size will be 960 * 640
    //  you can get a larger screen size but less compability
    // otherwise will use the same content size as old devices without a retina display(480 * 320)
    //  and scale the resources or load resources with @2x postfix when available
    void ukn_ios_set_use_retina(bool flag);
    bool ukn_ios_is_use_retina();
    
#endif 
    
} // namespace ukn


#endif
