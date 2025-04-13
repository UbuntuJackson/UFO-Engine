#ifndef WRAP_MENU_H
#define WRAP_MENU_H

#include <memory>
#include <vector>
#include "../mouse/mouse.h"
#include "../widgets/widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../widgets/button.h"
#include "../level/level.h"
#include "../graphics/graphics.h"
#include "../external/olcPixelGameEngine.h"
#include "../keyboard/single_keyboard.h"

using namespace ufoMaths;

class WrapMenu : public Widget{
public:
    std::vector<Button*> buttons;

    Vector2f original_position;

    int selected_index = 0;
    int spacing = 1;

    bool has_modified_controls = false;

    WrapMenu(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){

    }

    void OnAddChild(Actor* _actor){
        Widget::OnAddChild(_actor);
        Button* b = dynamic_cast<Button*>(_actor);
        if(b != nullptr) buttons.push_back(b);

        Refresh();
    }

    void OnPurgeDeadActors(){
        Widget::OnPurgeDeadActors();
        Engine::Get().current_level->PurgeHandles(buttons);
    }

    void OnSetup(Level* _level){
        Refresh();
    }

    void ControlWithKeys(bool _up, bool _down, bool _select){

        if(_up){
            selected_index--;
            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));

        }
        if(_down){
            selected_index++;

            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));

        }
        if(_select){
            buttons[selected_index]->on_pressed(this, buttons[selected_index]);
        }

        if(buttons[selected_index]->GetGlobalPosition().y + spacing > Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y){
            Console::PrintLine("Button was on line", buttons[selected_index]->GetGlobalPosition().y);
            local_position.y -= (buttons[selected_index]->GetRectangle().size.y + spacing);
            
        }

        if(buttons[selected_index]->GetGlobalPosition().y - spacing < 0.0f){
            Console::PrintLine("Button was on line", buttons[selected_index]->GetGlobalPosition().y);
            local_position.y += (buttons[selected_index]->GetRectangle().size.y - spacing);
        }
    }

    void ControlWithMouse(){
        for(int index = 0; index < buttons.size(); index++){
            if(buttons[index]->IsPressed()){
                selected_index = index;
                buttons[index]->on_pressed(this, buttons[index]); 
            }
        }
    }

    void Refresh(){
        float total_height = 0.0f;

        for(const auto& button : buttons){
            button->Refresh();
        }

        for(const auto& button : buttons){
            button->local_position.y = total_height;
            total_height+=button->rectangle.size.y;
            total_height+=spacing;
        }

        rectangle.size.y = total_height;
        rectangle.size.x = 0.0f;
    }

    void OnUpdate(){
        //Adjusting size and positioning of buttons
        Refresh();

        //Selection logic
        for(auto&& button : buttons){
            button->is_selected = false;    
        }

        buttons[selected_index]->is_selected = true;
        //Engine::Get().current_level->QueueForPurge(id);
        if(has_modified_controls) return;

        ControlWithKeys(
            SingleKeyboard::Get().GetKey(olc::UP).is_pressed,
            SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed, 
            SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed);
        
        ControlWithMouse();
    }

    void OnPurge(Level *_level){
        
    }

    void OnWidgetDraw(){
        Widget::OnWidgetDraw();
        Graphics::Get().DrawString(buttons[selected_index]->GetGlobalPosition()-Vector2f(16.0f,0.0f),">",Graphics::WHITE,{1.0f,1.0f});
    }

};

#endif