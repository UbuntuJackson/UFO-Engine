#pragma once

#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../actor/actor.h"

class Camera;
class AssetManager;

class AbstractSpriteReference : public Actor{
public:
    std::string key;
    olc::vf2d offset;
    olc::vf2d frame_size;
    olc::vf2d scale = {1.0f, 1.0f};
    float rotation = 0.0f;
    float current_frame_index = 0;
    int number_of_frames;
    bool visible = true;
    olc::Pixel tint = olc::WHITE;
    AssetManager* asset_manager = nullptr;

    AbstractSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation);
    
};