#pragma once
#include <vector>

namespace MarkTree{

class MTActor;

class MarkSweeper{
public:
    std::vector<MTActor*> memory;

    MTActor* root_mt_actor = nullptr;

    void Init();

    void Update();

    static MarkSweeper& Get();
};

}