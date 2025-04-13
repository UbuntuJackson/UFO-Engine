#pragma once

class Widget;

class WidgetStateMachine{
public:

    //The single active widget until something else gets clicked
    Widget* active_widget = nullptr;
    
};