#include "random_movement.h"
#include <godot_cpp/variant/utility_functions.hpp>

void RandomMovementComponent::_bind_methods() {
    // Binding Methods
    ClassDB::bind_method(D_METHOD("get_is_enabled"), &RandomMovementComponent::get_is_enabled);
    ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &RandomMovementComponent::set_is_enabled);
    
    ClassDB::bind_method(D_METHOD("get_movement_radius"), &RandomMovementComponent::get_movement_radius);
    ClassDB::bind_method(D_METHOD("set_movement_radius", "value"), &RandomMovementComponent::set_movement_radius);

    ClassDB::bind_method(D_METHOD("get_is_actor_attached"), &RandomMovementComponent::get_is_actor_attached);
    ClassDB::bind_method(D_METHOD("set_is_actor_attached", "value"), &RandomMovementComponent::set_is_actor_attached);

    // Registering Properties for the Inspector
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isActorAttached"), "set_is_actor_attached", "get_is_actor_attached");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isEnabled"), "set_is_enabled", "get_is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movementRadius", PROPERTY_HINT_RANGE, "0.01,0.1,0.01"), "set_movement_radius", "get_movement_radius");
}

RandomMovementComponent::RandomMovementComponent() {
    _rng.instantiate();
    _rng->randomize();
}

RandomMovementComponent::~RandomMovementComponent() {}

void RandomMovementComponent::_ready() {
    // Equivalent to @onready
    _parentActor = Object::cast_to<Node3D>(get_parent());
    if (_parentActor) {
        _parentActorPosition = _parentActor->get_position();
    }
}

void RandomMovementComponent::_notification(int p_what) {
    if (p_what == NOTIFICATION_WM_CLOSE_REQUEST) {
        queue_free();
    }
}

void RandomMovementComponent::_physics_process(double delta) {
    if (_isEnabled && _parentActor != nullptr) {
        Vector3 offset = Vector3(0, 0, 0);

        if (_isActorAttached) {
            _parentActorPosition = _parentActor->get_position();
        }

        if (_newOffset) {
            offset.x = _rng->randf_range(-1.0, 1.0) * movementRadius;
            offset.y = _rng->randf_range(-1.0, 1.0) * movementRadius;
            offset.z = _rng->randf_range(-1.0, 1.0) * movementRadius;
            
            _parentActor->set_position(_parentActorPosition + offset);
        } else {
            // Note: In your GDScript, 'offset' was local to the 'if' block. 
            // To subtract the same offset, it should ideally be stored as a member variable.
            _parentActor->set_position(_parentActorPosition); 
        }

        _newOffset = !_newOffset;
    }
}