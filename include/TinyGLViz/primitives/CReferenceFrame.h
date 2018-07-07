

#ifndef GLVIEWER_CREFERENCEFRAME_H
#define GLVIEWER_CREFERENCEFRAME_H

#include <math.h>
#include <memory>
#include <vector>

#include <TinyGLViz/CGLPrimitive.hpp>
#include <TinyGLViz/CMaterial.hpp>
#include <TinyGLViz/primitives/CSolidArrow.h>

namespace TinyGLViz {

    class CReferenceFrame: public CGLPrimitive {

    public:
        typedef std::shared_ptr<CReferenceFrame> Ptr;

        typedef std::shared_ptr<CReferenceFrame> ConstPtr;

    protected:
        CSolidArrow m_xaxis;
        CSolidArrow m_yaxis;
        CSolidArrow m_zaxis;
        CMaterial matX;
        CMaterial matY;
        CMaterial matZ;

        T_real m_length;

    public:
        CReferenceFrame(T_real length = 1.0, CTransform<T_real> t = CTransform<T_real>(), T_int resolution = 2) {
            setLength(length);
            setTransform(t);
            matX.setSpecular(1.0, 0.0, 0.0);
            matX.setDiffuse(1.0, 0.0, 0.0);
            matX.shininess = 0.0001;
            matY.setSpecular(0.0, 1.0, 0.0);
            matY.setDiffuse(0.0, 1.0, 0.0);
            matY.shininess = 0.0001;
            matZ.setSpecular(0.0, 0.0, 1.0);
            matZ.setDiffuse(0.0, 0.0, 1.0);
            matZ.shininess = 0.0001;
            setResolution(resolution);
            generateVertexData();
        }

        ~CReferenceFrame() = default;

        T_real getLenght() { return m_length; }

        void setLength(const T_real &len) {
            m_length = len;
            m_xaxis.setHeadHeight(m_length * 0.2);
            m_xaxis.setHeadRadius(m_length * 0.05);
            m_xaxis.setBodyRadius(m_length * 0.01);
            m_xaxis.setBodyHeight(m_length * 0.8);

            m_yaxis.setHeadHeight(m_length * 0.2);
            m_yaxis.setHeadRadius(m_length * 0.05);
            m_yaxis.setBodyRadius(m_length * 0.01);
            m_yaxis.setBodyHeight(m_length * 0.8);

            m_zaxis.setHeadHeight(m_length * 0.2);
            m_zaxis.setHeadRadius(m_length * 0.05);
            m_zaxis.setBodyRadius(m_length * 0.01);
            m_zaxis.setBodyHeight(m_length * 0.8);
        }

        void setResolution(const T_int &resolution) {
            m_xaxis.setResolution(resolution);
            m_yaxis.setResolution(resolution);
            m_zaxis.setResolution(resolution);
        }

        void setTransform(const CTransform<T_real> &transform) {
            CTransform<T_real> t_aux1 = transform;
            t_aux1.rotateY(M_PI * 0.5);
            m_xaxis.setTransform(t_aux1);

            CTransform<T_real> t_aux2 = transform;
            t_aux2.rotateX(-M_PI * 0.5);
            m_yaxis.setTransform(t_aux2);

            m_zaxis.setTransform(transform);
        }

        T_int getResolution() { return m_zaxis.getResolution(); }

        CTransform<T_real> &getTransform() { return m_zaxis.getTransform(); }

        void generateVertexData() {
            m_xaxis.generateVertexData();
            m_yaxis.generateVertexData();
            m_zaxis.generateVertexData();
        }

        void draw(Shader *s) {
            setTransform(m_zaxis.getTransform());
            matX.use(s);
            m_xaxis.draw(s);
            matY.use(s);
            m_yaxis.draw(s);
            matZ.use(s);
            m_zaxis.draw(s);
        }
    };
}
#endif //OPENGLLEARNING_CSOLIDSPHERE_H
