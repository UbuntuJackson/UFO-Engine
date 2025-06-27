#pragma once
#include "../actor/actor.h"
#include "../ufo_maths/ufo_maths.h"

namespace MarkTree{

template<typename tType>
class MTReference{
private:
    tType* ptr;

public:
    MTReference(tType* _ptr) : ptr{_ptr}{}

    tType* Get(){
        return ptr;
    }

public:
    MTReference<tType> Share(){
        ptr->mt_reference_count++;
        return MTReference<tType>(ptr);
    }

    ~MTReference(){
        ptr->mt_reference_count--;
    }

};

class MTActor{
public:
    std::vector<MTActor*> children;

    bool reachable_from_tree = false;

    Vector2f local_position;

    MTActor(Vector2f _position);

    void Update();

    virtual void OnUpdate();

    void Draw(Camera* _camera);

    virtual void OnDraw(Camera* _camera);

    void SetReachable();

    void SetUnreachable();

    int mt_reference_count = 0;

    bool to_be_removed_from_parent = true;

    void RemoveChildren();

};

}