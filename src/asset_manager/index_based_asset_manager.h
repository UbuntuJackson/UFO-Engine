#include "../external/olcPixelGameEngine.h"
#include <array>
#include <string_view>
#include "../console/console.h"

class IndexBasedAssetManager{
public:
    IndexBasedAssetManager(){
        
    }

    IndexBasedAssetManager(IndexBasedAssetManager&&) = delete;

    std::vector<olc::Decal> decals;
    std::vector<olc::Sprite> sprites;

    int LoadSprite(std::string_view _path){
        sprites.push_back(olc::Sprite(_path));
        
        switch (sprites.back().LoadFromFile(_path)){
            case olc::rcode::NO_FILE:
                Console::PrintLine("Error: The file does not exist at path", _path);
                sprites.erase(sprites.begin() + sprites.size()-1);
                return -1;
                break;
            case olc::rcode::FAIL:
                sprites.erase(sprites.begin() + sprites.size()-1);
                Console::PrintLine("Error: Failed to load the file at path", _path);
                return -1;
                break;
            default:
                decals.push_back(olc::Decal(_path));
                break;
        }

        return decals.size()-1;
    }

    void RemoveSprite(int _index){
        if(_index < 0 || _index >= sprites.size()){
            Console::Print("Sprite with index:", _index, "Does not exist\n");
            return;
        }
        sprites.erase(sprites.begin()+_index);
        decals.erase(decals.begin()+_index);
    }

};