#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>


class InputManager
{

    public:
        enum MOUSE_BUTTON { left = 0, right, middle, back, forward };

        static const int MAX_KEY_CALLBACKS = 32;
        static const int MAX_MOUSE_CALLBACKS = 32;

        using KeyCallback = void(*)(int key);
        using MouseCallback = void(*)(int button);
        using ScrollCallback = void(*)(double x, double y);

    private:
        InputManager() = default;
        ~InputManager() = default;

        // Keyboard
        bool m_current_keys [GLFW_KEY_LAST + 1]  = {};
        bool m_previous_keys[GLFW_KEY_LAST + 1]  = {};

        // Mouse buttons
        bool m_current_mouse [GLFW_MOUSE_BUTTON_LAST + 1] = {};
        bool m_previous_mouse[GLFW_MOUSE_BUTTON_LAST + 1] = {};

        // Mouse position
        double m_mouse_x      = 0.0, m_mouse_y      = 0.0;
        double m_last_mouse_x = 0.0, m_last_mouse_y = 0.0;
        double m_scroll_x     = 0.0, m_scroll_y     = 0.0;

        KeyCallback m_key_pressed_callbacks [MAX_KEY_CALLBACKS] = {};
        KeyCallback m_key_released_callbacks[MAX_KEY_CALLBACKS] = {};
        MouseCallback m_mouse_pressed_callbacks [MAX_MOUSE_CALLBACKS] = {};
        MouseCallback m_mouse_released_callbacks[MAX_MOUSE_CALLBACKS] = {};
        ScrollCallback m_scroll_callbacks[MAX_MOUSE_CALLBACKS] = {};

        int m_key_pressed_count = 0;
        int m_key_released_count = 0;
        int m_mouse_pressed_count = 0;
        int m_mouse_released_count = 0;
        int m_scroll_count = 0;


    private:
        static bool valid_key  (int key) { return key >= 0 && key <= GLFW_KEY_LAST; }
        static bool valid_mouse(int btn) { return btn >= 0 && btn <= GLFW_MOUSE_BUTTON_LAST; }
        static void glfw_key_callback(GLFWwindow*, int key, int, int action, int)
        {
            auto& im = get_instance();
            if (!valid_key(key)) return;

            if (action == GLFW_PRESS)
            {
                im.m_current_keys[key] = true;
                for (int i = 0; i < im.m_key_pressed_count; i++)
                    im.m_key_pressed_callbacks[i](key);
            }
            else if (action == GLFW_RELEASE)
            {
                im.m_current_keys[key] = false;
                for (int i = 0; i < im.m_key_released_count; i++)
                    im.m_key_released_callbacks[i](key);
            }
        }

        static void glfw_mouse_button_callback(GLFWwindow*, int button, int action, int)
        {
            auto& im = get_instance();
            if (!valid_mouse(button)) return;

            if (action == GLFW_PRESS)
            {
                im.m_current_mouse[button] = true;
                for (int i = 0; i < im.m_mouse_pressed_count; i++)
                    im.m_mouse_pressed_callbacks[i](button);
            }
            else if (action == GLFW_RELEASE)
            {
                im.m_current_mouse[button] = false;
                for (int i = 0; i < im.m_mouse_released_count; i++)
                    im.m_mouse_released_callbacks[i](button);
            }
        }

        static void glfw_cursor_callback(GLFWwindow*, double x, double y)
        {
            auto& im = get_instance();
            im.m_mouse_x = x;
            im.m_mouse_y = y;
        }

        static void glfw_scroll_callback(GLFWwindow*, double x, double y)
        {
            auto& im = get_instance();
            im.m_scroll_x += x;
            im.m_scroll_y += y;
            for (int i = 0; i < im.m_scroll_count; i++)
                im.m_scroll_callbacks[i](x, y);
        }





    public:
        static InputManager& get_instance()
        {
            static InputManager instance;
            return instance;
        }

        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(InputManager&&) = delete;

        void register_window(GLFWwindow* window)
        {
            glfwSetKeyCallback(window,glfw_key_callback);
            glfwSetMouseButtonCallback(window,glfw_mouse_button_callback);
            glfwSetCursorPosCallback(window,glfw_cursor_callback);
            glfwSetScrollCallback(window,glfw_scroll_callback);
        }

        void update()
        {
            for(int i = 0; i <= GLFW_KEY_LAST; i ++)
            {
                m_previous_keys[i] = m_current_keys[i];

            }

            for(int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i ++)
            {
                m_previous_mouse[i] = m_current_mouse[i];

            }
            m_last_mouse_x = m_mouse_x;
            m_last_mouse_y = m_mouse_y;
            m_scroll_x = 0.0;
            m_scroll_y = 0.0;
        }

        bool is_key_down(int key) const { return valid_key(key) &&  m_current_keys[key]; }
        bool is_key_pressed(int key) const { return valid_key(key) &&  m_current_keys[key] && !m_previous_keys[key]; }
        bool is_key_released(int key) const { return valid_key(key) && !m_current_keys[key] &&  m_previous_keys[key]; }


        bool is_mouse_down(int btn) const { return valid_mouse(btn) &&  m_current_mouse[btn]; }
        bool is_mouse_pressed(int btn) const { return valid_mouse(btn) &&  m_current_mouse[btn] && !m_previous_mouse[btn]; }
        bool is_mouse_released(int btn) const { return valid_mouse(btn) && !m_current_mouse[btn] &&  m_previous_mouse[btn]; }


        void get_mouse_position(double& x, double& y) const { x = m_mouse_x; y = m_mouse_y; }
        void get_mouse_delta(double& x, double& y) const { x = m_mouse_x - m_last_mouse_x; y = m_mouse_y - m_last_mouse_y; }
        void get_scroll_delta(double& x, double& y) const { x = m_scroll_x; y = m_scroll_y; }


        void on_key_pressed(KeyCallback cb) { if (m_key_pressed_count  < MAX_KEY_CALLBACKS)   m_key_pressed_callbacks [m_key_pressed_count++]  = cb; }
        void on_key_released(KeyCallback cb) { if (m_key_released_count < MAX_KEY_CALLBACKS)   m_key_released_callbacks[m_key_released_count++] = cb; }
        void on_mouse_pressed(MouseCallback cb) { if (m_mouse_pressed_count  < MAX_MOUSE_CALLBACKS) m_mouse_pressed_callbacks [m_mouse_pressed_count++]  = cb; }
        void on_mouse_released(MouseCallback cb) { if (m_mouse_released_count < MAX_MOUSE_CALLBACKS) m_mouse_released_callbacks[m_mouse_released_count++] = cb; }
        void on_scroll(ScrollCallback cb)        { if (m_scroll_count < MAX_MOUSE_CALLBACKS)         m_scroll_callbacks[m_scroll_count++] = cb; }


};


#endif
