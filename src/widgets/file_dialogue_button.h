#pragma once
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../sprite_reference/widget_sprite_reference.h"
#include "../console/console.h"
#include "../ufo_clock/timer.h"
#include "../mouse/mouse.h"

class FileDialogueButton : public Widget{
public:
    Timer double_click_timer;
    WidgetSpriteReference* spr = nullptr;

    FileDialogueButton() : Widget(Vector2f(0.0f,0.0f),Vector2f(32.0f,32.0f)){}
    
    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);
        spr = AddChild<WidgetSpriteReference>(
            "UFOEngine/folder_icon",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(32.0f,32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0
        );
        visible = false;
    }

    void OnWidgetHovered(){
        if(double_click_timer.GetTimeLeft() < 0.0f){
            double_click_timer.Stop();
        }
        
        bool double_click_happened = false;
        if(Mouse::Get().GetLeftButton().is_pressed && double_click_timer.GetTimeLeft() >= 0.0f){
            OnDoubleClicked();
            double_click_timer.Stop();
            double_click_happened = true;
        }
        if(Mouse::Get().GetLeftButton().is_pressed && !double_click_timer.is_started && !double_click_happened){
            double_click_timer.Start(1000.0f);
        }
    }
    
    virtual void OnDoubleClicked(){
        Console::PrintLine("FileDialogueButton", "OnDoubleClicked");
    }
};