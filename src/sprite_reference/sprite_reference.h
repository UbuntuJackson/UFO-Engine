#ifndef SPRITE_REFERENCE_H
#define SPRITE_REFERENCE_H
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "abstract_sprite_reference.h"

class Camera;

class SpriteReference : public AbstractSpriteReference{
public:

    SpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation);

    void OnDraw(Camera*);
    
};

#endif