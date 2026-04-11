#include "../include/engine/SceneCreator.h"
#include <stdio.h>
#include <stdlib.h>

static bool streq(const char * a, const char * b)
{
    while(*a && *b)
    {
        if (*a != *b) return false;
        ++a; ++b;
    }
    return *a == *b;

}

    template <int n>
void SceneCreator<n>::createShape(const ShapeParams<n>& params)
{
    //  std::cout << "Creating shape" << std::endl;
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
            createTriangularPrism(params);
            break;
        case 4:
            createBall(params);
            break;
        case 5:
            createCuboid(params);
            break;
        case 6:
            createCone(params);
            break;
        case 7:
            createCylinder(params);
            break;
        case 8:
            createTriangle(params);
        case 9:
            createSphere(params);
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
void SceneCreator<n>::createSphere(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addSphere(params);

}

    template <int n>
void SceneCreator<n>::createCuboid(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addCuboid(params);

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
void SceneCreator<n>::createTriangularPrism(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addTriangularPrism(params);
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

    template<int n>
void SceneCreator<n>::createCone(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addCone(params);

}

    template<int n>
void SceneCreator<n>::createCylinder(const ShapeParams<n>& params)
{
    Scene<n> * scene = getActive();
    scene->addCylinder(params);

}
    template <int n>
void SceneCreator<n>::loadScenes(const std::string courseName)
{
    std::string path ="../../Saves/" + courseName + ".txt";
    FILE * f = fopen(path.c_str(), "r");
    if(!f) return;

    char line[256];
    Scene<3> * current = nullptr;
    //#t1  type /x     /y     /z     /width /height/depth /col r /col g /col b /col a/ rotx  /roty  /rotz /radius /segments

    while (fgets(line, sizeof(line), f))
    {
        char cmd[32];
        sscanf(line, "%s", cmd);

        if(cmd[0] == '#') continue;
        if (streq(cmd, "scene"))
        {
            createScene();
            int idx;
            sscanf(line, "scene %d", &idx);
            setActive(idx);
            current = getScene(idx);
        }
        else if (streq(cmd, "shape"))
        {
            ShapeParams<3> p = {};
            float x = 0,y=0,z=0;
            int isCutout = 0;
            int count = sscanf(line, "shape %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",
                    &p.type,
                    &x, &y, &z,
                    &p.width, &p.height, &p.depth,
                    &p.col.r, &p.col.g, &p.col.b, &p.col.a,
                    &p.rot[0], &p.rot[1], &p.rot[2],
                    &p.radius, &p.segment,
                    &isCutout
                    );
            p.isCutout = (bool)isCutout;
            if(p.isCutout) {std::cout << "CUTOUT\n\n" << std::endl;}
            p.pos = Vector<3>{x, y, z};
            // float savedRotation = p.rotation;
            // p.rotation = 0.f;          // zero it so addShape stores 0
            if (count >= 11) {   // at least type + pos + size + color
                createShape(p);
            } else {
                std::cout << "Warning: malformed shape line: " << line << std::endl;
            }
            int idx = current->getCount() - 1;
            current->getShape(idx)->rotate(p.rot);
            current->getShape(idx)->setIsCutout(p.isCutout);
            current->getShapeParamRef(idx).rot = p.rot; // store it back

        }
        else if (streq(cmd, "selectObstacle") && current) {
            current->selectObstacle();
        }
        else if (streq(cmd, "selectBall") && current) {
            current->selectGolfBall();
        }
        else if (streq(cmd, "selectHole") && current) {
            current->selectHole();
        }
        else if (streq(cmd, "rotate") && current) {
            float theta;
            sscanf(line, "rotate %f", &theta);
            current->rotateSelected(theta);
        }
        else if (streq(cmd, "scale") && current) {
            float s;
            sscanf(line, "scale %f", &s);
            current->scaleSelected(s);
        }
        else if (streq(cmd, "ballvel") && current) {
            float x, y, z;
            sscanf(line, "ballvel %f %f %f", &x, &y, &z);
            current->setBallVel(Vector<3>{x, y, z});
        }
    }
    fclose(f);
    m_activeIndex = 0;
    setActive(m_activeIndex);

    current->updateRotationPoint();
}

    template <int n>
void SceneCreator<n>::saveScenes(const std::string courseName)
{
    /*
       std::string path ="../../Saves/" + courseName + ".txt";
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
       fprintf(f, "shape %d %.4f %.4f %.4f %.4f %.4f %.4f %d %.4f %.4f %.4f %.4f %d %.4f\n",
       p.type,
       p.pos[0], p.pos[1], p.pos[2],
       p.width, p.height, p.radius, p.cSegments,
       p.col.r, p.col.g, p.col.b, p.col.a,
       (int)p.isHole,
       p.rotation);
       }
       Vector<n> bv = scene->getBallVel();
       fprintf(f, "ballvel %.4f %.4f %.4f\n", bv[0], bv[1], bv[2]);
       }
       fclose(f);
       */
}

    template <int n>
void SceneCreator<n>::selectNextScene()
{
    // go next
    if(m_activeIndex + 1 < m_sceneCount)
    {
        setActive(m_activeIndex + 1);

    }
    // loop around
    else if(m_activeIndex != 0)
    {
        setActive(0);
    }
    // edge case i cant think of
    else
    {
        // do nothing
    }
    std::cout << "Active scene: " << m_activeIndex << std::endl;

}

template <int n>
void SceneCreator<n>::selectPreviousScene()
{

    // go prev
    if(m_activeIndex - 1 >= 0)
    {
    setActive(m_activeIndex - 1);

    }
    // loop around
    else if(m_activeIndex == 0)
    {
    setActive(m_sceneCount - 1);
    }
    // edge case i cant think of
    else
    {
        // do nothing
    }

    std::cout << "Active scene: " << m_activeIndex << std::endl;
}

template class SceneCreator<3>;
