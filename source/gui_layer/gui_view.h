#pragma once
#include "../global.h"



class GuiView {

public:
//needs testing
/*
    inline FunctionSink<void(int,float,float)> MouseButtonPressed(){
        return FunctionSink<void(int,float,float)>(m_MouseButtonPressedEvent);
    }
    inline FunctionSink<void(int,float,float)> MouseButtonReleased() {
        return FunctionSink<void(int,float,float)>(m_MouseButtonPressedEvent);
    }
    inline FunctionSink<void(int)> KeyDown(){
        return FunctionSink<void(int)>(m_KeyDownEvent);
    }
    inline FunctionSink<void(int)> KeyUp(){
        return FunctionSink<void(int)>(m_KeyUpEvent);
    }
*/

protected:

    

    EventLauncher<void(int,float,float)> m_MouseButtonPressedEvent;
    EventLauncher<void(int,float,float)> m_MouseButtonReleasedEvent;
    EventLauncher<void(int)> m_KeyDownEvent;
    EventLauncher<void(int)> m_KeyUpEvent;

};