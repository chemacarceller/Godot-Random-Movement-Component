#ifndef RANDOM_MOVEMENT_COMPONENT_H
#define RANDOM_MOVEMENT_COMPONENT_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class RandomMovementComponent : public Node {
    GDCLASS(RandomMovementComponent, Node)

private:
    bool _isActorAttached = true;
    bool _isEnabled = true;
    float movementRadius = 0.05f;

    Node3D *_parentActor = nullptr;
    Vector3 _parentActorPosition;
    bool _newOffset = true;
    
    // Efficient to keep one instance of RNG
    Ref<RandomNumberGenerator> _rng;

protected:
    static void _bind_methods();

public:
    RandomMovementComponent();
    ~RandomMovementComponent();

    void _ready() override;
    void _physics_process(double delta) override;
    void _notification(int p_what);

    // Getters and Setters for Godot Binding
    void set_is_enabled(bool p_value) { _isEnabled = p_value; }
    bool get_is_enabled() const { return _isEnabled; }
    
    void set_movement_radius(float p_radius) { movementRadius = p_radius; }
    float get_movement_radius() const { return movementRadius; }

    void set_is_actor_attached(bool p_attached) { _isActorAttached = p_attached; }
    bool get_is_actor_attached() const { return _isActorAttached; }
};

#endif