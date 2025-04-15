#pragma once
#include "file_dialogue_button.h"

class FileDialogue;

class FileDialogueFileButton : public FileDialogueButton{
public:
    FileDialogueFileButton(FileDialogue* _file_dialogue);

    void OnLevelEnter(Level* _level);

    void OnDoubleClicked(FileDialogue* _fd);
};