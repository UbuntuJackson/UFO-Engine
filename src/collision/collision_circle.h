#pragma once
#include "../actor/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/level.h"
#include "../ufo_engine/ufo_engine.h"
#include "../ufo_collision/collision_data.h"

namespace ufotest{

class CollisionCircle : public Actor{
public:
    Vector2f velocity = {0.0f, 0.0f};
    float radius = 10.0f;
    float minimum_moving_distance_post_dt = 0.001f;

    Level* level = nullptr;

    CollisionCircle(Vector2f _) : Actor(_){}

    void OnLevelEnter(Level* _level){
        Actor::OnLevelEnter(_level);

        level = _level;
    }

    void OnUpdate(){
        Vector2f velocity_dt = velocity * Engine::Get().GetDeltaTime(); 

        if(velocity_dt.mag() == 0.0f){
            
            return;
        }

        if(velocity_dt.mag() < minimum_moving_distance_post_dt){
            velocity_dt = velocity_dt.norm() * minimum_moving_distance_post_dt;
        }

        bool still_on_collision_course = false;
        bool collided = false;

        while(still_on_collision_course){
            
            std::vector<std::pair<float, Ray2>> hit_line_segments;

            still_on_collision_course = false;

            for(const auto& line : level->collision_system.lines){
                Ray2 movement_ray = Ray2(local_position, local_position + velocity);

                Circle this_shape = Circle(local_position, radius);

                CollisionData collision_data = ufoMaths::RayVsRay(movement_ray, Ray2(line.Start()+line.Normal()*radius, line.End()+line.Normal()*radius));
                if(collision_data.is_hit){
                    collided = true;
                    still_on_collision_course = true;

                    hit_line_segments.push_back(std::make_pair(collision_data.collision_time, line));
                    
                }
            }

            //If a is less than b, swap.
            std::sort(hit_line_segments.begin(), hit_line_segments.end(),[&](const auto _a, const auto _b){return _b.first > _a.first;});

            Circle this_shape = Circle(local_position, radius);

            local_position += level->collision_system.DynamicCircleVsRay(this_shape, velocity_dt, hit_line_segments[0].second);
            velocity = level->collision_system.DynamicCircleVsRay(this_shape, velocity, hit_line_segments[0].second);
        }

        if(!collided) local_position += velocity_dt;
    }

    void OnDraw(Camera* _camera){
        Engine::Get().pixel_game_engine.DrawCircle(_camera->Transform(GetGlobalPosition()), radius*_camera->scale);
    }

};

}