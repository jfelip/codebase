

#ifndef GLVIEWER_CSOLIDLINE_H
#define GLVIEWER_CSOLIDLINE_H

#include <TinyGLViz/CGLPrimitive.hpp>
#include <TinyGLViz/CVector3.hpp>
#include <math.h>
#include <memory>
#include <vector>
#include <TinyGLViz/CMaterial.hpp>

//TODO: Enable line per-vertex color

namespace TinyGLViz {

    class CSolidLine : public CGLPrimitive
    {
    public:
        typedef enum
        {
            LINES = GL_LINES,
            LINE_STRIP = GL_LINE_STRIP
        } DRAW_STYLE;


    protected:
        CMaterial mat;
        vec3::Vector3<T_vertex> m_color;
        DRAW_STYLE m_line_type = DRAW_STYLE::LINES;
    public:
        typedef std::shared_ptr<CSolidLine> Ptr;

        typedef std::shared_ptr<CSolidLine> ConstPtr;

        std::vector< vec3::Vector3<T_vertex> > m_points;

        T_vertex m_lineWidth = 1.0;

        CSolidLine(const T_vertex& lwidth=1.0, const vec3::Vector3<T_vertex >& color=vec3::Vector3<T_vertex>(1,0,0))
        {
            m_lineWidth = lwidth;
            setColor(color);
        }
        ~CSolidLine() = default;

        void setDrawStyle(DRAW_STYLE ds) { m_line_type = ds; }

        virtual void addVertex(const vec3::Vector3<T_vertex> &v)
        {
            m_points.push_back(v);
        };

        virtual void addVertex(const T_vertex& x, const T_vertex& y, const T_vertex& z)
        {
            m_points.push_back(vec3::Vector3<T_vertex>(x,y,z));
        };

        void setColor(const vec3::Vector3<T_vertex >& color)
        {
            m_color = color;
            mat.setDiffuse(color.x(),color.y(),color.z());
            mat.setSpecular(color.x(),color.y(),color.z());
            mat.shininess = 0.00001;
        }

        void setTransform(const CTransform<T_real> &transform) { m_geom.setTransform(transform); }

        CTransform<T_real> &getTransform() { return m_geom.getTransform(); }

        void generateVertexData()
        {
            std::vector<T_vertex> vertices;
            std::vector<T_indices> indices;
            std::vector<T_vertex> normals;
            std::vector<T_real> colors;
            std::vector<T_real> textureCoords;
            T_indices i =0;

            //If the number of points is odd do not use the last one
            bool odd = !(m_points.size()%2 == 0);
            for (const auto& p:m_points)
            {
                if (i != m_points.size()-1 || !odd)
                {
                    vertices.push_back(p.x());
                    vertices.push_back(p.y());
                    vertices.push_back(p.z());

                    normals.push_back(p.x());
                    normals.push_back(p.y());
                    normals.push_back(p.z());

                    colors.push_back(m_color.x());
                    colors.push_back(m_color.y());
                    colors.push_back(m_color.z());

                    indices.push_back(i);
                    ++i;
                }
            }

            m_geom.setDrawType(m_line_type);
            m_geom.setVertices(vertices);
            m_geom.setIndices(indices);
            m_geom.setNormals(normals);
            m_geom.updateBuffers();
        }

        void draw(Shader *s)
        {
            glLineWidth(m_lineWidth);
            mat.use(s);
            m_geom.draw(s);
        }
    };

}
#endif //OPENGLLEARNING_CSOLIDSPHERE_H
