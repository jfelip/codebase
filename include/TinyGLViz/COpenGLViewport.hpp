//
// Created by jfelip on 10/23/17.
//

#ifndef PROJECT_COPENGLVIEWPORT_HPP
#define PROJECT_COPENGLVIEWPORT_HPP

#include <TinyGLViz/CCamera.hpp>
#include <TinyGLViz/Types.h>

//TODO: Focus camera on object under the cursor
//TODO: View all objects

namespace TinyGLViz
{

class COpenGLViewport
{


public:
    typedef enum
    {
        PERSPECTIVE,
        ORTHO
    }projectionType;

    COpenGLViewport( const std::string& name )
    {
        COpenGLViewport::name = name;
        for (uint i=0; i<3; ++i)
            mouse[i] = false;

        for (uint i=0; i<1024; ++i)
            keys[i] = false;
    }

    void getGLViewMatrix(GLfloat view[16]) const { m_camera.GetViewMatrix(view); }

    T_real getLocationHorizontal()  const { return m_locationHorizontal;}

    T_real getLocationVertical()    const { return m_locationVertical;  }

    T_real getSizeHorizontal()      const { return m_sizeHorizontal;    }

    T_real getSizeVertical()        const { return m_sizeVertical;      }

    void setLocationHorizontal(const T_real& locationHorizontal) { m_locationHorizontal = locationHorizontal; }

    void setLocationVertical  (const T_real& locationVertical)   { m_locationVertical   = locationVertical;   }

    void setSizeHorizontal    (const T_real& sizeHorizontal)     { m_sizeHorizontal     = sizeHorizontal;     }

    void setSizeVertical      (const T_real& sizeVertical)       { m_sizeVertical = sizeVertical;             }

    void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key >= 0 && key < 1024)
        {
            if(action == GLFW_PRESS)
                keys[key] = true;
            else if(action == GLFW_RELEASE)
                keys[key] = false;
        }

        double deltaTime = 0.033; //Assume 30FPS render //TODO: Remove this magic number

        // Camera controls
        if(keys[GLFW_KEY_W])
            m_camera.ProcessKeyboard(FORWARD, deltaTime);
        if(keys[GLFW_KEY_S])
            m_camera.ProcessKeyboard(BACKWARD, deltaTime);
        if(keys[GLFW_KEY_A])
            m_camera.ProcessKeyboard(LEFT, deltaTime);
        if(keys[GLFW_KEY_D])
            m_camera.ProcessKeyboard(RIGHT, deltaTime);
        if(keys[GLFW_KEY_Q])
            m_camera.ProcessKeyboard(UP, deltaTime);
        if(keys[GLFW_KEY_E])
            m_camera.ProcessKeyboard(DOWN, deltaTime);
        if(keys[GLFW_KEY_H])
            m_camera.Reset();
        if(keys[GLFW_KEY_KP_7])
            m_camera.setView(100,CAMERA_VIEW::TOP);
        if(keys[GLFW_KEY_KP_1])
            m_camera.setView(100,CAMERA_VIEW::BOTTOM);
        if(keys[GLFW_KEY_KP_4])
            m_camera.setView(100,CAMERA_VIEW::LEFT);
        if(keys[GLFW_KEY_KP_6])
            m_camera.setView(100,CAMERA_VIEW::RIGHT);
        if(keys[GLFW_KEY_KP_8])
            m_camera.setView(100,CAMERA_VIEW::FRONT);
        if(keys[GLFW_KEY_KP_2])
            m_camera.setView(100,CAMERA_VIEW::BACK);
    }

    void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
    {
        m_camera.ProcessMouseScroll(yoffset);
    }

    void mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
    {
        if (mouse[0])
        {
            xoffset = xpos - lastX;
            yoffset = ypos - lastY;  // Reversed since y-coordinates go from bottom to left
            m_camera.ProcessMouseMovementLeftClick(xoffset, yoffset);
        }
        else if (mouse[1])
        {
            xoffset = xpos - lastX;
            yoffset = ypos - lastY;  // Reversed since y-coordinates go from bottom to left
            m_camera.ProcessMouseMovementMiddleClick(xoffset, yoffset);
        }

        lastX = xpos;
        lastY = ypos;
    }

    void mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            mouse[0] = true;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            mouse[0] = false;
        }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        {
            mouse[1] = true;
        }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        {
            mouse[1] = false;
        }
    }

    void char_callback_impl(GLFWwindow *window, int param){}
    void drop_callback_impl(GLFWwindow *window, int count, const char **paths){}
    void charmods_callback_impl(GLFWwindow *window, unsigned int codepoint, int mods){}

    //Draw viewport specific stuff (e.g. border, name, etc...)
    void draw() const
    {


    }

    CCamera m_camera;

protected:
    T_real m_locationHorizontal = 0.0;  ///< Position of the viewport left border in the OpenGL window. Range [0 , 1].
    T_real m_locationVertical   = 0.0;  ///< Position of the viewport bottom border in the OpenGL window. Range [0 , 1].
    T_real m_sizeHorizontal = 1.0;      ///< Horizontal viewport size relative to its parent window. Range [0 , 1].
    T_real m_sizeVertical   = 1.0;      ///< Vertical viewport size relative to its parent window. Range [0 , 1].

    projectionType m_projectionType = PERSPECTIVE;
    T_bool keys[1024];
    T_bool mouse[3];
    T_real lastX = 0;
    T_real lastY = 0;
    T_real xoffset = 0;
    T_real yoffset = 0;
    std::string name;
    std::string title;
};

}

#endif //PROJECT_COPENGLVIEWPORT_HPP
