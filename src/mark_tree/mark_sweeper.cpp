#include "mark_sweeper.h"
#include "mt_actor.h"

namespace MarkTree{

void MarkSweeper::Init(){
    root_mt_actor = new MTActor(Vector2f(0.0f, 0.0f));
}

void MarkSweeper::Update(){
    root_mt_actor->SetReachable();

    //This will iterate through removed children, and when it sees they aren't set reachable with SetReachable
    // it will sweep them
    for(int i = memory.size(); i >= 0; i--){
        if(!memory[i]->reachable_from_tree && memory[i]->mt_reference_count == 0){
            delete memory[i];

            memory.erase(memory.begin() + i);
        }
    }

    root_mt_actor->SetUnreachable();

    //Removed children will not be seen as reachable
    root_mt_actor->RemoveChildren();
}

MarkSweeper& MarkSweeper::Get(){
    static MarkSweeper self;
    return self;
}

}