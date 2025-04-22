#pragma once
#include <string>
#include "label.h"
#include "../mouse/mouse.h"
#include "../sprite_reference/widget_sprite_reference.h"

class CheckBox : public Label{
public:
    bool checked = false;
    std::string text_ = "";

    CheckBox(Vector2f _local_position, Vector2f _size, std::string _text_) : Label(_local_position, _size),text_{_text_}{}

    void OnWidgetHovered(){
        if(Mouse::Get().GetLeftButton().is_pressed){
            checked = !checked;
            
            if(checked) text = text_ + "[x]";
            else text = text_ + "[ ]";
        }
    }

};