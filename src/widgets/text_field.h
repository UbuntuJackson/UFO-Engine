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
    void OnSetActive(){
        Engine::Get().pixel_game_engine.TextEntryEnable(true);
    }

    void OnWidgetHovered(){
        Console::PrintLine("TextField::OnWidgetHovered");
        if(Mouse::Get().GetLeftButton().is_pressed){
            Engine::Get().current_level->SetActiveWidget(this);
        }
    }

    void OnSetInactive(){
        Engine::Get().pixel_game_engine.TextEntryEnable(false);
    }

    void OnActiveUpdate(){
        if(Engine::Get().pixel_game_engine.IsTextEntryEnabled()){
            text = Engine::Get().pixel_game_engine.TextEntryGetString();
        }
    }

};