#include "../include/engine/SceneCreator.h"


template <int n>
void SceneCreator<n>::createShape(const ShapeParams<n>& params)
{
    // get the type and make that shape 
    int type = params.type;
    switch(type)
    {
        case 0:
            // none valid
            break;
        case 1:
            createSquare(params);
            break;
        case 2:
            createCircle(params);
            break;
        case 3:
            createTriangle(params);
            break;
        case 4:
            createBall(params);
            break;
        default:
            break;
    }
}

template <int n>
void SceneCreator<n>::createSquare(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addSquare(params);

}

template <int n>
void SceneCreator<n>::update(double dt, Renderer<n>& r)
{
    m_scenes[m_activeIndex].update(dt, r);
}

template <int n>
void SceneCreator<n>::createCircle(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addCircle(params);
}

template <int n>
void SceneCreator<n>::createTriangle(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addTriangle(params);
}

template <int n>
void SceneCreator<n>::createBall(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addBall(params);
} // a circle with enabled physics

template <int n>
void SceneCreator<n>::loadScenes(const std::string courseName)
{
}

template <int n>
void SceneCreator<n>::saveScenes(const std::string courseName)
{
}


template class SceneCreator<3>;
