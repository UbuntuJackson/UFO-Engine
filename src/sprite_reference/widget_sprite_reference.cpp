#include <string>
#include "../external/olcPixelGameEngine.h"
#include "widget_sprite_reference.h"
#include "../actor/actor.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"

WidgetSpriteReference::WidgetSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation) :
AbstractSpriteReference(_key, _position,_offset,_frame_size, _scale, _rotation)
    {
        asset_manager = &AssetManager::Get();
        number_of_frames = asset_manager->GetDecal(_key)->sprite->Size().x/(int)_frame_size.x * asset_manager->GetDecal(_key)->sprite->Size().y/(int)_frame_size.y;
    }

void
WidgetSpriteReference::OnWidgetDraw(){
    if(!visible) return;
    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(key,current_frame_index,frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        GetGlobalPosition(),
        asset_manager->GetDecal(key),
        rotation,
        offset,
        sample_rectangle.position,
        sample_rectangle.size,
        scale,
        tint
    ); 
}