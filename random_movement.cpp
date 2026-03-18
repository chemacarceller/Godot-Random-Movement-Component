#include "random_movement.h"
#include <godot_cpp/variant/utility_functions.hpp>

void RandomMovementComponent::_bind_methods() {
    // Binding Methods
    ClassDB::bind_method(D_METHOD("get_is_enabled"), &RandomMovementComponent::get_is_enabled);
    ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &RandomMovementComponent::set_is_enabled);
    
    ClassDB::bind_method(D_METHOD("get_movement_radius"), &RandomMovementComponent::get_movement_radius);
    ClassDB::bind_method(D_METHOD("set_movement_radius", "value"), &RandomMovementComponent::set_movement_radius);

    // Registering Properties for the Inspector
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isEnabled"), "set_is_enabled", "get_is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_movementRadius", PROPERTY_HINT_RANGE, "0 , 0.1, 0.05"), "set_movement_radius", "get_movement_radius");
}

RandomMovementComponent::RandomMovementComponent() {
    rng.instantiate();
    rng->randomize();
}

RandomMovementComponent::~RandomMovementComponent() {}

void RandomMovementComponent::_ready() {
    // Equivalent to @onready
    parentActor = Object::cast_to<Node3D>(get_parent());
}

// Handle the close request notification to free the node when the window is closed.
void RandomMovementComponent::_notification(int p_what) {
    if (p_what == NOTIFICATION_WM_CLOSE_REQUEST) {
        queue_free();
    }
}

// This method is called every physics frame. It applies a random offset to the parent actor's position if enabled.
void RandomMovementComponent::_physics_process(double delta) {

    // Static variables to store the offset and original position across frames. 
    // This allows us to reset the position after applying the random offset.
    Vector3 offset;
    static Vector3 parentActorPosition;
    static bool forthcomingOffset = true;

    if (_isEnabled && parentActor != nullptr) {

        // If newOffset is true, generate a new random offset. Otherwise, reset the position to the original.
        if (forthcomingOffset) {

            parentActorPosition = parentActor->get_position();

            offset.x = rng->randf_range(-1.0, 1.0) * _movementRadius;
            offset.y = rng->randf_range(-1.0, 1.0) * _movementRadius;
            offset.z = rng->randf_range(-1.0, 1.0) * _movementRadius;
            
            parentActor->set_position(parentActorPosition + offset);
        } else {
            parentActor->set_position(parentActorPosition);
        }

        forthcomingOffset = !forthcomingOffset; // Toggle for the next frame
    }
}