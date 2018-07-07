

#ifndef GLVIEWER_CSOLIDSPHERE_H
#define GLVIEWER_CSOLIDSPHERE_H

#include <TinyGLViz/CGLPrimitive.hpp>
#include <math.h>
#include <memory>
#include <vector>

namespace TinyGLViz {

    class CSolidSphere : public CGLPrimitive {
    public:

        typedef std::shared_ptr<CSolidSphere> Ptr;

        typedef std::shared_ptr<CSolidSphere> ConstPtr;


    protected:
        T_real m_radius;
        T_int  m_alpha_resolution;
        T_int  m_beta_resolution;

    public:
        CSolidSphere(T_real radius = 1.0, const CTransform<T_real> &t = CTransform<T_real>(),
                     T_int alpha_resolution = 20, T_int beta_resolution = 20) {
            m_radius = radius;
            m_alpha_resolution = alpha_resolution;
            m_beta_resolution = beta_resolution;
            setTransform(t);
            generateVertexData();
        }

        void setRadius(T_real r) { m_radius = r; }

        void setAlphaResolution(T_int alpha_resolution) { m_alpha_resolution = alpha_resolution; }

        void setBetaResolution(T_int beta_resolution) { m_beta_resolution = beta_resolution; }

        void setTransform(const CTransform<T_real> &transform) { m_geom.setTransform(transform); }

        T_real getRadius() { return m_radius; }

        T_int getAlphaResolution() { return m_alpha_resolution; }

        T_int getBetaResolution() { return m_beta_resolution; }

        CTransform<T_real> &getTransform() { return m_geom.getTransform(); }

        std::vector<T_real> &getVertices() { return m_geom.getVertices(); }

        std::vector<T_real> &getNormals() { return m_geom.getNormals(); }

        void generateVertexData() {
            std::vector<T_real> vertices;
            std::vector<T_indices> indices;
            std::vector<T_real> normals;
            std::vector<T_real> colors;
            std::vector<T_real> textureCoords;

            T_real x, y, z, alpha, beta; // Storage for coordinates and angles
            T_real radius = m_radius;
            T_indices idx = 0;

            T_real alpha_incr = M_PI / m_alpha_resolution;
            T_real beta_incr = M_PI / m_beta_resolution;
            T_int  M_PI2 = 2 * M_PI + alpha_incr;
            T_int  M_PI1 = M_PI + beta_incr;

            //TODO: Correct sphere vertices generation
            for (alpha = 0.0; alpha < M_PI1; alpha += alpha_incr) {
                for (beta = 0.0; beta < M_PI2; beta += beta_incr) {
                    x = radius * cos(beta) * sin(alpha);
                    y = radius * sin(beta) * sin(alpha);
                    z = radius * cos(alpha);
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                    indices.push_back(idx);
                    ++idx;

                    x = radius * cos(beta) * sin(alpha + M_PI / m_alpha_resolution);
                    y = radius * sin(beta) * sin(alpha + M_PI / m_alpha_resolution);
                    z = radius * cos(alpha + M_PI / m_alpha_resolution);
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                    indices.push_back(idx);
                    ++idx;
                }
            }

            normals = vertices;

            m_geom.setDrawType(GL_TRIANGLE_STRIP);
            m_geom.setVertices(vertices);
            m_geom.setIndices(indices);
            m_geom.setNormals(normals);
            m_geom.updateBuffers();
        }

        void draw(Shader *s) {
            m_geom.draw(s);
        }
    };

}
#endif //OPENGLLEARNING_CSOLIDSPHERE_H
