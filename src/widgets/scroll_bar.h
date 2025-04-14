#pragma once
#include "widget.h"
#include "../mouse/mouse.h"
#include "../graphics/graphics.h"
#include "../external/olcPixelGameEngine.h"
class ScrollBar : public Widget{
public:
    float scroll_percentage = 0.0f;
    float visible_portion_percentage = 1.0f;
    ScrollBar(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){
        
    }

    void OnWidgetHovered(){
        if(Mouse::Get().GetLeftButton().is_held){
            scroll_percentage = (Mouse::Get().GetPosition()-GetGlobalPosition()).y / GetRectangle().size.y;
        }
    }

    void OnWidgetDraw(){
        Graphics::Get().DrawRectangle(
            GetGlobalPosition()+Vector2f(0.0f,scroll_percentage*GetRectangle().size.y),
            Vector2f(16.0f,visible_portion_percentage*GetRectangle().size.y), olc::BLACK);
    }

};