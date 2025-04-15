#pragma once
#include "file_dialogue_button.h"

class FileDialogue;

class FileDialogueFileButton : public FileDialogueButton{
public:
    FileDialogueFileButton();

    void OnLevelEnter(Level* _level);

    void OnDoubleClick(FileDialogue* _fd);
};