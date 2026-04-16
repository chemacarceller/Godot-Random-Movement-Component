// random_movement.h
// This component applies a random movement to its parent actor (Node3D) every physics frame when enabled.
#ifndef RANDOM_MOVEMENT_COMPONENT_H
#define RANDOM_MOVEMENT_COMPONENT_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

// Using the godot namespace to avoid prefixing all Godot classes with 'godot::'
using namespace godot;

// The RandomMovementComponent class inherits from Node, allowing it to be added as a child to any Node in the scene tree.
class RandomMovementComponent : public Node {

    // Macro to enable Godot's class system and reflection
    GDCLASS(RandomMovementComponent, Node)

    private:

        // Underscored private variable for exported variables (following Godot's convention)
        bool _isEnabled = true;
        float _movementRadius = 0.05f;

        // Private variables for internal use without exposing to Godot not underscored
        // Store the original position of the parent actor to reset after applying the random offset
        Node3D* parentActor = nullptr;
    
        // Efficient to keep one instance of RNG
        // Ref<RandomNumberGenerator> is used to manage the lifecycle of the RNG instance, ensuring it is properly reference-counted and freed when no longer needed.
        Ref<RandomNumberGenerator> rng;

    protected:
        // Method to bind properties and methods to Godot
        static void _bind_methods();

    public:
        // Constructor and Destructor
        RandomMovementComponent();
        ~RandomMovementComponent();

        // Overridden methods from Node
        void _ready() override;
        void _physics_process(double delta) override;
        void _notification(int p_what);

        // Getters and Setters for Godot Binding
        void set_is_enabled(bool p_value) { _isEnabled = p_value; }
        bool get_is_enabled() const { return _isEnabled; }
    
        void set_movement_radius(float p_radius) { _movementRadius = p_radius; }
        float get_movement_radius() const { return _movementRadius; }
};

#endif