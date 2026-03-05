#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

struct DebugOptions
{
    bool wireframe = false;
    bool showCollisionNormals = false;
    bool showAimLine = false;
    float timeScale = 1.0f;
    bool showGrid = false;

    static DebugOptions& get()
    {
        static DebugOptions instance;
        return instance;
    }

    //reset to defaults
    void reset()
    {
        wireframe = false;
        showCollisionNormals = false;
        showAimLine = false;
        timeScale = 1.0f;
    }

private:
    DebugOptions() = default;
    DebugOptions(const DebugOptions&) = delete;
    DebugOptions& operator=(const DebugOptions&) = delete;
};

#endif
