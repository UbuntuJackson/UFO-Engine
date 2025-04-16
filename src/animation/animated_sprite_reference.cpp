#include "animated_sprite_reference.h"

AnimatedSpriteReference::AnimatedSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, float _pace) :
    SpriteReference(_key, _position, _offset, _frame_size, _scale, _rotation),
    pace{_pace}
    {}