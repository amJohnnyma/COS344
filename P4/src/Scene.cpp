#include "../include/engine/Scene.h"
#include "../include/math/Square.h"
#include "../include/math/Circle.h"
#include "../include/math/Triangle.h"
#include "../include/math/Triangular_Prism.h"
#include "../include/math/Cuboid.h"
#include "../include/math/Cone.h"
#include "../include/math/Cylinder.h"
#include "../include/math/Sphere.h"
#include "../include/math/PointLight.h"
#include <cmath>

    template <int n>
void Scene<n>::addSquare(const ShapeParams<n>& params) 
{

    Square<3>* square = new Square<3>(params.pos, params.height, params.depth);
    square->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    square->setShapeType(1);

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
    square->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    square->setShapeType(5);

    if (floorIndex== -1)
    {
        floorIndex = getCount();
    }

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
    cone->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    cone->setShapeType(6);

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
    cylinder->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    cylinder->setShapeType(7);

    int idx = m_shapeCount;
    if(addShape(cylinder))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addCircle(const ShapeParams<n>& params)
{

    Circle<3> * circle = new Circle(params.pos, params.radius, params.segment);
    circle->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    circle->setShapeType(2);


    int idx = m_shapeCount;
    if(addShape(circle))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addTriangle(const ShapeParams<n>& params)
{
    // std::cout << "Adding triangle" << std::endl;
    //params.print();

    // Create an equilateral-ish triangle centered on params.pos
    float hw = params.width * 0.5f;

    float apex   =  params.height * (1.5f / 3.0f);
    float base_y = params.height * (1.5f / 3.0f);

    Vector<3> p1 = { params.pos[0],          params.pos[1] + apex, params.pos[2] };
    Vector<3> p2 = { params.pos[0] - hw,     params.pos[1] - base_y, params.pos[2] };
    Vector<3> p3 = { params.pos[0] + hw,     params.pos[1] - base_y, params.pos[2] };

    Triangle<3>* triangle= new Triangle<3>(p1, p2, p3);

    triangle->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    triangle->setShapeType(8);


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

    prism->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    prism->setShapeType(3);


    int idx = m_shapeCount;
    if (addShape(prism))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
    }
}

    template <int n>
void Scene<n>::addSphere(const ShapeParams<n>& params)
{

    Sphere<3>* sphere = new Sphere<3>(params.pos, params.radius, params.segment);

    sphere->setColor(params.col.r, params.col.g, params.col.b, params.col.a);

    sphere->setShapeType(9);

    if (ballIndex == -1)
    {
        ballIndex = getCount();
    }


    int idx = m_shapeCount;
    if (addShape(sphere))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
        m_shape_params[idx].type = 9;

    }

}

    template <int n>
void Scene<n>::addPointLight(const ShapeParams<n>& params)
{
    PointLight<3>* pl = new PointLight<3>(params.pos, params.pos, params.radius);
    lightIndex = getCount();
    pl->setShapeType(10);

    int idx = m_shapeCount;
    if (addShape(pl))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
    }
}


    template <int n>
void Scene<n>::addBall(const ShapeParams<n>& params)
{

    Sphere<3>* sphere = new Sphere<3>(params.pos, params.radius, params.segment);

    sphere->setColor(params.col.r, params.col.g, params.col.b, params.col.a);
    sphere->enablePhysics(Vector<3>{0.f, 0.f, -20.f});
    sphere->setPhysicsType(PhysicsType::BALL);
    ballIndex = getCount();
    sphere->setShapeType(4);


    int idx = m_shapeCount;
    if (addShape(sphere))
    {
        //std::cout << "Index: " << idx << std::endl;
        m_shape_params[idx] = params;
    }
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
    selectedObjectCol.r = selectedObjectCol.r;
    selectedObjectCol.g = selectedObjectCol.g;
    selectedObjectCol.b = selectedObjectCol.b;
    selectedObjectCol.a = selectedObjectCol.a;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b, selectedObjectCol.a);
}
    template <int n>
void Scene<n>::update(double dt, Renderer<n>& renderer)
{
    // printf("Shape count: %d\n", m_shapeCount);
    //for (int i = 0; i < m_shapeCount; i++)
    //   printf("  shape[%d] hasPhysics=%d type=%d\n", i, m_shapes[i]->physicsBodyActive(), m_shapes[i]->getPhysicsType());
    if(!paused)
    {
        //physics.update(getBodies(), m_shapeCount, m_shapes[ballIndex], dt);
    }

    for (int i = 0; i < m_shapeCount; i++)
    {
        if(m_shape_params[i].isCutout)
        {
            if(i + 1 < m_shapeCount)
            {
                renderer.beginStencilMask();
                m_shapes[i+1]->render(renderer);

                renderer.beginStencilCutout();
                m_shapes[i]->render(renderer);

                renderer.endStencilMask();
                m_shapes[i + 1]->render(renderer);

                renderer.endStencilCutout();
            }
            continue;

        }
        bool prevIsCutout = (i - 1 >= 0) && m_shape_params[i-1].isCutout;
        if(prevIsCutout) continue;

        m_shapes[i]->render(renderer);
        if (m_shapes[i]->physicsBodyActive())
        {
            m_shape_params[i].pos = m_shapes[i]->getPosition();
            // Rotate ball based on horizontal velocity
            if (i == ballIndex)
            {
                Vector<3> vel = m_shapes[i]->getVelocity();
                // Roll along XZ plane — axis is perpendicular to movement direction
                Vector<3> moveDir = {vel[0], 0.f, vel[2]};
                float speed = std::sqrt(vel[0]*vel[0] + vel[2]*vel[2]);
                if (speed > 0.01f)
                {
                    // Rotation axis is perpendicular to movement in XZ (cross with up)
                    Vector<3> axis = {vel[2], 0.f, -vel[0]};
                    float axisLen = std::sqrt(axis[0]*axis[0] + axis[2]*axis[2]);
                    axis = axis * (1.f / axisLen);

                    // angle = distance / radius, but per frame so use speed * dt
                    // you don't have dt here so use a scale factor
                    float angle = (speed / m_shape_params[i].radius) * dt; // approx if 60fps, pass dt if you can
                    m_shapes[i]->rotateAroundAxis(axis, angle, m_shapes[i]->getPosition());
                }
            }
        }

    }


}
    template <int n>
void Scene<n>::selectHole()
{

    // reset colour from selected
    if (holeIndex< 0 || holeIndex>= m_shapeCount) return;

    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b, originalSelectedObjectCol.a);
    selectedObstacle = holeIndex;

    selectedObjectCol = m_shape_params[selectedObstacle].col;
    selectedObjectCol.r *= selectedObjectCol.r * 0.25f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.25f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.25f + 0.5f;
    selectedObjectCol.a = selectedObjectCol.a;
    originalSelectedObjectCol = m_shape_params[selectedObstacle].col;
    m_shapes[selectedObstacle]->setColor(selectedObjectCol.r, selectedObjectCol.g, selectedObjectCol.b, selectedObjectCol.a);
}
    template <int n>
void Scene<n>::deselectObject()
{


    if (selectedObstacle >= 0 && selectedObstacle < m_shapeCount)
        m_shapes[selectedObstacle]->setColor(originalSelectedObjectCol.r, originalSelectedObjectCol.g, originalSelectedObjectCol.b, originalSelectedObjectCol.a);
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
    if(selectedObstacle != -1)
    {
        auto& shape = m_shapes[selectedObstacle];
        if(auto * sphere = dynamic_cast<Sphere<3>*>(m_shapes[selectedObstacle]))
        {
            m_shapes[selectedObstacle]->addSegments((int)(scale));
            m_shape_params[selectedObstacle].segment += (int)(scale);
        }
        else if (auto * cuboid = dynamic_cast<Cuboid<3>*>(m_shapes[selectedObstacle]))
        {
            cuboid->splitVertices((int)(scale));
        }
        else
        {
        }
    }
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
    // this is slow

    for (int i = 0; i < m_shapeCount; i++)
    {
        m_shapes[i]->rotate(angles, rotate_point, true);
        // Sync physics body position
        if (m_shapes[i]->physicsBodyActive())
        {
            m_shape_params[i].pos = m_shapes[i]->getPosition();
            m_shapes[i]->setPosition(m_shapes[i]->getPosition()); // flush to physics body
        }

    }
}


    template <int n>
void Scene<n>::updateRotationPoint()
{
    for(int i = 0; i < m_shapeCount; i++)
    {
        rotate_point = rotate_point + m_shapes[i]->getPosition();
    }

    rotate_point = rotate_point * (1.f/(float)(m_shapeCount));
}

    template <int n>
void Scene<n>::translateScene(const Vector<n>& translation)
{

    for (int i = 0; i < m_shapeCount; i++)
    {
        m_shapes[i]->applyTranslation(translation);
        // Sync physics body position
        if (m_shapes[i]->physicsBodyActive())
        {
            m_shape_params[i].pos = m_shapes[i]->getPosition();
            m_shapes[i]->setPosition(m_shapes[i]->getPosition()); // flush to physics body
        }

    }

}

    template <int n>
void Scene<n>::changeAlpha(float diff)
{

    if(selectedObstacle == -1) return;

    Vector<4> col = m_shapes[selectedObstacle]->getColor();
    m_shapes[selectedObstacle]->setColor(col[0], col[1], col[2], col[3] + diff);
    m_shape_params[selectedObstacle].col = {col[0], col[1], col[2], col[3] + diff};

    originalSelectedObjectCol = {col[0], col[1], col[2], col[3] + diff};
}

    template <int n>
void Scene<n>::cycleColorSelected(int dir)
{
    if(selectedObstacle == -1) return;

    if(dir > 0)
    {
        // cycle left
        if (selectedObstacle == ballIndex)
        {
            ballCol.cycleRight();
            m_shapes[selectedObstacle]->setColor(ballCol.current().r,ballCol.current().g,ballCol.current().b,ballCol.current().a );
            m_shape_params[selectedObstacle].col = ballCol.current();
            originalSelectedObjectCol = ballCol.current();

        }
        else if (selectedObstacle == floorIndex)
        {
            floorCol.cycleRight();
            m_shapes[selectedObstacle]->setColor(floorCol.current().r, floorCol.current().g, floorCol.current().b, floorCol.current().a);
            m_shape_params[selectedObstacle].col = floorCol.current();
            originalSelectedObjectCol = floorCol.current();

        }
        else if (selectedObstacle == lightIndex)
        {
            lightCol.cycleRight();

            m_shape_params[lightIndex].col= lightCol.current();

            originalSelectedObjectCol = lightCol.current();

        }
    }
    else if (dir < 0)
    {
        //cycle right
        if (selectedObstacle == ballIndex)
        {
            ballCol.cycleLeft();
            m_shapes[selectedObstacle]->setColor(ballCol.current().r,ballCol.current().g,ballCol.current().b,ballCol.current().a );
            m_shape_params[selectedObstacle].col = ballCol.current();
            originalSelectedObjectCol = ballCol.current();

        }
        else if (selectedObstacle == floorIndex)
        {
            floorCol.cycleLeft();
            m_shapes[selectedObstacle]->setColor(floorCol.current().r, floorCol.current().g, floorCol.current().b, floorCol.current().a);
            m_shape_params[selectedObstacle].col = floorCol.current();
            originalSelectedObjectCol = floorCol.current();

        }
        else if (selectedObstacle == lightIndex)
        {
            lightCol.cycleLeft();

            m_shape_params[lightIndex].col= lightCol.current();
            originalSelectedObjectCol = lightCol.current();

        }
    }


}

    template <int n>
void Scene<n>::updatePointLight(const Vector<n>& pos,const Vector<n>& col,const float& radius, Renderer<n>& r)
{
    if (lightIndex == -1) return;
    m_shape_params[lightIndex].pos = m_shape_params[lightIndex].pos + pos;
    m_shape_params[lightIndex].col= Color4(col[0], col[1], col[2]);
    m_shape_params[lightIndex].radius= m_shape_params[lightIndex].radius + radius;

    Vector<n> fcol = Vector<n>{m_shape_params[lightIndex].col.r,m_shape_params[lightIndex].col.g,m_shape_params[lightIndex].col.b};

    r.updatePointLight(m_shape_params[lightIndex].pos,fcol , m_shape_params[lightIndex].radius);
}


    template <int n>
void Scene<n>::setPointLight(const Vector<n>& pos,const Vector<n>& col,const float& radius, Renderer<n>& r)
{

    if (lightIndex == -1) return;

    m_shape_params[lightIndex].pos =pos;
    m_shape_params[lightIndex].col= Color4(col[0], col[1], col[2]);
    m_shape_params[lightIndex].radius=radius;

    Vector<n> fcol = Vector<n>{m_shape_params[lightIndex].col.r,m_shape_params[lightIndex].col.g,m_shape_params[lightIndex].col.b};

    r.updatePointLight(m_shape_params[lightIndex].pos,fcol , m_shape_params[lightIndex].radius);


}

template class Scene<3>;
