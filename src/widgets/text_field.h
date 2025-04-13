#pragma once
#include "label.h"
#include "../ufo_maths/ufo_maths.h"
#include "../shapes/rectangle.h"
#include "../mouse/mouse.h"
#include "../ufo_engine/ufo_engine.h"

class TextField : public Label{
public:

    TextField(Vector2f _local_position, Vector2f _size) : Label(_local_position, _size){

    }
    void OnWidgetHovered(){
        if(ufoMaths::RectangleVsPoint(GetRectangle(), Mouse::Get().GetPosition()) && Mouse::Get().GetLeftButton().is_pressed){
            Engine::Get().pixel_game_engine.TextEntryEnable(true);
        }
        if(!ufoMaths::RectangleVsPoint(GetRectangle(), Mouse::Get().GetPosition()) && Mouse::Get().GetLeftButton().is_pressed){
            Engine::Get().pixel_game_engine.TextEntryEnable(false);
        }
        if(Engine::Get().pixel_game_engine.IsTextEntryEnabled()){
            text = Engine::Get().pixel_game_engine.TextEntryGetString();
        }
    }

};