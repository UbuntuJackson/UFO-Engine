#ifndef BUTTON_H
#define BUTTON_H
#include <string>
#include <memory>
#include <functional>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"
#include "widget.h"
#include "colour_rectangle_theme.h"
#include "theme.h"

class Json;
class Level;

class Button : public Widget{
public:
    bool is_selected = false;
    bool active;
    std::string on_clicked;
    std::string on_create;
    std::string text;
    olc::Pixel text_colour = olc::WHITE;
    olc::Pixel background_colour = olc::Pixel(0,150,160);
    std::function<void(Widget* _parent_widget, Button* _button)> on_pressed = [](Widget* _parent_widget, Button* _button){};
    std::function<void(Level*)> on_released = [](Level* _level){};
    std::function<void(Level*)> on_held = [](Level* _level){};
    bool refreshed = false;
    bool needs_to_refresh = false;
    Button(olc::vf2d _local_position, olc::vf2d _size, std::string _text = "", std::string on_create = "", std::string on_clicked = "");
    void OnLevelEnter(Level* _level);
    void OnSetup(Level* _level);
    void Refresh();
    bool on_widget_hovered_this_frame = false;
    void OnWidgetHovered();
    void OnUpdate();
    void OnPaused();
    bool IsPressed();
    bool IsHeld();
    bool IsReleased();
    bool IsHovered();
    void OnResetWidget();

    float font_height = 8.0f;

    std::unique_ptr<Theme> hovered_theme = std::make_unique<ColourRectangleTheme>(olc::CYAN);
    std::unique_ptr<Theme> held_theme = std::make_unique<ColourRectangleTheme>(olc::VERY_DARK_CYAN);

    void OnWidgetDraw();

    //Unused
    static std::unique_ptr<Button> Load(Json* _json);
};

#endif