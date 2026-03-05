#include "../include/engine/SceneCreator.h"
#include <cstdio>
#include <string.h>


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
    std::string path = courseName + ".txt";
    FILE * f = fopen(path.c_str(), "r");
    if(!f) return;

    char line[256];
    Scene<3> * current = nullptr;

    while (fgets(line, sizeof(line), f))
    {
        char cmd[32];
        sscanf(line, "%s", cmd);

        if (strcmp(cmd, "scene") == 0)
        {
            createScene();
            int idx;
            sscanf(line, "scene %d", &idx);
            setActive(idx);
            current = getScene(idx);
        }
        else if (strcmp(cmd, "shape") == 0)
        {
            ShapeParams<3> p;
            float x,y,z;
            sscanf(line, "shape %d %f %f %f %f %f %f %f %f %f %f %d %f",
                    &p.type,
                    &x, &y, &z,
                    &p.width, &p.height, &p.radius,
                    &p.col.r, &p.col.g, &p.col.b, &p.col.a,
                    (int*)&p.isHole,
                    &p.rotation);
            p.pos = Vector<3>{x, y, z};
            float savedRotation = p.rotation;
            p.rotation = 0.f;          // zero it so addShape stores 0
            createShape(p);
            if (savedRotation != 0.f) {
                int idx = current->getCount() - 1;
                current->getShape(idx)->rotate(savedRotation);
                current->getShapeParamRef(idx).rotation = savedRotation; // store it back
            }

        }
        else if (strcmp(cmd, "selectObstacle") == 0 && current) {
            current->selectObstacle();
        }
        else if (strcmp(cmd, "selectBall") == 0 && current) {
            current->selectGolfBall();
        }
        else if (strcmp(cmd, "selectHole") == 0 && current) {
            current->selectHole();
        }
        else if (strcmp(cmd, "rotate") == 0 && current) {
            float theta;
            sscanf(line, "rotate %f", &theta);
            current->rotateSelected(theta);
        }
        else if (strcmp(cmd, "scale") == 0 && current) {
            float s;
            sscanf(line, "scale %f", &s);
            current->scaleSelected(s);
        }
        else if (strcmp(cmd, "ballvel") == 0 && current) {
            float x, y, z;
            sscanf(line, "ballvel %f %f %f", &x, &y, &z);
            current->setBallVel(Vector<3>{x, y, z});
        }
    }
    fclose(f);
}

    template <int n>
void SceneCreator<n>::saveScenes(const std::string courseName)
{
    std::string path = courseName + ".txt";
    FILE* f = fopen(path.c_str(), "w");
printf("Saving to: %s\n", path.c_str());
    if(!f)
    {
        printf("Failed to open file: %s\n", path.c_str());
        return;
    }

    for (int i =0; i < m_sceneCount; i ++)
    {
        fprintf(f, "scene %d\n", i);
        Scene<n>* scene = getScene(i);
        for (int j = 0; j < scene->getCount(); j ++)
        {
            ShapeParams<n> p = scene->getShapeParam(j);
            fprintf(f, "shape %d %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %d %.4f\n",
                    p.type,
                    p.pos[0], p.pos[1], p.pos[2],
                    p.width, p.height, p.radius,
                    p.col.r, p.col.g, p.col.b, p.col.a,
                    (int)p.isHole,
                    p.rotation);
        }
        Vector<n> bv = scene->getBallVel();
        fprintf(f, "ballvel %.4f %.4f %.4f\n", bv[0], bv[1], bv[2]);
    }
    fclose(f);
}


template class SceneCreator<3>;
