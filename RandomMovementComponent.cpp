#include "RandomMovementComponent.h"

// It provides the interface to ClassDB, Godot's internal database of all registered classes.
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void RandomMovementComponent::_bind_methods() {
    
    // Record of methods for Godot to see.
    ClassDB::bind_method(D_METHOD("get_is_enabled"), &RandomMovementComponent::get_is_enabled);
    ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &RandomMovementComponent::set_is_enabled);
    
    ClassDB::bind_method(D_METHOD("get_movement_radius"), &RandomMovementComponent::get_movement_radius);
    ClassDB::bind_method(D_METHOD("set_movement_radius", "value"), &RandomMovementComponent::set_movement_radius);

    // Exporting properties (equivalent to @export)
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_isEnabled"), "set_is_enabled", "get_is_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "_movementRadius", PROPERTY_HINT_RANGE, "0 , 0.1, 0.05"), "set_movement_radius", "get_movement_radius");
}

// Constructor and Destructor
RandomMovementComponent::RandomMovementComponent() {

    // Instantiate the RNG and randomize it in the constructor to ensure it's ready for use when the component is added to the scene.
    rng.instantiate();
    rng->randomize();
}

RandomMovementComponent::~RandomMovementComponent() {}


// This method is called when the node enters the scene tree. It initializes the parentActor variable by casting the parent node to Node3D.
// Equivalent to @onready. This ensures that the parentActor is set up before any physics processing occurs, allowing the component to apply random movement to the parent actor correctly.
void RandomMovementComponent::_ready() {
    parentActor = Object::cast_to<Node3D>(get_parent());
}

// This method is called when the node receives a notification. In this case, it listens for the NOTIFICATION_WM_CLOSE_REQUEST notification, which is sent when the window is requested to close. When this notification is received, the component calls queue_free() to free itself from memory, ensuring proper cleanup.
void RandomMovementComponent::_notification(int p_what) {

    // Check if the notification is a window close request

    if (p_what == NOTIFICATION_WM_CLOSE_REQUEST) {
        // Free the component from memory when the window is requested to close
        parentActor->queue_free();
        parentActor=nullptr;
    }
}

// This method is called every physics frame. It applies a random offset to the parent actor's position if enabled.
void RandomMovementComponent::_physics_process(double delta) {

    // Local variable to store the random offset for this frame. This is calculated each frame when enabled and applied to the parent actor's position.
    Vector3 offset;

    // Static variables to store the offset and original position across frames. 
    // This allows us to reset the position after applying the random offset.
    static Vector3 parentActorPosition;
    static bool forthComingOffset = true;

    // Check if the component is enabled and if the parent actor is valid before applying random movement.
    // This ensures that we only apply random movement when the component is active and has a valid target to move.
    if (_isEnabled && parentActor != nullptr) {

        // If forthComingOffset is true, generate a new random offset. Otherwise, reset the position to the original.
        if (forthComingOffset) {

            // Store the original position of the parent actor before applying the random offset. 
            // This allows us to reset the position in the next frame if needed.
            parentActorPosition = parentActor->get_position();

            // Generate a random offset within the specified movement radius. 
            // The rng->randf_range(-1.0, 1.0) generates a random float between -1.0 and 1.0, which is then multiplied by the movement radius to scale it appropriately.
            offset.x = rng->randf_range(-1.0, 1.0) * _movementRadius;
            offset.y = rng->randf_range(-1.0, 1.0) * _movementRadius;
            offset.z = rng->randf_range(-1.0, 1.0) * _movementRadius;

            // Apply the random offset to the parent actor's position. 
            // This moves the parent actor to a new position based on the original position plus the random offset.
            parentActor->set_position(parentActorPosition + offset);

        } else {

            // Reset the parent actor's position to the original position before applying the random offset. 
            // This ensures that the random movement is temporary and can be reset in the next frame if needed.
            parentActor->set_position(parentActorPosition);
        }

        // Toggle for the next frame
        forthComingOffset = !forthComingOffset;

    } else {

        // If the component is not enabled or the parent actor is invalid, ensure the position is reset to the original if it was previously offset.
        if (!forthComingOffset && parentActor != nullptr) {

            // Reset the parent actor's position to the original position if it was previously offset. This ensures that when the component is disabled, the parent actor returns to its original position.
            parentActor->set_position(parentActorPosition);

            // Reset to generate a new offset when enabled again
            forthComingOffset = true; 
        }
    }
}