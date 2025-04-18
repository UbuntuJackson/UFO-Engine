#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_engine/ufo_engine.h"

class Button;
class ScrollBar;
class TextField;
class FileDialogueButton;

class FileDialogue : public Widget{
public:
    enum Modes{
        WRITE,
        READ,
    };

    std::vector<std::string> path_stack;
    TextField* file_name_text_field = nullptr;
    std::vector<FileDialogueButton*> file_dialogue_buttons;

    Button* b_save = nullptr;
    Button* b_cancel = nullptr;

    ScrollBar* scroll_bar = nullptr;

    Modes file_dialogue_mode = Modes::WRITE;
    
    FileDialogue(Modes _file_dialogue_mode, Vector2f _local_position, Vector2f size, std::string _directory);

    virtual bool OnAddFolder(const std::string& _directory_name);

    virtual bool OnAddFile(const std::string& _file_name);

    void SetDirectory(std::string _path);

    void OnLevelEnter(Level* _level);

    void OnWidgetHovered();

    void OnPurgeDeadActors();

    void OnUpdate();

    virtual void OnOpenFolder(const std::string& _file_name, const std::string& _path);
    virtual void OnOpenFile(const std::string& _file_name, const std::string& _path);
};