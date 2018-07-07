

#ifndef GLVIEWER_CSOLIDARROW_H
#define GLVIEWER_CSOLIDARROW_H

#include <math.h>
#include <memory>
#include <vector>

#include <TinyGLViz/CGLPrimitive.hpp>
#include <TinyGLViz/primitives/CSolidCone.h>
#include <TinyGLViz/primitives/CSolidCylinder.h>

namespace TinyGLViz {

    class CSolidArrow: public CGLPrimitive {
    public:

        typedef std::shared_ptr<CSolidArrow> Ptr;

        typedef std::shared_ptr<CSolidArrow> ConstPtr;

    protected:
        CSolidCone m_head;
        CSolidCylinder m_body;
        CTransform<T_real> m_transform;

    public:
        CSolidArrow(T_real head_radius = 0.2, T_real head_height = 0.2, T_real body_radius = 0.1,
                    T_real body_height = 0.8, CTransform<T_real> t = CTransform<T_real>(), T_int resolution = 20) {
            m_body.setRadius(body_radius);
            m_body.setHeight(body_height);
            m_body.setResolution(resolution);

            m_head.setRadius(head_radius);
            m_head.setHeight(head_height);
            m_head.setResolution(resolution);

            setTransform(t);
            generateVertexData();
        }

        void setBodyRadius(const T_real &r) { m_body.setRadius(r); }

        void setHeadRadius(const T_real &r) { m_head.setRadius(r); }

        void setBodyHeight(const T_real &h) { m_body.setHeight(h); }

        void setHeadHeight(const T_real &h) { m_head.setHeight(h); }

        void setResolution(const T_int &resolution) {
            m_body.setResolution(resolution);
            m_head.setResolution(resolution);
        }

        void setTransform(const CTransform<T_real> &transform) {
            m_transform = transform;

            CTransform<T_real> t1;
            t1.translateZ(m_body.getHeight() * 0.5);

            CTransform<T_real> t2;
            t2.translateZ(m_body.getHeight());

            m_body.setTransform(transform * t1);
            m_head.setTransform(transform * t2);
        }

        T_real getHeadRadius() { return m_head.getRadius(); }

        T_real getBodyRadius() { return m_body.getRadius(); }

        T_real getHeadHeight() { return m_head.getHeight(); }

        T_real getBodyHeight() { return m_body.getHeight(); }

        T_int getResolution() { return m_body.getResolution(); }

        CTransform<T_real> &getTransform() { return m_transform; }

        void generateVertexData() {
            m_head.generateVertexData();
            m_body.generateVertexData();
        }

        void draw(Shader *s) {
            setTransform(m_transform);
            m_head.draw(s);
            m_body.draw(s);
        }
    };
}

#endif //OPENGLLEARNING_CSOLIDSPHERE_H
