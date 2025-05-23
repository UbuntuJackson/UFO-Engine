#include "bitmap_collision_body.h"
#include "../level/level.h"
#include "colour_utils.h"
#include <string>
#include <memory>
#include "../camera/camera.h"
#include "../external/olcPixelGameEngine.h"
#include <iostream>
#include "../level/level.h"
#include "../actor/actor.h"
#include "../asset_manager/asset_manager.h"
#include "ufo_global.h"
#include "../ufo_engine/ufo_engine.h"
#include "../sprite_reference/sprite_reference.h"
#include "../keyboard/single_keyboard.h"
#include "../ufo_maths/ufo_maths.h"
#include <cmath>

BitmapCollisionBody::BitmapCollisionBody(olc::vf2d _position, std::string _mask, std::string _layer_tag) :
Actor(_position),
mask{_mask},
is_grounded{false},
was_grounded{false},
on_dynamic_solid{false},
snap_to_ground{6},
snap_up_range{10}
{
    game = Engine::Get().current_level.get();
    mask_decal = AssetManager::Get().GetDecal(mask);
    solid_layer = "solid";
    z_index = 1;
}

void BitmapCollisionBody::OnLevelEnter(Level* _level){
    /*auto u_sprite = std::make_unique<SpriteReference>(
        mask,
        olc::vf2d(0.0f, 0.0f),
        olc::vf2d(0.0f, 0.0f),
        AssetManager::Get().GetDecal(mask)->sprite->Size(),
        olc::vf2d(1.0f, 1.0f),
        0.0f,
        0
    );

    Console::Out("mask size",AssetManager::Get().GetDecal(mask)->sprite->Size());

    AddChild(std::move(u_sprite));*/
}

void
BitmapCollisionBody::OnUpdate(){
    Actor::OnUpdate();
    velocity.y += 60.0f * Engine::Get().GetDeltaTime();

    int direction = int(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held) - int(SingleKeyboard::Get().GetKey(olc::LEFT).is_held);
    velocity.x += 40.0f * (float)direction * Engine::Get().GetDeltaTime();

    float previous_velocity = velocity.x;

    velocity.x -= ufoMaths::Sign(velocity.x) * 10.7f * Engine::Get().GetDeltaTime();
    if(velocity.x * previous_velocity < 0.0f){
        velocity.x = 0.0f;
        Console::Out("BitmapCollisionBody Should Stop");
    }

    //Console::Out(velocity.x);

    if(is_grounded && SingleKeyboard::Get().GetKey(olc::Z).is_held) velocity.y = -50.1f;
    
    ApplyCollision(game);

    local_position = position;
    
}

bool
BitmapCollisionBody::IsOverlapping(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour){

    olc::Decal* collision_layer = _map->asset_manager.GetDecal(_layer);

    for(int y = int(_position.y); y < int(_position.y) + _decal->sprite->Size().y; y++){
        for(int x = int(_position.x); x < int(_position.x) + _decal->sprite->Size().x; x++){
            if(CompareColour(_decal->sprite->GetPixel(x- int(_position.x), y- int(_position.y)), olc::WHITE)
                && CompareColour(collision_layer->sprite->GetPixel(x, y), _colour)
            ){
                return true;
            }
        }
    }
    return false;
}

bool
BitmapCollisionBody::IsOverlappingOtherDecal(olc::Decal *_decal, olc::vf2d _position, olc::Decal *_other_decal, olc::vf2d _other_position, olc::Pixel _colour){

    for(int y = int(_position.y); y < int(_position.y) + _decal->sprite->Size().y; y++){
        for(int x = int(_position.x); x < int(_position.x) + _decal->sprite->Size().x; x++){
            if(CompareColour(_decal->sprite->GetPixel(x- int(_position.x), y- int(_position.y)), olc::WHITE)
                && CompareColour(_other_decal->sprite->GetPixel(x - int(_other_position.x), y - int(_other_position.y)), _colour)
            ){
                return true;
            }
        }
    }
    return false;
}

bool
BitmapCollisionBody::IsOverlappingSolid_Or_SemiSolid(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position){
    for(auto [k, v] : semisolid_colours_overlapped){
        olc::Pixel colour = StringToColour(k);
        if(!IsOverlapping(_map, mask_decal, solid_layer, _position, colour) &&
            !v &&
            velocity.y > 0.0f
        ){
            return true;
        }
    }
    if(IsOverlapping(_map, mask_decal, solid_layer, _position, olc::WHITE)) return true;
    return false;
}

int
BitmapCollisionBody::IsOverlappingHeight(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour){ //Pass in the map

    olc::Decal* collision_layer = _map->asset_manager.GetDecal(_layer);

    for(int y = int(_position.y); y < int(_position.y) + _decal->sprite->Size().y; y++){
        for(int x = int(_position.x); x < int(_position.x) + _decal->sprite->Size().x; x++){
            if(CompareColour(_decal->sprite->GetPixel(x- int(_position.x), y- int(_position.y)), olc::WHITE)
                && CompareColour(collision_layer->sprite->GetPixel(x, y), _colour) //I'm gonna need a downcast or access the name from the asset manager
            ){
                return y;
            }
        }
    }
    return int(position.y) + _decal->sprite->Size().y;
}

//HeightUntilGround
//Used to make platforming type actors snap to ground as if they are walking down a slope, or stairs.
//Do a comparison between the value returned by this function and snap_to_ground
int
BitmapCollisionBody::HeightUntilGround(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour){

    olc::Decal* collision_layer = _map->asset_manager.GetDecal(_layer);

    for(int y = int(_position.y) + _decal->sprite->Size().y; y < int(_position.y) + _decal->sprite->Size().y + snap_to_ground; y++){
        for(int x = int(_position.x); x < int(_position.x) + _decal->sprite->Size().x; x++){
            if(CompareColour(collision_layer->sprite->GetPixel(x,y), _colour)){

                //Difference between player bottom and highest collision cell
                return y -(int(_position.y) + _decal->sprite->Size().y);
            }
        }
    }
    //This is returned to consider it a non-snap case, too much space between actor and ground
    return snap_to_ground;
}

//should be IsOverlapping(olc::Decal *_decal, olc::vf2d _position, std::string _other_decal, olc::vf2d _other_position, olc::Pixel _colour)
//can make a function that iterates through actor layers and makes use of this function

void
BitmapCollisionBody::AdjustEnterDynamicSolidX(Level* _level){
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position;
        act_new_position.x = act->position.x + act->velocity.x;

        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            int a = 0;
            if(velocity.x > 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(velocity.x < 0.0f){
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustEnterDynamicSolidY(Level* _level){
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position + act->velocity; //Maybe check for a different position

        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            if(velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustEnterPseudoStaticSolidX(Level* _level){
    //We compare with the future state of DynamicSolid here to make sure we are out of collision by the end of the frame,
    //because if Player is relative to Dynamic solid, it is reasonable to emulate a scenario where both the x and y velocity is added
    //the behaviour should emulate that of a normal solid.
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position + act->velocity;

        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            std::cout << "help" << std::endl;
            int a = 0;
            if(velocity.x > 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(velocity.x < 0.0f){
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustEnterPseudoStaticSolidY(Level* _level){
    //We compare with the future state of DynamicSolid here to make sure we are out of collision by the end of the frame,
    //because if Player is relative to Dynamic solid, it is reasonable to emulate a scenario where both the x and y velocity is added
    //the behaviour should emulate that of a normal solid.
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position + act->velocity;
        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            if(velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustEnteredDynamicSolidX(Level* _level){
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position;
        act_new_position.x = act->position.x + act->velocity.x; //Dynamic solid moves in along the X-axis

        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            int a = 0;
            if(act->velocity.x < 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(act->velocity.x > 0.0f){ //next frame
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustEnteredDynamicSolidY(Level* _level){
    for(auto act : _level->dynamic_bitmap_solid_handles){
        olc::vf2d act_new_position = act->position + act->velocity; //Dynamic solid moves in along the X-axis

        if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
            if(act->velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(act->velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}

void
BitmapCollisionBody::AdjustCollisionX(Level* _map){ //What information should be passed in?
    if(IsOverlapping(_map, mask_decal, solid_layer, position)){
        if(velocity.x > 0.0f){
            position.x = std::floor(position.x); //Do I need floor and ceil?
            while(IsOverlapping(_map,mask_decal, solid_layer, position)){
                position.x -= 1.0f;
            }
        }
        if(velocity.x < 0.0f){
            position.x = std::ceil(position.x);
            while(IsOverlapping(_map,mask_decal, solid_layer, position)){
                position.x += 1.0f;
            }
        }
        velocity.x = 0.0f;
        hit_wall = true;
    }
}

void
BitmapCollisionBody::AdjustCollisionY(Level* _map){
    if(IsOverlapping(_map,mask_decal, solid_layer, position)){
        if(velocity.y > 0.0f){
            position.y = std::floor(position.y);
            while(IsOverlapping(_map,mask_decal, solid_layer, position)){
                position.y -= 1.0f;
            }
        }
        if(velocity.y < 0.0f){
            position.y = std::ceil(position.y);
            while(IsOverlapping(_map,mask_decal, solid_layer, position)){
                position.y += 1.0f;
            }
        }

        velocity.y = 0.0f;
    }

    //SEMI SOLID
}

void
BitmapCollisionBody::CB_ApplyUpSlope(Level* _lvl){
    int up_slope_range = 10;

    if(IsOverlapping(_lvl, mask_decal, solid_layer, position)){
        position = former_position;
        float direction = 0.0f;
        if(velocity.x > 0.0f) direction = 1.0f;
        if(velocity.x < 0.0f) direction = -1.0f;

        //This loop is to iterate from start position to end position. The +1 is to make sure the distance isn't somehow rounded down.
        for(int x_step = 0; x_step < (int)(std::abs(velocity.x*direction)+1); x_step++){
            olc::vf2d before_height_adj = position;        
            position.x += direction;
            for(int step = 0; step < up_slope_range; step++){
                position.y -= 1.0f;
                
                //If able to move full range
                if(
                    !IsOverlapping(_lvl, mask_decal, solid_layer, position) &&
                    x_step == (int)(std::abs(velocity.x*direction)) &&
                    step == up_slope_range-1
                ){
                    position.x += velocity.x;
                    break;
                }

                //Unsure if this is needed but it seems to work.
                if(!IsOverlapping(_lvl, mask_decal, solid_layer, position)) break;
                if(up_slope_range-1 == step && IsOverlapping(_lvl, mask_decal, solid_layer, position)){
                    position = before_height_adj;
                }
            }
        }
    }
}

void
BitmapCollisionBody::ApplyUpSlope(Level* _map){
    //SOLID
    olc::vf2d temporary_slope_adjustment_position = former_position;
    temporary_slope_adjustment_position.y = std::floor(former_position.y);
    if(IsOverlapping(_map,mask_decal, solid_layer, position)){
        //Explanation
        //Upon colliding in what we presume to be the x-axis, we look back on our former position to see how Dummy interacts
        //from there with the terrain, pixel by pixel.
        //We start by flooring former_position.y to make sure Dummy is properly aligned with the grid in case he for some reason wouldn't be.
        //depending on velocity we do the same in the x-axis.

        if(velocity.x > 0.0f){
            temporary_slope_adjustment_position.x = std::floor(temporary_slope_adjustment_position.x);
        }
        if(velocity.x < 0.0f){
            temporary_slope_adjustment_position.x = std::ceil(temporary_slope_adjustment_position.x);
        }

        if(velocity.x > 0.0f){
            //Explanation
            //Until former_position.x becomes our current position, we step in the direction of our velocity in the x-axis.
            //This works because Dummy's new position is on the same pixel as former_position.x + velocity.x
            while(std::floor(temporary_slope_adjustment_position.x) != std::floor(position.x)+1.0f){
                if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) > int(temporary_slope_adjustment_position.y) + snap_up_range
                    && IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) != temporary_slope_adjustment_position.y +
                    mask_decal->sprite->Size().y)
                {
                    while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position)){
                        temporary_slope_adjustment_position.y -= 1.0f;
                    }
                }
                temporary_slope_adjustment_position.x += 1.0f;
            }
            //if you're still in collision after moving +=1.0f, then you move up again
            //There is a possibility that Dummy entres another pixel upon the last step, so you need to readjust in the y-axis.
            if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) > int(temporary_slope_adjustment_position.y) + snap_up_range){
                while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position)){
                    temporary_slope_adjustment_position.y -= 1.0f;
                }
            }
        }

        if(velocity.x < 0.0f){
            while(temporary_slope_adjustment_position.x != std::floor(position.x)){

                if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) > int(temporary_slope_adjustment_position.y) + snap_up_range
                    && IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) != temporary_slope_adjustment_position.y +
                    mask_decal->sprite->Size().y){
                    while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position)){
                        temporary_slope_adjustment_position.y -= 1.0f;
                    }
                }
                temporary_slope_adjustment_position.x -= 1.0f;
            }

            if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position) > int(temporary_slope_adjustment_position.y) + snap_up_range){
                while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position)){
                    temporary_slope_adjustment_position.y -= 1.0f;
                }
            }
        }
        position.y = temporary_slope_adjustment_position.y;
    }
    
}

void
BitmapCollisionBody::ApplyUpSlope_SemiSolid(Level* _map){
    olc::vf2d temporary_slope_adjustment_position = former_position;
    
    for(auto [k, v] : semisolid_colours_overlapped){
        olc::Pixel colour = StringToColour(k);
        if(IsOverlapping(_map, mask_decal, solid_layer, position, colour) &&
            !v &&
            velocity.y > 0.0f
        ){
            temporary_slope_adjustment_position.y = std::floor(temporary_slope_adjustment_position.y);

            if(velocity.x > 0.0f){
                temporary_slope_adjustment_position.x = std::floor(temporary_slope_adjustment_position.x);
            }
            if(velocity.x < 0.0f){
                temporary_slope_adjustment_position.x = std::ceil(temporary_slope_adjustment_position.x);
            }

            if(velocity.x > 0.0f){

                while(std::floor(temporary_slope_adjustment_position.x) != std::floor(position.x)+1.0f){
                    if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) > int(temporary_slope_adjustment_position.y) + snap_up_range
                        && IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) != temporary_slope_adjustment_position.y +
                        mask_decal->sprite->Size().y){

                        while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour)){
                            temporary_slope_adjustment_position.y -= 1.0f;
                        }
                    }
                    temporary_slope_adjustment_position.x += 1.0f;
                }

                if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) > int(temporary_slope_adjustment_position.y) + snap_up_range){
                    while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour)){
                        temporary_slope_adjustment_position.y -= 1.0f;
                    }
                }
            }

            if(velocity.x < 0.0f){
                while(temporary_slope_adjustment_position.x != std::floor(position.x)){

                    if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) > int(temporary_slope_adjustment_position.y) + snap_up_range
                        && IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) != temporary_slope_adjustment_position.y +
                        mask_decal->sprite->Size().y){
                        while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour)){
                            temporary_slope_adjustment_position.y -= 1.0f;
                        }
                    }
                    temporary_slope_adjustment_position.x -= 1.0f;
                }

                if(IsOverlappingHeight(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour) > int(temporary_slope_adjustment_position.y) + snap_up_range){
                    while(IsOverlapping(_map, mask_decal, solid_layer, temporary_slope_adjustment_position, colour)){
                        temporary_slope_adjustment_position.y -= 1.0f;
                    }
                }
            }
            position.y = temporary_slope_adjustment_position.y;
        }
    }
}

/*void
CellActor::ApplyDownSlope_SemiSolid(CellMap* _map){
    int distance_under_character = 0;
    while(!IsOverlapping(_map, mask_decal, solid_layer, {int(position.x), int(position.y) + distance_under_character}, colour)){
        for(auto [k, v] : semisolid_colours_overlapped){
            olc::Pixel colour = StringToColour(k);
            if(!IsOverlapping(_map, mask_decal, solid_layer, {int(position.x), int(position.y) + distance_under_character}, colour) &&
                !v &&
                velocity.y > 0.0f
            ){
                
                distance_under_character++;
                
            }
            else{
                distance_under_character--;
            }
        }
    }
}*/

void
BitmapCollisionBody::CB_AdjustDownSlope(Level* _lvl){
    if(!(was_grounded == true && is_grounded == false && velocity.y > 0.0f)) return;

    int down_slope_range = 8;
    if(!IsOverlapping(_lvl, mask_decal, solid_layer, olc::vf2d(position.x, position.y + 1.0f), olc::WHITE)){
        for(int i= 0; i < down_slope_range; i++){
            olc::vf2d before_down_slope_adj = position;
            if(!IsOverlapping(_lvl, mask_decal, solid_layer, olc::vf2d(position.x, position.y + 1.0f), olc::WHITE)){
                position.y += 1.0f;
            }
            else{
                break;
            }
            if(i == down_slope_range-1 && IsOverlapping(_lvl, mask_decal, solid_layer, position, olc::WHITE)){
                position = before_down_slope_adj;
            }
        }
    }
}

void
BitmapCollisionBody::AdjustDownSlope(Level* _map){
    //SEMI SOLID HEIGHT ADJUSTMENT SNAP_TO_GROUND
    if(was_grounded == true && is_grounded == false && velocity.y > 0.0f){
        position.y = std::floor(position.y);
        if((HeightUntilGround(_map, mask_decal, solid_layer, position, olc::RED) < snap_to_ground) ||
            (HeightUntilGround(_map, mask_decal, solid_layer, position, olc::WHITE) < snap_to_ground)
            ){
            while(
                !IsOverlapping(_map, mask_decal, solid_layer, position, olc::RED) &&
                !IsOverlapping(_map, mask_decal, solid_layer, position, olc::WHITE)
                ){

                position.y += 1.0f;

            }
            position.y -= 1.0f;
        }
    }
}


bool BitmapCollisionBody::IsBeingEntered(olc::vf2d _position, olc::vf2d _delta_position, int _direction, std::string _mask){
    if(IsOverlappingOtherDecal(AssetManager::Get().GetDecal(_mask), _position, AssetManager::Get().GetDecal(mask), position)){
        std::cout << "just colliding before: " << _delta_position << std::endl;
    }
    if(!IsOverlappingOtherDecal(AssetManager::Get().GetDecal(_mask), _position, AssetManager::Get().GetDecal(mask), position)){
        if(IsOverlappingOtherDecal(AssetManager::Get().GetDecal(_mask), {_position.x + _delta_position.x, _position.y}, AssetManager::Get().GetDecal(mask), position)){
            if(_delta_position.x > 0){
                std::cout << "left" << std::endl;
                if(_direction == UfoGlobal::LEFT) return true;
            }
            if(_delta_position.x < 0){
                std::cout << "right" << std::endl;
                if(_direction == UfoGlobal::RIGHT) return true;
            }
        }
    }
    if(!IsOverlappingOtherDecal(AssetManager::Get().GetDecal(_mask), {_position.x + _delta_position.x, _position.y}, AssetManager::Get().GetDecal(mask), position)){
        if(IsOverlappingOtherDecal(AssetManager::Get().GetDecal(_mask), _position + _delta_position, AssetManager::Get().GetDecal(mask), position)){
            if(_delta_position.y > 0){
                std::cout << "up" << std::endl;
                if(_direction == UfoGlobal::UP) return true; //I get it, I actually need to get OUT of the hitbox before the next frame, right? No?
            }
            if(_delta_position.y < 0){
                std::cout << "down" << std::endl;
                if(_direction == UfoGlobal::DOWN) return true;
            }
        }
    }
    
    return false;
}

olc::Pixel BitmapCollisionBody::StringToColour(std::string _colour_name){
    if(_colour_name == "red") return olc::RED;
    if(_colour_name == "orange") return UfoGlobal::ORANGE;
    if(_colour_name == "yellow") return UfoGlobal::YELLOW;
    if(_colour_name == "lemon") return UfoGlobal::LEMON;
}

bool BitmapCollisionBody::IsAlreadyInSolid(std::string _colour_name){
    return semisolid_colours_overlapped.at(_colour_name);
}

void
BitmapCollisionBody::UpdateSemiSolidOverlapStatus(Level* _map){
    for(auto [k,v] : semisolid_colours_overlapped){
        semisolid_colours_overlapped[k] = IsOverlapping(_map, mask_decal, solid_layer, position, StringToColour(k));
    }
}

void BitmapCollisionBody::CheckSemiSolidOverlapStatus(olc::vf2d _position){

}

void
BitmapCollisionBody::ApplyCollisionNaive(Level* _map){
    former_position = position;
    position.x += velocity.x * Engine::Get().GetDeltaTime();
    ApplyUpSlope_SemiSolid(_map);
    CB_ApplyUpSlope(_map);
    AdjustCollisionX(_map);
    UpdateSemiSolidOverlapStatus(_map);
    position.y += velocity.y * Engine::Get().GetDeltaTime();
    AdjustCollisionY(_map);
    for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal, solid_layer, position, StringToColour(k)) &&
            velocity.y > 0.0f &&
            !v){
            
            position.y = std::floor(position.y);
            while(IsOverlapping(_map,mask_decal, solid_layer, position, StringToColour(k))){
                position.y -= 1.0f;
            }

            velocity.y = 0.0f;
        }
    }
    was_grounded = is_grounded;
    is_grounded = false;

    for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, StringToColour(k))){
            is_grounded = true;        
        }
    }

    if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f})){
        is_grounded = true;        
    }

    CB_AdjustDownSlope(_map);

    for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, StringToColour(k))){
            is_grounded = true;        
        }
    }
    if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f})){
        is_grounded = true;        
    }

}

void
BitmapCollisionBody::ApplyCollision(Level* _map){

    collision_history.clear();

    auto act_layer = _map;

    if(on_dynamic_solid){
        for(auto act : _map->dynamic_bitmap_solid_handles){

            //These checks are for making sure the player has moved out of the DynamicSolid before checking for player-movement
            //Checking 2 pixles below player to see if still on the ground. Might not be necessary
            if(IsOverlappingOtherDecal(mask_decal, olc::vf2d(position.x, position.y + 2.0f), AssetManager::Get().GetDecal(act->mask),act->position)){

                position.x += act->velocity.x; //This is to move the player relative to the DynamicSolid you collided with
                //These checks are for making sure the player has moved out of the DynamicSolid before checking for player-movement
                if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), {act->position.x+act->velocity.x,act->position.y})){
                    
                    if(IsOverlappingOtherDecal(mask_decal, {position.x+1.0f, position.y}, AssetManager::Get().GetDecal(act->mask), {act->position.x+act->velocity.x,act->position.y})){
                        position.x -= 1.0f;
                        while(IsOverlappingOtherDecal(mask_decal, {position.x, position.y}, AssetManager::Get().GetDecal(act->mask), {act->position.x+act->velocity.x,act->position.y})){
                            position.x -= 1.0f;
                        }
                    }
                    else if(IsOverlappingOtherDecal(mask_decal, {position.x-1.0f, position.y}, AssetManager::Get().GetDecal(act->mask), {act->position.x+act->velocity.x,act->position.y})){
                        position.x += 1.0f;
                        while(IsOverlappingOtherDecal(mask_decal, {position.x, position.y}, AssetManager::Get().GetDecal(act->mask), {act->position.x+act->velocity.x,act->position.y})){
                            position.x += 1.0f;
                        }
                    }
                }

                position.y += act->velocity.y; //This is to move the player relative to the DynamicSolid you collided with
                //This is where things go wrong. This was mainly for the cases where you aren't quite resolved after DynamicSolid is moved

                if(IsOverlappingOtherDecal(mask_decal, position, AssetManager::Get().GetDecal(act->mask), act->position+act->velocity)){
                    
                    if(IsOverlappingOtherDecal(mask_decal, {position.x, position.y+1.0f}, AssetManager::Get().GetDecal(act->mask), act->position+act->velocity)){
                        position.y -= 1.0f;
                        while(IsOverlappingOtherDecal(mask_decal, {position.x, position.y}, AssetManager::Get().GetDecal(act->mask), act->position+act->velocity)){
                            position.y -= 1.0f;
                        }
                    }
                    else if(IsOverlappingOtherDecal(mask_decal, {position.x, position.y-1.0f}, AssetManager::Get().GetDecal(act->mask), act->position+act->velocity)){ //added the if statements,dint do anything
                        position.y += 1.0f;
                        while(IsOverlappingOtherDecal(mask_decal, {position.x, position.y}, AssetManager::Get().GetDecal(act->mask), act->position+act->velocity)){
                            position.y += 1.0f;
                        }
                    }
                }
            }
        }
    }

    former_position = position;

    if(!on_dynamic_solid) AdjustEnteredDynamicSolidX(act_layer);
    position.x += velocity.x * Engine::Get().GetDeltaTime();
    if(on_dynamic_solid) AdjustEnterPseudoStaticSolidX(act_layer);

    //HEIGHT ADJUSTMENT OVERLAP
    ApplyUpSlope(_map);
    // COLLISION ADJUSTMENT X-AXIS
    AdjustCollisionX(_map);
    //ThisVsDynamicSolid
    if(!on_dynamic_solid) AdjustEnterDynamicSolidX(act_layer);

    // ADJUSTMENT ALONG Y-AXIS
    is_already_in_semi_solid = false;
    is_already_in_semi_solid = IsOverlapping(_map, mask_decal, solid_layer, position, olc::RED);
    position.y += velocity.y * Engine::Get().GetDeltaTime();

    if(on_dynamic_solid) AdjustEnterPseudoStaticSolidY(act_layer);
    AdjustCollisionY(_map);

    if(!on_dynamic_solid) AdjustEnterDynamicSolidY(act_layer);

    was_grounded = is_grounded;
    is_grounded = false;

    for(auto act : act_layer->dynamic_bitmap_solid_handles){

        olc::vf2d act_new_position = act->position+act->velocity;
        on_dynamic_solid = false;
        if(IsOverlappingOtherDecal(mask_decal, olc::vf2d(position.x, position.y + 2.0), AssetManager::Get().GetDecal(act->mask), act_new_position)){
            on_dynamic_solid = true;
            is_grounded = true;
        }
    }

    if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}) ||
        IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, olc::RED)){
        is_grounded = true;        
    }

    AdjustDownSlope(_map);

    if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}) ||
        IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, olc::RED)){
        is_grounded = true;        
    }
    
}

std::string BitmapCollisionBody::GetType(){return "CollisionBody";}