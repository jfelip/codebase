
#include <unistd.h> //Include for the usleep function


#include <GenericMessages.hpp>
#include <CScopedTimer.hpp>
#include <StringHelpers.hpp>


#include <TinyGLViz/COpenGLVisualizer.hpp>
#include <TinyGLViz/primitives/CSolidBox.h>
#include <TinyGLViz/primitives/CSolidCapsule.h>
#include <TinyGLViz/primitives/CSolidCone.h>
#include <TinyGLViz/primitives/CSolidCylinder.h>
#include <TinyGLViz/primitives/CSolidSphere.h>
#include <TinyGLViz/primitives/CSolidArrow.h>
#include <TinyGLViz/primitives/CReferenceFrame.h>
#include <TinyGLViz/primitives/CSolidLine.h>
#include <TinyGLViz/primitives/CParticleSystem.h>

class Point
{
public:
    double& x() { return m_x; }
    double& y() { return m_y; }
    double& z() { return m_z; }

    double m_x;
    double m_y;
    double m_z;
};

int main() {

    ///////////////////////////////////////////////////////////////////////////////////
    // Generic messages and scoped timer
    ///////////////////////////////////////////////////////////////////////////////////


    _SCOPED_TIMER_("Execution took: ");

    _GENERIC_MESSAGE_ ("This is a generic message");
    _GENERIC_ERROR_   ("This is a generic ERROR message");
    _GENERIC_DEBUG_   ("This is a generic DEBUG message");
    _GENERIC_WARNING_ ("This is a generic WARNING message");

    try
    {
        throw std::exception();
    }
    catch (const std::exception& e)
    {
        _GENERIC_EXCEPTION_(e);
    }


    ///////////////////////////////////////////////////////////////////////////////////
    // String helpers
    ///////////////////////////////////////////////////////////////////////////////////

    std::string str = "  Hello, World!  ";
    std::string ltrimstr = StringHelpers::ltrimCopy(str);
    std::string rtrimstr = StringHelpers::rtrimCopy(str);
    std::string trimstr  = StringHelpers::trimCopy(str);

    std::cout << "Original string: " << str << "EOF" << std::endl;
    std::cout << "Trim from start: " << ltrimstr << "EOF" << std::endl;
    std::cout << "Trim from end  : " << rtrimstr << "EOF" << std::endl;
    std::cout << "Trim from both : " << trimstr << "EOF" << std::endl;

    ///////////////////////////////////////////////////////////////////////////////////
    // TinyGL Visualizer
    ///////////////////////////////////////////////////////////////////////////////////

    TinyGLViz::COpenGLVisualizer viz;

    std::string fontsPath = "/home/jfelip/workspace/codebase/include/TinyGLViz/fonts/";
    std::string shadersPath = "/home/jfelip/workspace/codebase/include/TinyGLViz/shaders/";

    viz.initialize("TinyViz Test",1080,720,fontsPath,shadersPath);

    //Create a viewport
    TinyGLViz::COpenGLViewport viewport("view1");
    viewport.setLocationHorizontal(0);
    viewport.setLocationVertical(0);
    viewport.setSizeHorizontal(1.0);
    viewport.setSizeVertical(1.0);

    //Set camera position
    viewport.m_camera.setHome(
            TinyGLViz::vec3::Vector3<TinyGLViz::T_real>(TinyGLViz::T_real(0), TinyGLViz::T_real(10), TinyGLViz::T_real(0) ),
            TinyGLViz::vec3::Vector3<TinyGLViz::T_real>(TinyGLViz::T_real(0), TinyGLViz::T_real(0), TinyGLViz::T_real(0) )
    );
    viewport.m_camera.MouseSensitivity = 0.1f;
    viewport.m_camera.Reset();
    viz.addViewport(viewport);


    //Add a shape to the visualizer
    TinyGLViz::CSolidBox::Ptr pCube = TinyGLViz::CSolidBox::Ptr( new TinyGLViz::CSolidBox() );
    pCube->getTransform().translateX(10);
    viz.m_primitives["box1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pCube);

    TinyGLViz::CSolidCapsule::Ptr pCap = TinyGLViz::CSolidCapsule::Ptr( new TinyGLViz::CSolidCapsule() );
    pCap->getTransform().translateX(12);
    viz.m_primitives["cap1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pCap);

    TinyGLViz::CSolidCone::Ptr pCone = TinyGLViz::CSolidCone::Ptr( new TinyGLViz::CSolidCone() );
    pCone->getTransform().translateX(8);
    viz.m_primitives["cone1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pCone);

    TinyGLViz::CSolidCylinder::Ptr pCyl = TinyGLViz::CSolidCylinder::Ptr( new TinyGLViz::CSolidCylinder() );
    pCyl->getTransform().translateX(6);
    viz.m_primitives["cyl1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pCyl);

    TinyGLViz::CSolidSphere::Ptr pSphere = TinyGLViz::CSolidSphere::Ptr( new TinyGLViz::CSolidSphere() );
    pSphere->getTransform().translateX(4);
    viz.m_primitives["sph1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pSphere);

    TinyGLViz::CSolidArrow::Ptr pArrow = TinyGLViz::CSolidArrow::Ptr( new TinyGLViz::CSolidArrow() );
    pArrow->getTransform().translateX(2);
    pArrow->getTransform().translateY(2);
    viz.m_primitives["arr1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pArrow);

    TinyGLViz::CReferenceFrame::Ptr pFrame = TinyGLViz::CReferenceFrame::Ptr( new TinyGLViz::CReferenceFrame() );
    pFrame->setLength(1);
    pFrame->generateVertexData();
    pFrame->getTransform().translateY(3);
    viz.m_primitives["reference_frame1"] = static_cast< TinyGLViz::CGLPrimitive::Ptr >(pFrame);

    TinyGLViz::CParticleSystem<Point>::Ptr glPointCloud = TinyGLViz::CParticleSystem<Point>::Ptr( new TinyGLViz::CParticleSystem<Point>() );
    std::vector<Point> points;
    for (uint i=0; i<300; ++i)
    for (uint j=0; j<1000; ++j)
    {
        Point p;
        p.m_x = i/100.0;
        p.m_y = j/100.0;
        p.m_z = 0.0;
        points.push_back(p);
    }

    glPointCloud->setParticleSystem(points);
    glPointCloud->getTransform().translateZ(1.0);
    glPointCloud->updateBuffers();
    viz.m_primitives["pointCloud"] = static_cast<TinyGLViz::CGLPrimitive::Ptr>(glPointCloud);


    viz.addGrid(50,10.0);
    viz.addReferenceFrame(1);

    while(!viz.shouldClose())
    {
        viz.processEvents();
        usleep(10000);
    }

    return 0;
}