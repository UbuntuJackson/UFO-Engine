#pragma once

class Widget;

class Theme{
public:

    Theme() = default;

    virtual void OnDraw(Widget* _widget);
};