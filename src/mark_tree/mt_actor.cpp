#include "mt_actor.h"
#include "mark_sweeper.h"

namespace MarkTree{

MTActor::MTActor(Vector2f _position) : local_position{_position}{
    MarkSweeper::Get().memory.push_back(this);
}

void MTActor::Update(){
    for(const auto& child : children){
        child->Update();
    }

    OnUpdate();
}

void MTActor::Draw(Camera* _camera){
    for(const auto& child : children){
        child->Draw(_camera);
    }
}

void MTActor::OnUpdate(){

}

void MTActor::SetReachable(){
    reachable_from_tree = true;
    for(MTActor* actor : children){
        actor->SetReachable();
    }   
}

void MTActor::SetUnreachable(){
    reachable_from_tree = false;
    for(MTActor* actor : children){
        actor->SetReachable();
    }   
}

void MTActor::RemoveChildren(){
    for(int i = children.size(); i >= 0; i--){
        if(children[i]->to_be_removed_from_parent){
            children.erase(children.begin()+i);
        }
    }

    for(MTActor* child : children){
        child->RemoveChildren();
    }
}

}