#ifndef TILEMAP_H
#define TILEMAP_H
#include <vector>
#include <unordered_map>
#include <string>
#include "tileset_data.h"
#include "../external/olcPixelGameEngine.h"
#include "../json/json.h"
#include "../actor/actor.h"
#include "../shapes/rectangle.h"
#include "../ufo_maths/ufo_maths.h"

class Camera;

struct TileCollisionData{
    bool place_free;
    std::vector<int> tiles;
};

class Tilemap{
public:
    const int EMPTY = 0;
    const int FULL = 1;
    const int HALF_BOTTOM = 2;
    const int HALF_TOP = 3;
    const int HALF_LEFT = 4;
    const int HALF_RIGHT = 5;
    const int SLOPE45_BOTTOM_RIGHT = 10;
    const int SLOPE45_BOTTOM_LEFT = 15;
    const int SLOPE_22dot5_RIGHT_1 = 6+5;
    const int SLOPE_22dot5_RIGHT_2 = 7+5;

    std::vector<std::vector<int>> layer_data_sets;
    std::vector<std::vector<int>> foreground_layer_data_sets;

    std::vector<int> tilemap_collision_data;
    TileCollisionData GetTileCollisionData(const ufo::Rectangle& _rectangle, std::string _tileset);
    
    int number_of_rows;
    int number_of_columns;
    int tile_width;
    int tile_height;
    std::vector<std::string> keys_to_temporary_assets;
    //  firstgid, key
    std::vector<TilesetData> tileset_data;

    TilesetData GetTilesetData(std::string _name);

    void Load(const Json& _json);

    Tilemap() = default;
    void Draw(Camera* _camera);
    void DrawForeGround(Camera* _camera);
    void DrawCollisionTiles(Camera* _camera);
};

#endif