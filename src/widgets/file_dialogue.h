#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "text_field.h"
#include "file_menu_button.h"

class FileDialogue : public Widget{
public:
    std::vector<std::string> path_stack;
    TextField* file_name_text_field = nullptr;
    std::vector<FileMenuButton*> file_menu_buttons;
    
    FileDialogue(Vector2f _local_position, Vector2f size, std::string _directory) : Widget(_local_position, size)
    {
        path_stack.push_back(_directory);
    }

    virtual bool OnAddFolder(const std::string& _directory_name){
        return true;
    }

    virtual bool OnAddFile(const std::string& _file_name){
        return true;
    }

    void SetDirectory(std::string _path){

        for(const auto& directory_entry : std::filesystem::directory_iterator{_path}){
            Console::Out("Directory entry found");
    
            std::error_code ec;
            std::string s_path = std::string(directory_entry.path().string());
            if(std::filesystem::is_directory(directory_entry,ec)){
    
                std::string directory_name = s_path.substr(s_path.find_last_of("/")+1);
    
                if(OnAddFolder(directory_name)){
                    AddChild<FileDialogueButton>();
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

    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);
        
        file_name_text_field = AddChild<TextField>(Vector2f(0.0f, GetRectangle().size.y),Vector2f(200.0f,20.0f));
        file_name_text_field->text = "Hello Text Field";
    }

    void OnWidgetHovered(){
        for(const auto& file_menu_button : file_menu_buttons){

        }
    }
};