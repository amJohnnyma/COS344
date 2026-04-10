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
    float width, height, depth; 
    Color4 col;
    Vector<n> rot;
    float radius;
    int segment;
void print() const
    {
        std::cout << "ShapeParams:\n";
        std::cout << "  Type     : ";
        switch(type) {
            case 1: std::cout << "Square"; break;
            case 2: std::cout << "Circle"; break;
            case 3: std::cout << "Triangle"; break;
            case 4: std::cout << "Ball"; break;
            default: std::cout << "Unknown(" << type << ")"; break;
        }
        std::cout << " (" << type << ")\n";

        std::cout << "  Position : ";
        pos.print();

        std::cout << "  Size     : width=" << width 
                  << " height=" << height 
                  << " depth=" << depth << "\n";

        std::cout << "  Color    : (" << col.r << ", " << col.g 
                  << ", " << col.b << ", " << col.a << ")\n";

        std::cout << "  Rotation : ";
        rot.print();
        std::cout << "\n";
    }

};

#define MAX_SHAPES 128

template <int n>
class Scene {
    private:

        Shape<n>* m_shapes[MAX_SHAPES];
        ShapeParams<n> m_shape_params[MAX_SHAPES];
        int m_shapeCount;
        int m_activeIndex;

        std::string name;

        int ballIndex = -1; // there can only be one ball
        int selectedObstacle = -1;
        int holeIndex = -1; // also only one hole

        PhysicsEngine physics;
        bool paused = false;
        Color4 selectedObjectCol;
        Color4 originalSelectedObjectCol;

        Vector<n> rotate_point = {};


    public:
        Scene<n>() : m_shapeCount(0), m_activeIndex(0), ballIndex(-1), holeIndex(-1) {}
        ~Scene<n>()
        {
            for (int i = 0; i < m_shapeCount; i++)
                delete m_shapes[i];
        }
        void update(double dt, Renderer<n> & r); 
        void togglePause() {paused = !paused;}
        void setPause(bool p) {paused = p;}
        bool isPaused() {return paused;}


        bool addShape(Shape<n> * shape)
        {
            if (m_shapeCount >= MAX_SHAPES){delete shape; return false;}
            m_shapes[m_shapeCount++] = shape;
            return true;
        }

        Shape<n>** getBodies()
        {
            return m_shapes;
        }

        Shape<n>* getActive() {return m_shapes[m_activeIndex];}
        void setActive(int index) {if(index < m_shapeCount) m_activeIndex = index; }
        int getCount() const {return m_shapeCount;}
        Shape<n>* getShape(int index) {return (index < m_shapeCount) ? m_shapes[index] : nullptr;}
        ShapeParams<n> getShapeParam(int index) {return index < m_shapeCount ? m_shape_params[index] : ShapeParams<n>();}
        ShapeParams<n>& getShapeParamRef(int index) { return m_shape_params[index]; }

        void addSquare(const ShapeParams<n>& params);
        void addCircle(const ShapeParams<n>& params);
        void addTriangle(const ShapeParams<n>& params);
        void addTriangularPrism(const ShapeParams<n>& params);
        void addBall(const ShapeParams<n>& params);
        void addCylinder(const ShapeParams<n>& params); 
        void addCone(const ShapeParams<n>& params); 
        void addCuboid(const ShapeParams<n>& params); 
        void addSphere(const ShapeParams<n>& params); 

        void selectGolfBall();
        void selectObstacle(); // select an obstacle (if already selected then select another different one)
        void selectHole();
        void deselectObject();
        void selectObstacle(int index);

        void moveSelected(const Vector<n>& force);
        void scaleSelected(const float& scale);
        void rotateSelected(const float& theta);
        void rotateScene(const Vector<n>& angles);

        void setBallVel(const Vector<n> & force) {m_shapes[ballIndex]->setVelocity(force);}
        Vector<n> getBallVel() {
            if (ballIndex >= 0 && ballIndex < m_shapeCount) {
                return m_shapes[ballIndex]->getVelocity();
            }
            return Vector<n>{}; // zero vector as default
        }

        void rotateWindmillBlade(float speed);
        void updateRotationPoint();





};
