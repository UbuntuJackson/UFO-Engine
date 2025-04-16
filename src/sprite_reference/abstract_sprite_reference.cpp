#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../actor/actor.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"
#include "abstract_sprite_reference.h"

AbstractSpriteReference::AbstractSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation) :
    key{_key},
    Actor(_position),
    offset{_offset},
    frame_size{_frame_size},
    scale{_scale},
    rotation{_rotation}
    {
        
    }