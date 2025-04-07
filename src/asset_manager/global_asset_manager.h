#include "abstract_asset_manager.h"

class GlobalAssetManager : public AbstractAssetManager{
public:
    GlobalAssetManager(GlobalAssetManager&&) = delete;
    GlobalAssetManager() = default;
    GlobalAssetManager& Get(){
        static GlobalAssetManager self;
        return self;
    }

};