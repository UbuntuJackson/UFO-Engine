#include "file_dialogue_file_button.h"
#include "file_dialogue_button.h"
#include "file_dialogue.h"
#include "../level/level.h"
#include "../sprite_reference/widget_sprite_reference.h"

FileDialogueFileButton::FileDialogueFileButton(FileDialogue* _file_dialogue) : FileDialogueButton(_file_dialogue){

}

void FileDialogueFileButton::OnLevelEnter(Level* _level){
    Widget::OnLevelEnter(_level);
    spr->key = "UFOEngine/file_icon";
    visible = false;

    tf_file_name = AddChild<TextField>(Vector2f(-75.0f+15.0f,32.0f),Vector2f(150.0f,32.0f));
}

void FileDialogueFileButton::OnDoubleClick(FileDialogue* _fd){
    
}