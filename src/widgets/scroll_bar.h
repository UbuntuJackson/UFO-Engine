#pragma once
#include "widget.h"
#include "../mouse/mouse.h"
#include "../graphics/graphics.h"
#include "../external/olcPixelGameEngine.h"
class ScrollBar : public Widget{
public:
    float scroll_percentage = 0.0f;
    float visible_portion_percentage = 1.0f;
    float scroll_bar_grab_y = 0.0f;
    ScrollBar(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){
        
    }

    void OnWidgetHovered(){
        if(Mouse::Get().GetLeftButton().is_pressed){
            scroll_bar_grab_y = Mouse::Get().GetPosition().y - (GetGlobalPosition().y + scroll_percentage*GetRectangle().size.y);
        }
        if(Mouse::Get().GetLeftButton().is_held){
            scroll_percentage = (Mouse::Get().GetPosition()-(GetGlobalPosition()+Vector2f(0.0f,scroll_bar_grab_y))).y / GetRectangle().size.y;
        
        }
        if(scroll_percentage > 1.0f - visible_portion_percentage) scroll_percentage = 1.0f - visible_portion_percentage;
        if(scroll_percentage < 0.0f) scroll_percentage = 0.0f;
    }

    void OnWidgetDraw(){
        Graphics::Get().DrawRectangle(
            GetGlobalPosition()+Vector2f(0.0f,scroll_percentage*GetRectangle().size.y),
            Vector2f(16.0f,visible_portion_percentage*GetRectangle().size.y), olc::DARK_CYAN);
    }

};