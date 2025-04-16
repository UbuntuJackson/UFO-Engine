#pragma once

#include "../ufo_maths/ufo_maths.h"
#include "../actor/actor.h"
#include "../json/json_dictionary.h"

class Camera;
class Level;

class TileMap : public Actor{
public:
    
    std::vector<int> tilemap_data;
    Level* level;

    void OnLevelEnter(Level *_level);

    static std::unique_ptr<TileMap> Load(JsonDictionary& _layer);

    int GetTileID_AtLevelPosition(Vector2f _position);

    void OnDraw(Camera* _camera);
};