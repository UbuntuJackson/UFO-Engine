#include "file_dialogue_folder_button.h"
#include "file_dialogue.h"
#include "text_field.h"
#include "file_dialogue.h"

FileDialogueFolderButton::FileDialogueFolderButton(FileDialogue* _file_dialogue) : FileDialogueButton(_file_dialogue){

}

void FileDialogueFolderButton::OnLevelEnter(Level* _level){
    FileDialogueButton::OnLevelEnter(_level);
    level = _level;

}

void FileDialogueFolderButton::OnDoubleClick(FileDialogue* _fd){
    _fd->OnOpenFolder(tf_file_name->text,path);
}