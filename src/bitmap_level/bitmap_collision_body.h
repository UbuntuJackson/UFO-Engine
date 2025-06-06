#ifndef CELL_ACTOR_H
#define CELL_ACTOR_H
#include "../external/olcPixelGameEngine.h"
#include "../actor/actor.h"
#include <string>
#include <vector>
class Camera;
class Level;
class LayerActor;
class LayerSolid;

class BitmapCollisionBody : public Actor{
public:
    Level *game;
    
    //olc::vf2d position;
    olc::vf2d former_position;
    olc::vf2d velocity;
    olc::vf2d size;
    bool is_grounded;
    bool was_grounded;
    std::map<std::string, bool> semisolid_colours_overlapped = {{"red", false}, {"orange", false}, {"yellow", false}, {"lemon", false}};
    bool is_already_in_semi_solid = false;
    bool is_already_in_semi_solid_red = false;
    bool is_already_in_semi_solid_orange = false;
    bool is_already_in_semi_solid_yellow = false;
    bool is_already_in_semi_solid_lemon = false;
    bool on_dynamic_solid = false;
    bool is_grounded_dynamic = false;
    bool was_grounded_dynamic = false;
    olc::vf2d correct_dynamic;
    int snap_to_ground;
    int snap_up_range;
    bool is_target = false;
    std::string mask;
    olc::Decal* mask_decal;
    std::string solid_layer;
    std::vector<std::string> collision_history;

    bool hit_wall = false;
    bool hit_ceiling = false;
    bool hit_floor = false;
    bool hit_slope = false;

    int draw_layer = 2;

    BitmapCollisionBody(olc::vf2d _position, std::string _mask, std::string _layer_tag);

    void OnLevelEnter(Level* _level);

    bool IsOverlapping(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour = olc::WHITE);
    int IsOverlappingHeight(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour = olc::WHITE);
    int HeightUntilGround(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position, olc::Pixel _colour = olc::WHITE);
    bool IsOverlappingOtherDecal(olc::Decal *_decal, olc::vf2d _position, olc::Decal *_other_decal, olc::vf2d _other_position, olc::Pixel _colour = olc::WHITE);
    bool IsOverlappingSolid_Or_SemiSolid(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position);

    virtual void AdjustCollisionX(Level* _map);
    virtual void AdjustCollisionY(Level* _map);
    virtual void AdjustDownSlope(Level* _map);
    virtual void CB_AdjustDownSlope(Level* _lvl);
    virtual void CB_ApplyUpSlope(Level* _lvl);
    virtual void ApplyUpSlope(Level* _map);
    virtual void ApplyUpSlope_SemiSolid(Level* _map);
    virtual void AdjustEnteredDynamicSolidX(Level* _act_layer);
    virtual void AdjustEnteredDynamicSolidY(Level* _act_layer);
    virtual void AdjustEnterPseudoStaticSolidX(Level* _act_layer);
    virtual void AdjustEnterPseudoStaticSolidY(Level* _act_layer);
    virtual void AdjustEnterDynamicSolidX(Level* _act_layer);
    virtual void AdjustEnterDynamicSolidY(Level* _act_layer);
    virtual bool IsBeingEntered(olc::vf2d _position, olc::vf2d _delta_position, int _direction, std::string _mask);
    olc::Pixel StringToColour(std::string _colour_name);
    bool IsAlreadyInSolid(std::string _colour_name);
    void UpdateSemiSolidOverlapStatus(Level* _map);
    void CheckSemiSolidOverlapStatus(olc::vf2d _position);
    virtual void ApplyCollisionNaive(Level* _map);
    virtual void ApplyCollision(Level* _map);

    virtual void OnUpdate();
    
    virtual std::string GetType();
};

#endif