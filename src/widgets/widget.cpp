#include <memory>
#include "widget.h"
#include "../shapes/rectangle.h"
#include "../json/json.h"
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../drawing_system/drawing_system.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/level.h"
#include "../mouse/mouse.h"

using namespace ufoMaths;

ufo::Rectangle
Widget::GetRectangle(){
    return ufo::Rectangle(GetGlobalPosition(), rectangle.size);
}

Widget::Widget(Vector2f _local_position, Vector2f _size) :
    Actor(_local_position), rectangle(_local_position, _size){}

void Widget::OnLevelEnter(Level* _level){
    Actor::OnLevelEnter(_level);
    if(!parent) _level->widget_handles.push_back(this);

}

WrappedText Widget::GetWrappedTextWrapOnSpace(std::string _text){

    int text_width = 8;

    std::vector<std::string> text_as_vector;

    std::string word = "";

    for(int c = 0; c < _text.size(); c++){
        
        word += _text[c];

        if(c == _text.size()-1 || _text[c] == ' ' || _text[c] == '\n'){
            text_as_vector.push_back(word);
            word = "";
        }
    }

    /*Console::PrintLine("GetWrappedTextWrapOnSpace:");
    for(const auto str : text_as_vector){
        Console::Print(str,",");
    }
    Console::Print("\n");*/

    std::string new_text = "";

    std::string row = "";

    int number_of_rows = 1;

    for(int i_word = 0; i_word < text_as_vector.size(); i_word++){
        

        if(row.size()+text_as_vector[i_word].size() < int(rectangle.size.x/text_width)){
            if(text_as_vector[i_word][text_as_vector[i_word].size()-1] != '\n'){
                row+=text_as_vector[i_word];
            }
            else{
                number_of_rows++;
                row+=text_as_vector[i_word];
                //Console::PrintLine("if Row:",row, "comparison:", row.size()+text_as_vector[i_word].size(), int(rectangle.size.x/text_width));
                new_text+=row;
                row = "";
            }
        }
        else{
            number_of_rows++;
            //Console::PrintLine("else Row:",row);
            //Add all previous text to the row
            new_text+=(row+"\n");
            row = "";
            row+=text_as_vector[i_word];

            if(text_as_vector[i_word][text_as_vector[i_word].size()-1] == '\n'){
                number_of_rows++;
                
                //Console::PrintLine("else if Row:",row, "comparison:", row.size()+text_as_vector[i_word].size(), int(rectangle.size.x/text_width));
                new_text+=row;
                row = "";
            }
        }
    }
    new_text+=row;
    
    return {new_text, number_of_rows};
}

WrappedText Widget::GetWrappedTextWrapOnCharMeetsBorder(std::string _text){
    int text_width = 8;
    std::string new_text = "";
    int number_of_rows = 0;

    for(int c = 0; c < _text.size(); c++){
        if(c % int(rectangle.size.x/text_width) == int(rectangle.size.x/text_width)-1){
            new_text+="\n";
            number_of_rows++;
        }
        new_text+=_text[c];

        //Console::Out((c*text_width)%int(rectangle.size.x));
    }

    return {new_text, number_of_rows};
}

WrappedText Widget::GetWrappedText(std::string _text){
    if(text_wrapping_mode == TextWrappingModes::CHAR_MEETS_BORDER){
        return GetWrappedTextWrapOnCharMeetsBorder(_text);
    }
    if(text_wrapping_mode == TextWrappingModes::WORD_MEETS_BORDER){
        return GetWrappedTextWrapOnSpace(_text);
    }
    return WrappedText{_text,1};
}

void Widget::OnUpdate(){
    Actor::OnUpdate();
    
}

bool Widget::SearchForHoveredWidget(){
    for(int index = widget_handles.size()-1; index >= 0; index--){
        bool found_hovered_widget = widget_handles[index]->SearchForHoveredWidget();
        if(found_hovered_widget) return true;
    }
    if(ufoMaths::RectangleVsPoint(GetRectangle(), Mouse::Get().GetPosition())){
        OnWidgetHovered();
        return true;
    }
    return false;
}

void Widget::OnWidgetHovered(){

}

void Widget::OnActiveUpdate(){

}

void Widget::OnSetInactive(){

}

void Widget::OnSetActive(){

}

void Widget::ResetWidget(){
    for(const auto& widget : widget_handles) widget->ResetWidget();
    OnResetWidget();
}
void Widget::OnResetWidget(){

}

void Widget::OnWidgetDraw(){
    if(!visible) return;
    theme->OnDraw(this);
    
}

void Widget::OnPurgeDeadActors(){
    Console::PrintLine("Widget::OnPurgeDeadActors");
    Engine::Get().current_level->PurgeHandles(widget_handles);
}

void Widget::OnPurge(Level* _level){
    if(_level->GetActiveWidget() == this){
        _level->SetActiveWidget(nullptr);
    }
}