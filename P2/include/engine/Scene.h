#include "Physics.h"
#include "Camera.h"
#include "Renderer.h"
#include "../math/Shape.h"
#include "../math/Vector.h"
#include "Color.h"
#include "Physics.h"

// This kind of acts like a shape factory aswell
// Make specific shapes and they get assigned the correct values and stuff
template <int n>
struct ShapeParams
{
    int type;
    // square = 1
    // circle = 2
    // triangle = 3
    // ball = 4
    Vector<n> pos;
    float width, height; 
    float radius; 
    Color4 col;

    bool isHole = false;
};

#define MAX_SHAPES 128

template <int n>
class Scene {
    private:

        Shape<n>* m_shapes[MAX_SHAPES];
        int m_shapeCount;
        int m_activeIndex;

        std::string name;
        Camera<n> camera;

        int ballIndex; // there can only be one ball
        int selectedObstacle = -1;
        int holeIndex; // also only one hole

        PhysicsEngine physics;
        bool paused = false;
    
    
    public:
        Scene<n>() : m_shapeCount(0), m_activeIndex(0), ballIndex(-1), holeIndex(-1) {}
        ~Scene<n>()
        {
            for (int i = 0; i < m_shapeCount; i++)
                delete m_shapes[i];
        }
        void update(double dt, Renderer<n> & r); // this updates the scene like main.cpp currently does it
        void togglePause() {paused = !paused;}


        void addShape(Shape<n> * shape)
        {
            if (m_shapeCount >= MAX_SHAPES){delete shape; return;}
            m_shapes[m_shapeCount++] = shape;
        }

        Shape<n>** getBodies()
        {
            return m_shapes;
        }

        Shape<n>* getActive() {return m_shapes[m_activeIndex];}
        void setActive(int index) {if(index < m_shapeCount) m_activeIndex = index; }
        int getCount() const {return m_shapeCount;}
        Shape<n>* getShape(int index) {return (index < m_shapeCount) ? m_shapes[index] : nullptr;}

        void addSquare(const ShapeParams<n>& params);
        void addCircle(const ShapeParams<n>& params);
        void addTriangle(const ShapeParams<n>& params);
        void addBall(const ShapeParams<n>& params);

        void selectGolfBall();
        void selectObstacle(); // select an obstacle (if already selected then select another different one)
        void selectHole();
        void deselectObject();

        void moveSelected(const Vector<n>& force);
        void scaleSelected(const float& scale);
        void rotateSelected(const float& theta);

        void setBallVel(const Vector<n> & force) {m_shapes[ballIndex]->setVelocity(force);}


        
};
