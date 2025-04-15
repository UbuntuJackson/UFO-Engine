#include <string>
#include "button.h"
#include "widget.h"
#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"
#include "../graphics/graphics.h"

Button::Button(
    olc::vf2d _local_position, olc::vf2d _size,
    std::string _text,
    std::string _on_create,
    std::string _on_clicked
) :
Widget(_local_position, _size),
on_create{_on_create},
on_clicked{_on_clicked},
text{_text}
{
    
}

void Button::OnLevelEnter(Level* _level){
    Widget::OnLevelEnter(_level);
    Console::Print("Button Onstart Ran\n");
    Refresh();
}

void Button::OnSetup(Level* _level){
    Refresh();
}

void Button::Refresh(){
    if(adjust_height_after_text_rows){
        float height_of_text_chunk = GetWrappedText(text).rows * font_height;
        if(height_of_text_chunk > GetRectangle().size.y) rectangle.size.y = height_of_text_chunk;
    }
}

void Button::OnWidgetHovered(){
    on_widget_hovered_this_frame = true;
}

void Button::OnUpdate(){
    Widget::OnUpdate();
    Refresh();
}

void Button::OnPaused(){
    Widget::OnPaused();
    Refresh();
}

bool Button::IsPressed(){
    if(!IsHovered()) return false;
    if(Mouse::GetLeftButton().is_pressed) return true;
    return false;
}
bool Button::IsHovered(){
    return on_widget_hovered_this_frame;
}
bool Button::IsHeld(){
    if(!IsHovered()) return false;
    if(Mouse::GetLeftButton().is_held) return true;
    return false;
}
bool Button::IsReleased(){
    if(!IsHovered()) return false;
    if(Mouse::GetLeftButton().is_released) return true;
    return false;
}

void Button::OnResetWidget(){
    on_widget_hovered_this_frame = false;
}

void Button::OnWidgetDraw(){
    if(!visible) return;
        
    if(!IsHovered() && !IsHeld()) theme->OnDraw(this);
    if((IsHovered() && !IsHeld()) || is_selected) hovered_theme->OnDraw(this);
    if(IsHeld()) held_theme->OnDraw(this);
    Graphics::Get().DrawString(GetGlobalPosition(), GetWrappedText(text).text, text_colour, {1.0f,1.0f});
    
}

std::unique_ptr<Button> Button::Load(Json* _json){
    std::string on_click = "";
    std::string on_create = "";
    float x;
    float y;
    float width;
    float height;
    std::string text = "";

    Json property_json = _json->GetObject("properties");
    Json::ArrayForEach(Json(), property_json, [&](Json _json_b, Json _data_b){
        if(property_json.member){
            if(_json_b.GetAsString("name") == "on_click"){
                on_click = _json_b.GetAsString("value");
                Console::Out(on_click);
            }
            if(_json_b.GetAsString("name") == "on_create"){
                on_create = _json_b.GetAsString("value");
                Console::Out(on_create);
            }
            if(_json_b.GetAsString("name") == "text"){
                text = _json_b.GetAsString("value");
            }
        }
    }
    );
    
    x = _json->GetAsInt("x");
    y = _json->GetAsInt("y");
    width = _json->GetAsInt("width");
    height = _json->GetAsInt("height");

    return std::make_unique<Button>(olc::vf2d(x,y),olc::vf2d(width,height),on_create,on_click,text);
}