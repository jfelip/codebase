//
// Created by jfelip on 10/24/17.
//

#ifndef JFELIP_CODEBASE_CCAMERA_HPP
#define JFELIP_CODEBASE_CCAMERA_HPP

// CCamera Class for OpenGL adapted from https://learnopengl.com/#!Getting-started/Camera
//

// Std. Includes
#include <vector>
#include <iostream>
#include <math.h>

//Custom includes
#include <TinyGLViz/Types.h>
#include <TinyGLViz/CVector3.hpp>
#include <GenericMessages.hpp>

namespace TinyGLViz {
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    namespace CAMERA_VIEW {
        enum Camera_View {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT,
            FRONT,
            BACK
        };
    }

// Default camera values
    const T_real SPEED = 3.0f;
    const T_real SENSITIVTY = 0.1f;
    const T_real FoV = (T_real(45.0) * (T_real(M_PI) / T_real(180.0)) );

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
    class CCamera {

    public:
        // CCamera Attributes
        vec3::Vector3 <T_real> Position;
        vec3::Vector3 <T_real> defaultPosition;
        vec3::Vector3 <T_real> Front;
        vec3::Vector3 <T_real> Up;
        vec3::Vector3 <T_real> Right;
        // CCamera options
        T_real MovementSpeed;
        T_real m_FoV;
        T_real MouseSensitivity;
        vec3::Vector3<T_real> m_target;
        vec3::Vector3<T_real> m_defaulttarget;

        // Constructor with vectors
        CCamera(vec3::Vector3 <T_real> position = vec3::Vector3<T_real>(T_real(0.0), T_real(1.0), T_real(0.0)),
                vec3::Vector3 <T_real> up = vec3::Vector3<T_real>(T_real(0.0), T_real(1.0), T_real(0.0)),
                vec3::Vector3 <T_real> target = vec3::Vector3<T_real>(T_real(0.0), T_real(0.0), T_real(0.0))
                ) : Front(vec3::Vector3<T_real>(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
                                       MouseSensitivity(SENSITIVTY), m_FoV(FoV)
        {
            Position = position;
            defaultPosition = position;
            m_target = target;
            m_defaulttarget = target;
            Front = vec3::Vector3<T_real>(0.0f, 0.0f, 1.0f);
            Up = up;
            vec3::cross(Up,Front,Right);
            Right.normalize();
            lookAt(m_target);
        }

        void Reset()
        {
            m_target = m_defaulttarget;
            Position = defaultPosition;
            lookAt(m_target);
        }

        void setHome(const vec3::Vector3 <T_real>& position,const vec3::Vector3 <T_real>& target)
        {
            m_defaulttarget = target;
            defaultPosition = position;
        }

        // Constructor with scalar values
//        CCamera(T_real posX, T_real posY, T_real posZ, T_real upX, T_real upY, T_real upZ, T_real yaw, T_real pitch)
//                : Front(vec3::Vector3<T_real>(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY),
//                  Zoom(ZOOM) {
//            Position = vec3::Vector3<T_real>(posX, posY, posZ);
//            WorldUp = vec3::Vector3<T_real>(upX, upY, upZ);
//            Yaw = yaw;
//            Pitch = pitch;
//            //updateCameraVectors();
//            lookAt(m_target);
//        }

        // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
        void GetViewMatrix(T_vertex view[16]) const {
            view[0] = this->Right(0);
            view[1] = this->Right(1);
            view[2] = this->Right(2);
            view[3] = 0;

            view[4] = this->Up(0);
            view[5] = this->Up(1);
            view[6] = this->Up(2);
            view[7] = 0;

            view[8] = this->Front(0);
            view[9] = this->Front(1);
            view[10] = this->Front(2);
            view[11] = 0;

            view[12] = this->Right(0) * this->Position(0) + this->Right(1) * this->Position(1) +
                       this->Right(2) * this->Position(2);
            view[13] =
                    this->Up(0) * this->Position(0) + this->Up(1) * this->Position(1) + this->Up(2) * this->Position(2);
            view[14] = this->Front(0) * this->Position(0) + this->Front(1) * this->Position(1) +
                       this->Front(2) * this->Position(2);
            view[15] = 1;
        }

        // Processes input received from any keyboard-like input system.
        // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction, T_real deltaTime) {
            T_real dist_ini = (Position - m_target).norm();
            T_real velocity = this->MovementSpeed * deltaTime;
            if (direction == FORWARD)
                this->Position += this->Front * velocity;
            if (direction == BACKWARD)
                this->Position -= this->Front * velocity;
            if (direction == LEFT)
            {
                this->m_target += this->Right * velocity;
                this->Position += this->Right * velocity;
            }
            if (direction == RIGHT)
            {
                this->m_target -= this->Right * velocity;
                this->Position -= this->Right * velocity;
            }
            if (direction == UP)
            {
                this->m_target += this->Up * velocity;
                this->Position += this->Up * velocity;
            }
            if (direction == DOWN)
            {
                this->m_target -= this->Up * velocity;
                this->Position -= this->Up * velocity;
            }

            lookAt(m_target);
        }

        void ProcessMouseMovementMiddleClick(T_real xoffset, T_real yoffset) {
            xoffset *= this->MouseSensitivity;
            yoffset *= this->MouseSensitivity;

            m_target += Right * xoffset;
            Position += Right * xoffset;
            m_target += Up * -yoffset;
            Position += Up * -yoffset;
            lookAt(m_target);
        }

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        // TODO: Improve the arcball rotation around the focus point that is not working properly
        void ProcessMouseMovementLeftClick(T_real xoffset, T_real yoffset)
        {
            xoffset *= this->MouseSensitivity;
            yoffset *= this->MouseSensitivity;

            T_real dist_ini = (Position - m_target).norm();
            Position += Right * xoffset;
            Position += Up * yoffset;

            //Keep the camera at the same distance from the target
            T_real dist_diff = (Position - m_target).norm() - dist_ini;
            Position += Front * dist_diff;
            T_real final_dist = (Position - m_target).norm();

//            std::cout << "TargetDist: " << final_dist << std::endl;
//            std::cout << "CameraPos: " << Position << std::endl;
//            std::cout << "CameraTgt: " << m_target << std::endl;

            lookAt(m_target);
        }

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(T_real yoffset)
        {
            this->Position += this->Front * yoffset * this->MouseSensitivity * 10.0f;
            lookAt(m_target);
        }

        void lookAt(const vec3::Vector3 <T_real>& target)
        {
            if (target != Position)
            {
                m_target = target;
                //Make the front vector point at the target point
                Front = (target - Position);
                Front.normalize();

                //Obtain the up and check that it is not colinear with the Front
                vec3::Vector3 <T_real> RightTmp;
                vec3::cross(Up, Front, RightTmp);
                if ( RightTmp.norm() <= 0.0001 )   //TODO: Change this by a proper epsilon
                {
                    vec3::cross(Front, Right, Up);
                    Up.normalize();
                    vec3::cross(Up, Front, Right);
                    Right.normalize();
                }
                else
                {
                    Right = RightTmp;
                    Right.normalize();
                    vec3::cross(Front, Right, Up);
                    Up.normalize();
                }
            }
        }

        void setView(const T_real& dist, const CAMERA_VIEW::Camera_View cameraView)
        {
            switch (cameraView)
            {
                case CAMERA_VIEW::TOP:
                    Position = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),-dist);
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view TOP");
                    break;
                case CAMERA_VIEW::BOTTOM:
                    Position = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),dist);
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view BOTTOM");
                    break;
                case CAMERA_VIEW::LEFT:
                    Position = TinyGLViz::vec3::Vector3<T_real>(T_real(0),dist,T_real(0));
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view LEFT");
                    break;
                case CAMERA_VIEW::RIGHT:
                    Position = TinyGLViz::vec3::Vector3<T_real>(T_real(0),-dist,T_real(0));
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view RIGHT");
                    break;
                case CAMERA_VIEW::FRONT:
                    Position = TinyGLViz::vec3::Vector3<T_real>(dist,T_real(0),T_real(0));
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view FRONT");
                    break;
                case CAMERA_VIEW::BACK:
                    Position = TinyGLViz::vec3::Vector3<T_real>(-dist,T_real(0),T_real(0));
                    m_target = TinyGLViz::vec3::Vector3<T_real>(T_real(0),T_real(0),T_real(0));
                    _GENERIC_MESSAGE_("Set view BACK");
                    break;
            }
            lookAt(m_target);
        }

    };

}

#endif //JFELIP_CODEBASE_CCAMERA_HPP
