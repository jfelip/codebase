//
// Created by jfelip on 11/2/17.
//

#ifndef JFELIP_CODEBASE_CGLPRIMITIVE_HPP
#define JFELIP_CODEBASE_CGLPRIMITIVE_HPP

#include <TinyGLViz/CGLGeometry.h>
#include <TinyGLViz/Types.h>

namespace TinyGLViz
{

    class CGLPrimitive {
    public:
        typedef std::shared_ptr<CGLPrimitive> Ptr;

        typedef const std::shared_ptr<CGLPrimitive> ConstPtr;

        virtual void draw( Shader *s ) = 0;

        virtual void generateVertexData() = 0;

        virtual CTransform<T_real> & getTransform() = 0;

        virtual void setTransform(const CTransform<T_real> &transform) = 0;

        virtual void addVertex(const vec3::Vector3<T_vertex> &v){};

        virtual void addVertex(const T_vertex& x, const T_vertex&y, const T_vertex& z){};
        std::string m_materialId = "default";
        std::string m_shaderId = "default";

    protected:
        COpenGLGeometry m_geom;


    };

}
#endif //JFELIP_CODEBASE_CGLPRIMITIVE_HPP
