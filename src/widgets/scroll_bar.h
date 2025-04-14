#pragma once
#include "widget.h"
#include "../mouse/mouse.h"
class ScrollBar : public Widget{
public:
    float scroll_percentage = 0.0f;
    float visible_portion_percentage = 0.0f;
    ScrollBar(float _visible_portion_percentage, Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){
        visible_portion_percentage = _visible_portion_percentage;
    }

    void OnWidgetHovered(){
        if(Mouse::Get().GetLeftButton().is_held){
            
        }
    }

};