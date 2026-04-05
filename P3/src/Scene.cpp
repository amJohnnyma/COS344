#include "../include/engine/Scene.h"
#include "../include/math/Square.h"
#include "../include/math/Circle.h"
#include "../include/math/Triangle.h"
#include "../include/math/Triangular_Prism.h"
#include "../include/math/Cuboid.h"
#include "../include/math/Cone.h"
#include "../include/math/Cylinder.h"

    template <int n>
void Scene<n>::addSquare(const ShapeParams<n>& params) 
{

    Square<3>* square = new Square<3>(params.pos, params.height, params.depth);
    square->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(square))
    {
        m_shape_params[idx] = params;
    }
}

    template <int n>
void Scene<n>::addCuboid(const ShapeParams<n>& params) 
{

    float hw = params.width * 0.5f;
    float hh = params.height * 0.5f;
    float hd = params.depth* 0.5f;

    Cuboid<3>* square = new Cuboid<3>(params.pos, hw,hh,hd);
    square->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(square))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addCone(const ShapeParams<n>& params) 
{

    /*const Vector<n>& apex_pt,
      const Vector<n>& base_centre,
      float r,
      int segs = 32 */

    Vector<n> base_centre = params.pos + Vector<n>{0.f, params.height/-2.f, 0.f};
    Vector<n> apex= params.pos + Vector<n>{0.f, params.height/2.f, 0.f};

    Cone<3>* cone = new Cone(apex, base_centre, params.radius, params.segment);
    cone->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(cone))
    {
        m_shape_params[idx] = params;
    }
}

    template <int n>
void Scene<n>::addCylinder(const ShapeParams<n>& params) 
{
    /*

       Cylinder(const Vector<n>& top_centre,
       const Vector<n>& bot_centre,
       float r,
       int segs = 32);
       */

    Vector<n> top_centre = params.pos + Vector<n>{0.f,params.height / 2.f,0.f};
    Vector<n> bottom_centre = params.pos + Vector<n>{0.f,-params.height / 2.f,0.f};
    Cylinder<3>* cylinder= new Cylinder<3>(top_centre, bottom_centre, params.radius, params.segment);
    cylinder->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(cylinder))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addCircle(const ShapeParams<n>& params)
{
    /*

       Circle<3> * circle = new Circle(params.pos, params.radius, params.cSegments);
       circle->setColor(params.col.r, params.col.g, params.col.b);

       if(params.isHole)
       {
       holeIndex = getCount();
       }

       int idx = m_shapeCount;
       if(addShape(circle))
       {
       m_shape_params[idx] = params;
       }
       */
}
    template <int n>
void Scene<n>::addTriangle(const ShapeParams<n>& params)
{
    // std::cout << "Adding triangle" << std::endl;
    //params.print();

    // Create an equilateral-ish triangle centered on params.pos
    float hw = params.width * 0.5f;
    float hh = params.height * 0.5f;

    float apex   =  params.height * (1.5f / 3.0f);
    float base_y = -params.height * (1.5f / 3.0f);

    Vector<3> p1 = { params.pos[0],          params.pos[1] + apex, params.pos[2] };
    Vector<3> p2 = { params.pos[0] - hw,     params.pos[1] - base_y, params.pos[2] };
    Vector<3> p3 = { params.pos[0] + hw,     params.pos[1] - base_y, params.pos[2] };

    Triangle<3>* triangle= new Triangle<3>(p1, p2, p3);

    triangle->setColor(params.col.r, params.col.g, params.col.b);

    // Apply initial rotation if any
    if (params.rot[0] != 0.0f || params.rot[1] != 0.0f || params.rot[2] != 0.0f)
    {
        triangle->rotate3D(params.rot);
    }

    int idx = m_shapeCount;
    if (addShape(triangle))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
    }
}

    template <int n>
void Scene<n>::addTriangularPrism(const ShapeParams<n>& params)
{
    // std::cout << "Adding triangle" << std::endl;
    //params.print();

    // Create an equilateral-ish triangle centered on params.pos
    float hw  = params.width * 0.5f;
    float hh  = params.height * 0.5f;
    float z   = params.pos[2] + params.depth / 2;

    // Centroid sits 1/3 up from base, 2/3 down from apex
    // So apex is +2/3*height above centre, base is -1/3*height below
    float apex   =  params.height * (1.5f / 3.0f);
    float base_y = -params.height * (1.5f / 3.0f);

    Vector<3> p1 = { params.pos[0],       params.pos[1] + apex,   z };  // top
    Vector<3> p2 = { params.pos[0] - hw,  params.pos[1] + base_y, z };  // bottom-left
    Vector<3> p3 = { params.pos[0] + hw,  params.pos[1] + base_y, z };  // bottom-right
    TriangularPrism<3>* prism = new TriangularPrism<3>(p1, p2, p3, params.depth);

    prism->setColor(params.col.r, params.col.g, params.col.b);

    // Apply initial rotation if any
    if (params.rot[0] != 0.0f || params.rot[1] != 0.0f || params.rot[2] != 0.0f)
    {
        prism->rotate3D(params.rot);
    }

    int idx = m_shapeCount;
    if (addShape(prism))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addBall(const ShapeParams<n>& params)
{
    /*

       Circle<3>* ball = new Circle<3>(params.pos, params.radius, params.cSegments);
       ball->setColor(params.col.r, params.col.g, params.col.b);
       ball->enablePhysics();
       ball->setPhysicsType(PhysicsType::BALL);

       ballIndex = getCount();

       int idx = m_shapeCount;
       if(addShape(ball))
       {
       m_shape_params[idx] = params;
       }
       */
}

    template <int n>
void Scene<n>::selectGolfBall()
{
    // reset colour from selected
    if (ballIndex < 0 || ballIndex >= m_shapeCount) return;

    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b);
    // Now set the colour of the selected one
    selectedObstacle = ballIndex;
    selectedObjectCol = m_shape_params[selectedObstacle].col;
    selectedObjectCol.r = selectedObjectCol.r * 0.25f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.25f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.25f + 0.5f;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b);
}
    template <int n>
void Scene<n>::selectObstacle()
{

    // reset colour from selected
    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b);

    int attempts = 0;
    int index;
    do
    {
        index = rand() % m_shapeCount;
        attempts++;
        if(attempts > m_shapeCount * 2) return; // luck isnt on our side today // will defnitely happen when there are only a few shapes
    }
    while (index == ballIndex || index == holeIndex || index == selectedObstacle);

    selectedObstacle = index;

    selectedObjectCol = m_shape_params[selectedObstacle].col;
    selectedObjectCol.r *= selectedObjectCol.r * 0.5f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.5f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.5f + 0.5f;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b);

} 
    template <int n>
void Scene<n>::selectObstacle(int index)
{
    // reset colour from selected
    if (index < 0 || index >= m_shapeCount) return;

    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b);
    selectedObstacle = index;

    selectedObjectCol = m_shape_params[selectedObstacle].col;
    selectedObjectCol.r *= selectedObjectCol.r * 0.5f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.5f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.5f + 0.5f;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b);
}
    template <int n>
void Scene<n>::update(double dt, Renderer<n>& renderer)
{
    // printf("Shape count: %d\n", m_shapeCount);
    //for (int i = 0; i < m_shapeCount; i++)
    //   printf("  shape[%d] hasPhysics=%d type=%d\n", i, m_shapes[i]->physicsBodyActive(), m_shapes[i]->getPhysicsType());
    if(!paused)
    {
        physics.update(getBodies(), m_shapeCount, dt);
    }

    for (int i = 0; i < m_shapeCount; i++)
    {
        m_shapes[i]->render(renderer);
        if (m_shapes[i]->physicsBodyActive())
        {
            m_shape_params[i].pos = m_shapes[i]->getPosition();
        }

    }


}
    template <int n>
void Scene<n>::selectHole()
{

    // reset colour from selected
    if (holeIndex< 0 || holeIndex>= m_shapeCount) return;

    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b);
    selectedObstacle = holeIndex;

    selectedObjectCol = m_shape_params[selectedObstacle].col;
    selectedObjectCol.r *= selectedObjectCol.r * 0.25f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.25f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.25f + 0.5f;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b);
}
    template <int n>
void Scene<n>::deselectObject()
{


    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b);
    selectedObstacle = -1;
}

    template <int n>
void Scene<n>::moveSelected(const Vector<n>& force)
{
    if(selectedObstacle != -1)
    {
        // not actually applying force because my physics sucks
        if (selectedObstacle == ballIndex)
        {
            // dont add to current val just overrride current
            //Vector<n> vel = m_shapes[selectedObstacle]->getVelocity();
            Vector<n> totalVel = force;
            totalVel = totalVel *  9.f; // move it fast like a golf ball
            m_shapes[selectedObstacle]->setVelocity(totalVel);
            return;
        }
        m_shapes[selectedObstacle]->applyTranslation(force);
        m_shape_params[selectedObstacle].pos = m_shapes[selectedObstacle]->getPosition();
    }
}
    template <int n>
void Scene<n>::scaleSelected(const float& scale)
{
    /*
       if(selectedObstacle != -1)
       {
       m_shapes[selectedObstacle]->scale(scale);
       m_shape_params[selectedObstacle].width *= scale;
       m_shape_params[selectedObstacle].height *= scale;
       m_shape_params[selectedObstacle].radius *= scale;
       }
       */
}
    template <int n>
void Scene<n>::rotateSelected(const float& theta)
{
    /*
       if(selectedObstacle != -1)
       {
       m_shapes[selectedObstacle]->rotate(theta);
       m_shape_params[selectedObstacle].rotation += theta;
       }
       */
}

    template <int n>
void Scene<n>::rotateScene(const Vector<n>& angles)
{
    Vector<n> rotate_point;
    for(int i = 0; i < m_shapeCount; i++)
    {
        rotate_point = rotate_point + m_shapes[i]->getPosition();
    }

    rotate_point = rotate_point * (1.f/(float)(m_shapeCount + 1));

    for (int i = 0; i < m_shapeCount; i++)
    {
        m_shapes[i]->rotate(angles, rotate_point, true);

    }
}

template class Scene<3>;
