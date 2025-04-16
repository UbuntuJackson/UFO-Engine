#pragma once
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"
#include "button.h"

class FileDialoguePathStack : public Widget{
    std::vector<Button*> path_buttons;

    FileDialoguePathStack(Vector2f _size) : Widget(Vector2f(0.0f,0.0f), _size){

    }

    void OnWidgetHovered(){
        for(const auto& button : path_buttons){
            if(button->IsReleased()){
                
            }
        }
    }
};