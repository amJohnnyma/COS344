#include "Physics.h"
#include "Camera.h"
#include "Renderer.h"

template <int n>
class Scene {
    std::vector<std::unique_ptr<Shape<n>>> staticShapes;  // Walls
    std::vector<PhysicsBody*> dynamicBodies;  // Ball
    Camera<n> camera;
public:
    void addWall(std::unique_ptr<Shape<n>> wall);
    void addBall(PhysicsBody* ball);
    void update(float dt, PhysicsEngine& phys);
    void render(Renderer<n>& r);
};
