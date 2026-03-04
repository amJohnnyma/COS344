#ifndef SCENE_CREATOR_H
#define SCENE_CREATOR_H

#include "Scene.h"
#include "Color.h"
#include <cinttypes>

#define MAX_SCENES 18

//this can be used as my scene manager aswell
// Will be integrated with imgui
template <int n>
class SceneCreator
{
    // LOAD/SAVE systems first as concept
    private:
        Scene<n> m_scenes[MAX_SCENES];
        int m_sceneCount;
        int m_activeIndex;

    public:
        SceneCreator() : m_sceneCount(0), m_activeIndex(0) {}

        void update(double dt, Renderer<n> & r);

        bool createScene()
        {
            Scene<n> scene;
            if (m_sceneCount >= MAX_SCENES) return false;
            m_scenes[m_sceneCount++] = scene;
            return true;
        }
        Scene<n>* getActive() {return &m_scenes[m_activeIndex];}
        void setActive(int index) {if(index < m_sceneCount) m_activeIndex = index; }
        int getCount() const {return m_sceneCount;}
        Scene<n>* getScene(int index) {return (index < m_sceneCount) ? &m_scenes[index] : nullptr;}


        void createShape(const ShapeParams<n>& params);


        void loadScenes(const std::string courseName);
        void saveScenes(const std::string courseName);

        void togglePause() {m_scenes[m_activeIndex].togglePause();}


    private:
        void createSquare(const ShapeParams<n>& params);
        void createCircle(const ShapeParams<n>& params);
        void createTriangle(const ShapeParams<n>& params);
        void createBall(const ShapeParams<n>& params); // a circle with enabled physics
};

#endif
