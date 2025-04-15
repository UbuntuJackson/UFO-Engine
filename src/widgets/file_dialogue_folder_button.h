#pragma once
#include "file_dialogue_button.h"
#include "../level/level.h"

class FileDialogue;

class FileDialogueFolderButton : public FileDialogueButton{
private:
    Level* level = nullptr;
public:
    FileDialogueFolderButton();

    void OnLevelEnter(Level* _level);

    void OnDoubleClick(FileDialogue* _fd);
};