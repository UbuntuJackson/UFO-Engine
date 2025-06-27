#include "mark_sweeper.h"
#include "mt_actor.h"
namespace MarkTree{

void ExampleProgram(){
    MarkSweeper::Get().Init();

    MarkSweeper::Get().root_mt_actor->children.push_back(new MTActor(Vector2f(0.0f, 0.0f)));
}

}