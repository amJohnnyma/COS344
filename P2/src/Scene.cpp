#include "../include/engine/Scene.h"
#include "../include/math/Square.h"
#include "../include/math/Circle.h"
#include "../include/math/Triangle.h"
#include <cstdlib>

    template <int n>
void Scene<n>::addSquare(const ShapeParams<n>& params) 
{
    Square<3>* square = new Square<n>(params.pos, params.height, params.width);
    square->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(square))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addCircle(const ShapeParams<n>& params)
{

    Circle<3> * circle = new Circle(params.pos, params.radius);
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
}
    template <int n>
void Scene<n>::addTriangle(const ShapeParams<n>& params)
{

    Vector<3> p1 = {params.pos[0], params.pos[1] + params.height * 0.667f, params.pos[2]};
    Vector<3> p2 = {params.pos[0] - params.width * 0.5f, params.pos[1] - params.height * 0.333f, params.pos[2]};
    Vector<3> p3 = {params.pos[0] + params.width * 0.5f, params.pos[1] - params.height * 0.333f, params.pos[2]};

    Triangle<3>* triangle = new Triangle<n>(p1,p2,p3);
    triangle->setColor(params.col.r, params.col.g, params.col.b);

    int idx = m_shapeCount;
    if(addShape(triangle))
    {
        m_shape_params[idx] = params;
    }
}
    template <int n>
void Scene<n>::addBall(const ShapeParams<n>& params)
{

    Circle<3>* ball = new Circle<3>(params.pos, params.radius);
    ball->setColor(params.col.r, params.col.g, params.col.b);
    ball->enablePhysics();
    ball->setPhysicsType(PhysicsType::BALL);

    ballIndex = getCount();

    int idx = m_shapeCount;
    if(addShape(ball))
    {
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
    selectedObjectCol.r = selectedObjectCol.r * 0.5f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.5f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.5f + 0.5f;
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
    selectedObjectCol.r *= selectedObjectCol.r * 0.5f + 0.5f;
    selectedObjectCol.g *= selectedObjectCol.g * 0.5f + 0.5f;
    selectedObjectCol.b *= selectedObjectCol.b * 0.5f + 0.5f;
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
        m_shapes[selectedObstacle]->applyTranslation(force);
    }
}
    template <int n>
void Scene<n>::scaleSelected(const float& scale)
{
    if(selectedObstacle != -1)
    {
       m_shapes[selectedObstacle]->scale(scale);
    }
}
    template <int n>
void Scene<n>::rotateSelected(const float& theta)
{
    if(selectedObstacle != -1)
    {
        m_shapes[selectedObstacle]->rotate(theta);
        m_shape_params[selectedObstacle].rotation += theta;
    }
}

template class Scene<3>;
