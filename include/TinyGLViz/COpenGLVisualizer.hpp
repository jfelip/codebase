//
// Created by jfelip on 10/23/17.
//

#ifndef PROJECT_COPENGLVISUALIZER_HPP
#define PROJECT_COPENGLVISUALIZER_HPP

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include <map>
#include <chrono>

#include <TinyGLViz/COpenGLViewport.hpp>
#include <TinyGLViz/CGLShader.hpp>
#include <TinyGLViz/CGLPrimitive.hpp>
#include <TinyGLViz/CGLLight.hpp>
#include <TinyGLViz/CMaterial.hpp>
#include <TinyGLViz/Transform.h>
#include <TinyGLViz/CDualQuaternion.hpp>
#include <TinyGLViz/CQuaternion.hpp>
#include <TinyGLViz/CVector3.hpp>
#include <TinyGLViz/primitives/CSolidLine.h>
#include <TinyGLViz/primitives/CReferenceFrame.h>
#include <TinyGLViz/CTextDrawer.hpp>

//TODO: Image primtiive: A primitive that displays an image on a quad
//TODO: Enable/Disable wireframe mode
//TODO: Enable/Disable reference frame
//TODO: Enable/Disable grid
//TODO: 2D Text
//TODO: Fix camera orbital point on the clicked item
//TODO: Show camera orbital point
//TODO: Fix camera orbit controls with a more intuitive solution

namespace TinyGLViz {

    template<typename T>
    void projectionMatrix(T angle, T ratio, T near, T far, T mat[16])
    {
        for (uint i=0; i<16; ++i)
            mat[i] = 0;

        T   tan_half_angle = tan(angle / 2);
        mat[0] = 1.0 / (ratio * tan_half_angle);
        mat[5] = 1.0 / tan_half_angle;
        mat[10] = -(far + near) / (far - near);
        mat[11] = -1;
        mat[14] = -(2 * far * near) / (far - near);
    }

    //Static callbacks that will call each specific callback inside the class that has the window pointer
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void char_callback(GLFWwindow* window, unsigned int codepoint);
    static void drop_callback(GLFWwindow* window, int count, const char** paths);
    static void error_callback(int error, const char * description);
    static void monitor_callback(GLFWmonitor* monitor, int event){}
    static void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
    static void cursor_enter_callback(GLFWwindow* window, int entered);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void window_close_callback(GLFWwindow* window);
    static void window_focus_callback(GLFWwindow* window, int focused); //focused = 1 got focus  || focused = 0 lost focus
    static void window_iconify_callback(GLFWwindow* window, int iconified); //Minimized
    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
    static void window_refresh_callback(GLFWwindow* window);
    static void window_size_callback(GLFWwindow* window, int width, int height);


    class COpenGLVisualizer
    {
        typedef enum
        {
            GENERIC_ERROR,
            NO_ERROR
        }enumERROR;


    public:
        COpenGLVisualizer(){}
        ~COpenGLVisualizer()
        {
            delete stdShader;
            delete pTextDrawer;
            glfwTerminate();
        }

        enumERROR initialize(const std::string& windowName, uint width, uint height,
                             const std::string& fontsPath,
                             const std::string& shaderPath
        )
        {

            // Init GLFW
            glfwInit();
            // Set all the required options for GLFW
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

            // Create a GLFWwindow object that we can use for GLFW's functions
            m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(m_window);

            // Set the callback function forwarding
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetKeyCallback(m_window, key_callback);
            glfwSetCursorPosCallback(m_window, mouse_callback);
            glfwSetScrollCallback(m_window, scroll_callback);
            glfwSetMouseButtonCallback(m_window, mouse_button_callback);
            glfwSetCharCallback(m_window, char_callback);
            glfwSetDropCallback(m_window, drop_callback);
            glfwSetErrorCallback(error_callback);
            glfwSetMonitorCallback(monitor_callback);
            glfwSetCharModsCallback(m_window, charmods_callback);
            glfwSetCursorEnterCallback(m_window, cursor_enter_callback);
            glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
            glfwSetWindowCloseCallback(m_window, window_close_callback);
            glfwSetWindowFocusCallback(m_window, window_focus_callback);
            glfwSetWindowIconifyCallback(m_window, window_iconify_callback);
            glfwSetWindowPosCallback(m_window, window_pos_callback);
            glfwSetWindowRefreshCallback(m_window, window_refresh_callback);
            glfwSetWindowSizeCallback(m_window, window_size_callback);

            // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
            glewExperimental = GL_TRUE;
            // Initialize GLEW to setup the OpenGL Function pointers
            glewInit();

            glfwSetWindowUserPointer(m_window, this);

            //Load shaders
            // Build and compile our shader program
            stdShader = new Shader
            (
                    shaderPath + "indicesTextureColorNormal.vs",
                    "",
                    shaderPath + "indicesTextureColorNormal.frag"
            );

            m_shaders["default"] = stdShader;

            //Set light parameters
            dirLight.setDirection(0,-1,-1);
            dirLight.setSpecular(0.5,0.5,0.5);
            dirLight.setDiffuse(1,1,1);

            //Create a default materials
            CMaterial::Ptr defaultMat = CMaterial::Ptr(new CMaterial);
            defaultMat->setDiffuse(0,0.44,0.77);
            defaultMat->setSpecular(0.5,0.5,0.5);
            defaultMat->shininess = 0.000001;
            m_material["default"] = defaultMat;

            defaultMat = CMaterial::Ptr(new CMaterial);
            defaultMat->setDiffuse(1,0,0);
            defaultMat->setSpecular(0.5,0.5,0.5);
            defaultMat->shininess = 0.000001;
            m_material["red"] = defaultMat;

            defaultMat = CMaterial::Ptr(new CMaterial);
            defaultMat->setDiffuse(0,1,0);
            defaultMat->setSpecular(0.5,0.5,0.5);
            defaultMat->shininess = 0.00001;
            m_material["green"] = defaultMat;

            defaultMat = CMaterial::Ptr(new CMaterial);
            defaultMat->setDiffuse(0,0,1);
            defaultMat->setSpecular(0.5,0.5,0.5);
            defaultMat->shininess = 0.00001;
            m_material["blue"] = defaultMat;


            //Opengl configuration
            glEnable(GL_DEPTH_TEST);
            glEnable( GL_PROGRAM_POINT_SIZE );

            //Prepare text drawer
            pTextDrawer = new CTextDrawer(fontsPath+"arial.ttf", shaderPath);

            //Initialize FPS counter
            lastFrameTime = std::chrono::high_resolution_clock::now();

            return NO_ERROR;
        }

        int getHeight()
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            return height;
        }

        int getWidth()
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            return width;
        }

        int shouldClose()
        {
            return glfwWindowShouldClose(m_window);
        }

        void draw()
        {
            // Clear the colorbuffer and the depth buffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Draw all viewports
            for (const auto& v:m_viewports )
            {
                //Get window size
                int width,height;
                glfwGetFramebufferSize(m_window,&width,&height);

                //Set viewport size and location
                glViewport(static_cast<T_int>(v.getLocationHorizontal()*width),
                           static_cast<T_int>(v.getLocationVertical()*height),
                           static_cast<T_int>(v.getSizeHorizontal()*width),
                           static_cast<T_int>(v.getSizeVertical()*height) );

                //Get viewport view matrix
                GLfloat view[16];
                v.getGLViewMatrix(view);

                //Compute projection matrix
                GLfloat projection[16];
                projectionMatrix<GLfloat>(v.m_camera.m_FoV, (v.getSizeHorizontal()*width)/(v.getSizeVertical()*height), 0.001f, 10000.0f,projection);

                //Draw all objects
                for (const auto& p:m_primitives)
                {
                    //Load the object specific shader
                    Shader * s = nullptr;
                    if ( m_shaders.find( p.second->m_shaderId) != m_shaders.end() )
                    {
                        s = m_shaders[p.second->m_shaderId];
                    }
                    else
                    {
                        s = stdShader;
                    }

                    s->Use();

                    // Pass camera position, view and projection matrices to the shader
                    GLint viewLoc = glGetUniformLocation(s->Program, "view");
                    GLint projLoc = glGetUniformLocation(s->Program, "projection");
                    GLint viewPosLoc = glGetUniformLocation(s->Program, "viewPos");
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
                    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
                    glUniform3f(viewPosLoc, v.m_camera.Position.x(), v.m_camera.Position.y(), v.m_camera.Position.z());

                    // Pass other context information
                    GLint scrWidthLoc = glGetUniformLocation(s->Program, "screenWidth");
                    GLint lightPosLoc = glGetUniformLocation(s->Program, "lightPos");
                    glUniform1i(scrWidthLoc, width);
                    //glUniform3f(lightPosLoc, camera.Position.x(),camera.Position.y(),camera.Position.z());
                    glUniform3f(lightPosLoc, 0,0,2);

                    //Activate lights
                    dirLight.use(s);

                    //Set material properties to the shader
                    m_material[p.second->m_materialId]->use(s);

                    p.second->draw(s);
                }

                //TODO: Draw viewport specific content (title, borders, status message)
                v.draw();

                //Show FPS counter
                auto time = std::chrono::high_resolution_clock::now();
                T_real FPS = T_real(1) / (std::chrono::duration_cast<std::chrono::nanoseconds>(lastFrameTime - time).count() * 1e-9);
                lastFrameTime = std::chrono::high_resolution_clock::now();

                pTextDrawer->RenderText("FPS: " + std::to_string(FPS),0.0,0.0,0.5,vec3::Vector3<GLfloat>(0,0,0),v.getSizeVertical()*height,v.getSizeHorizontal()*width);
            }
        }

        enumERROR processEvents( )
        {
            //_SCOPED_TIMER_("DrawLoop took");
            draw( );
            glfwPollEvents( );
            glfwSwapBuffers(m_window);
            return NO_ERROR;
        }

        void addReferenceFrame(T_real size)
        {
            TinyGLViz::CReferenceFrame::Ptr pFrame = TinyGLViz::CReferenceFrame::Ptr( new TinyGLViz::CReferenceFrame() );
            pFrame->setLength(size);
            pFrame->generateVertexData();
            m_primitives["reference_frame"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pFrame);
        }

        void addGrid(T_int ncells, T_real cellsize)
        {
            TinyGLViz::CSolidLine::Ptr pLine = TinyGLViz::CSolidLine::Ptr( new TinyGLViz::CSolidLine() );
            pLine->setColor(TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(0.2,0.2,0.2));
            for(int i=-ncells; i<ncells; ++i)
            {
                for(int j=-ncells; j<ncells; ++j)
                {
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize           , j*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize           , j*cellsize+1*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize           , j*cellsize+1*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize+1*cellsize, j*cellsize+1*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize+1*cellsize, j*cellsize+1*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize+1*cellsize, j*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize+1*cellsize, j*cellsize,0) );
                    pLine->m_points.push_back( TinyGLViz::vec3::Vector3<TinyGLViz::T_vertex>(i*cellsize           , j*cellsize,0) );
                }
            }
            pLine->generateVertexData();
            m_primitives["grid"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pLine);
        }

        void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].key_callback_impl( window, key, scancode, action, mode);
        }

        void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
        {
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].scroll_callback_impl( window, xoffset, yoffset );
        }

        void mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
        {
            //TODO:: Set the active viewport depending on the cursor position
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].mouse_callback_impl( window, xpos, ypos );
        }

        void mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods)
        {
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].mouse_button_callback_impl(window, button, action, mods);
        }

        void char_callback_impl(GLFWwindow *window, int param)
        {
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].char_callback_impl(window, param);
        }

        void drop_callback_impl(GLFWwindow *window, int count, const char **paths)
        {
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].drop_callback_impl(window, count, paths);
        }

        void charmods_callback_impl(GLFWwindow *window, unsigned int codepoint, int mods)
        {
            if (m_viewports.size() > 0)
            m_viewports[m_activeViewport].charmods_callback_impl(window, codepoint, mods);
        }

        void cursor_enter_callback_impl(GLFWwindow *window, int entered){ }

        void framebuffer_size_callback_impl(GLFWwindow *window, int width, int height){ }

        void window_close_callback_impl(GLFWwindow *window) { }

        void window_focus_callback_impl(GLFWwindow *window, int focused) { }

        void window_iconify_callback_impl(GLFWwindow *window, int iconified) { }

        void window_pos_callback_impl(GLFWwindow *window, int xpos, int ypos) { }

        void window_refresh_callback_impl(GLFWwindow *window) { }

        void window_size_callback_impl(GLFWwindow *window, int width, int height) { }

        void addViewport(COpenGLViewport viewport) {
            m_viewports.push_back(viewport);
        }

        std::map< std::string, CGLPrimitive::Ptr > m_primitives;
        std::map< std::string, CMaterial::Ptr > m_material;
        std::map< std::string, Shader * > m_shaders;
        std::vector< COpenGLViewport > m_viewports;

    protected:
        std::chrono::high_resolution_clock::time_point lastFrameTime;
        CTextDrawer * pTextDrawer;
        CDirectionalLight dirLight;
        uint m_activeViewport = 0;
        GLFWwindow* m_window;
        bool mouse[3];
        Shader* stdShader;
    };


    //Static callbacks that will call each specific callback inside the class that has the window pointer
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->key_callback_impl(window,key,scancode,action,mode);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->scroll_callback_impl(window, xoffset, yoffset);
    }

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->mouse_callback_impl(window,xpos,ypos);
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->mouse_button_callback_impl(window,button,action,mods);
    }

    static void char_callback(GLFWwindow* window, unsigned int codepoint)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->char_callback_impl(window,codepoint);
    }

    static void drop_callback(GLFWwindow* window, int count, const char** paths)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
            pointer->drop_callback_impl(window, count, paths);
    }

    static void error_callback(int error, const char * description)
    {
        _GENERIC_ERROR_("GLFW: " + std::to_string(error));
    }

    static void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->charmods_callback_impl(window,codepoint,mods);
    }

    static void cursor_enter_callback(GLFWwindow* window, int entered)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->cursor_enter_callback_impl(window,entered);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->framebuffer_size_callback_impl(window,width,height);
    }

    static void window_close_callback(GLFWwindow* window)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_close_callback_impl(window);
    }

    static void window_focus_callback(GLFWwindow* window, int focused) //focused = 1 got focus  || focused = 0 lost focus
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_focus_callback_impl(window,focused);
    }

    static void window_iconify_callback(GLFWwindow* window, int iconified) //Minimized
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_iconify_callback_impl(window,iconified);
    }

    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_pos_callback_impl(window,xpos,ypos);
    }

    static void window_refresh_callback(GLFWwindow* window)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_refresh_callback_impl(window);
    }

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {
        auto pointer = static_cast<COpenGLVisualizer*>(glfwGetWindowUserPointer(window));
        if (pointer)
        pointer->window_size_callback_impl(window,width,height);
    }

}

#endif //PROJECT_COPENGLVISUALIZER_HPP
