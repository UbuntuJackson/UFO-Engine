#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "text_field.h"
#include "file_dialogue_folder_button.h"
#include "../ufo_engine/ufo_engine.h"
#include "scroll_bar.h"
#include "button.h"
#include "file_dialogue.h"
#include "file_dialogue_file_button.h"
#include "../sprite_reference/widget_sprite_reference.h"

FileDialogue::FileDialogue(Vector2f _local_position, Vector2f size, std::string _directory) : Widget(_local_position, size)
{
    path_stack.push_back(_directory);
}

bool FileDialogue::OnAddFolder(const std::string& _directory_name){
    return true;
}

bool FileDialogue::OnAddFile(const std::string& _file_name){
    return true;
}

void FileDialogue::SetDirectory(std::string _path){
    for(const auto& wid : widget_handles) wid->QueueForPurge();

    for(const auto& directory_entry : std::filesystem::directory_iterator{_path}){

        std::error_code ec;
        std::string s_path = std::string(directory_entry.path().string());
        if(std::filesystem::is_directory(directory_entry,ec)){

            std::string directory_name = s_path.substr(s_path.find_last_of("/")+1);

            if(OnAddFolder(directory_name)){
                auto b_folder = AddChild<FileDialogueFolderButton>(this);
                b_folder->tf_file_name->text = directory_name;
                file_dialogue_buttons.push_back(b_folder);
                b_folder->path = path_stack.back()+"/"+directory_name;
            }

            //auto b = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 20.0f), directory_name, directory_entry.path().string());
            //b->background_colour = olc::Pixel(150,150,80);

            /*b->on_pressed = [&](Widget* _w, Button* _button){

                auto fm = dynamic_cast<FileMenu*>(_w);
                fm->path_stack.push_back(fm->path);

                fm->SetDirectory(dynamic_cast<FileMenuButton*>(_button)->path);
            };*/

            //AddChild(std::move(b));
        }
        else{
            Console::PrintLine("is file");

            std::string file_name = s_path.substr(s_path.find_last_of("/")+1);
            
            if(OnAddFile(file_name)){
                auto b_file = AddChild<FileDialogueFileButton>(this);
                b_file->tf_file_name->text = file_name;
                file_dialogue_buttons.push_back(b_file);
                b_file->path = path_stack.back()+"/"+file_name;
            }

            //auto b = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 20.0f),file_name, directory_entry.path().string());
            //b->background_colour = olc::Pixel(100,100,100);
            
            /*b->on_pressed = [&](Widget* _w, Button* _button){

                auto fm = dynamic_cast<FileMenu*>(_w);

                std::string file_path = dynamic_cast<FileMenuButton*>(_button)->path;
                fm->func_on_file_open(_w, dynamic_cast<FileMenuButton*>(_button), file_path.substr(file_path.find_last_of(".")+1));
                fm->OnFileOpen(dynamic_cast<FileMenuButton*>(_button), file_path.substr(file_path.find_last_of(".")+1));
            };*/
            
            //AddChild(std::move(b));
        }

    }
}

void FileDialogue::OnLevelEnter(Level* _level){
    Widget::OnLevelEnter(_level);

    file_name_text_field = AddChild<TextField>(Vector2f(0.0f, GetRectangle().size.y-30.0f),Vector2f(200.0f,15.0f));
    file_name_text_field->text = "Hello Text Field";
    SetDirectory(path_stack.back());

    scroll_bar = AddChild<ScrollBar>(Vector2f(GetRectangle().size.x-16.0f,0.0f),Vector2f(16.0f,GetRectangle().size.y));

    b_cancel = AddChild<Button>(Vector2f(0.0f, GetRectangle().size.y-15.0f),Vector2f(64.0f,15.0f), "Cancel");
    AddChild<Button>(Vector2f(64.0f, GetRectangle().size.y-15.0f),Vector2f(64.0f,15.0f), "Save");

    dynamic_cast<ColourRectangleTheme*>(theme.get())->colour = Colour(0,0,0,220);
}

void FileDialogue::OnWidgetHovered(){
    for(const auto& file_menu_button : file_dialogue_buttons){

    }
}

void FileDialogue::OnPurgeDeadActors(){
    Engine::Get().current_level->PurgeHandles(file_dialogue_buttons);
}

void FileDialogue::OnUpdate(){
    int rows = 0;

    float horizontal_spacing = 64.0f*3.0f;
    float vertical_spacing = 64.0f*1.5f;

    float file_x_position = 32.0f;
    for(int index = 0; index < file_dialogue_buttons.size(); index++){
        if(file_x_position > GetRectangle().size.x-32.0f){
            rows++;
            file_x_position = 32.0f;
        }

        file_dialogue_buttons[index]->local_position.x = file_x_position;
        file_dialogue_buttons[index]->local_position.y = rows*vertical_spacing+32.0f;

        file_x_position += horizontal_spacing;

    }

    float total_page_height = rows*vertical_spacing+64.0f;

    if(total_page_height > GetRectangle().size.y){
        Console::PrintLine(total_page_height);
        scroll_bar->visible_portion_percentage = (GetRectangle().size.y - 64.0f - 30.0f) / total_page_height;
        
        Console::PrintLine("FileDialogue visible_portion_percentage",(GetRectangle().size.y) / total_page_height);
    }

    for(int index = 0; index < file_dialogue_buttons.size(); index++){
        file_dialogue_buttons[index]->local_position.y -= total_page_height*scroll_bar->scroll_percentage;

        if(file_dialogue_buttons[index]->local_position.y > GetRectangle().size.y - 32.0f - 30.0f ||
        file_dialogue_buttons[index]->local_position.y < 32.0f
        ){
            file_dialogue_buttons[index]->spr->visible = false;
        }
        else{
            file_dialogue_buttons[index]->spr->visible = true;
        }
    }

    if(b_cancel->IsReleased()) QueueForPurge();

}

void FileDialogue::OnOpenFolder(const std::string& _file_name, const std::string& _path){
    path_stack.push_back(_path);
    SetDirectory(_path);
    Console::PrintLine("FileDialogue::OnOpenFolder",_file_name,_path);
}

void FileDialogue::OnOpenFile(const std::string& _file_name, const std::string& _path){
    
    Console::PrintLine("FileDialogue::OnOpenFile",_file_name,_path);
}