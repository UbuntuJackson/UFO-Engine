#include <string>
#include "../external/olcPixelGameEngine.h"
#include "sprite_reference.h"
#include "../actor/actor.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"
#include "abstract_sprite_reference.h"

SpriteReference::SpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation) :
    AbstractSpriteReference(_key, _position,_offset,_frame_size, _scale, _rotation)
    {
        asset_manager = &AssetManager::Get();
        number_of_frames = asset_manager->GetDecal(_key)->sprite->Size().x/(int)_frame_size.x * asset_manager->GetDecal(_key)->sprite->Size().y/(int)_frame_size.y;
    }

void SpriteReference::OnDraw(Camera* _camera){

    if(!visible) return;
    ufo::Rectangle sample_rectangle = asset_manager->GetFrameFromSpriteSheet(key,current_frame_index,frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        _camera->Transform(GetGlobalPosition()),
        asset_manager->GetDecal(key),
        rotation,
        offset,
        sample_rectangle.position,
        sample_rectangle.size,
        scale*_camera->scale,
        tint
    );

}