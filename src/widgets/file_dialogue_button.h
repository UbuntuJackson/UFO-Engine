#pragma once
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../console/console.h"
#include "../ufo_clock/timer.h"
#include "../mouse/mouse.h"

class WidgetSpriteReference;
class TextField;
class FileDialogue;
class Level;

class FileDialogueButton : public Widget{
public:
    Timer double_click_timer;
    WidgetSpriteReference* spr = nullptr;
    TextField* tf_file_name = nullptr;
    std::string path;
    FileDialogue* file_dialogue = nullptr;

    FileDialogueButton(FileDialogue* _file_dialogue);
    
    void OnLevelEnter(Level* _level);

    void OnWidgetHovered();
    
    virtual void OnDoubleClicked(FileDialogue* _fd);
};