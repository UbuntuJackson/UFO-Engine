#pragma once
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../sprite_reference/widget_sprite_reference.h"
#include "../console/console.h"
#include "../ufo_clock/timer.h"
#include "../mouse/mouse.h"

class Label;
class FileDialogue;

class FileDialogueButton : public Widget{
public:
    Timer double_click_timer;
    WidgetSpriteReference* spr = nullptr;
    TextField* tf_file_name = nullptr;
    std::string path;

    FileDialogueButton();
    
    void OnLevelEnter(Level* _level);

    void OnWidgetHovered();
    
    virtual void OnDoubleClicked(FileDialogue* _fd);
};