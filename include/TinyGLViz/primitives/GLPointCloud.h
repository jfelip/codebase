#ifndef GLVIEWER_POINT_CLOUD_H
#define GLVIEWER_POINT_CLOUD_H

#include <vector>
#include <memory>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <TinyGLViz/Transform.h>
#include <TinyGLViz/CGLShader.hpp>
#include <TinyGLViz/CGLPrimitive.hpp>

#define GL_POINT_CLOUD_MAX_POINTS    524288 //2^19

namespace TinyGLViz
{

class COpenGLPointCloud: public CGLPrimitive
{
    struct drawpoint
    {
        float position[3];
        unsigned char color[4];
    };

public:
	typedef std::shared_ptr<COpenGLPointCloud> Ptr;

	typedef std::shared_ptr<COpenGLPointCloud> ConstPtr;

public:
	COpenGLPointCloud();

    ~COpenGLPointCloud();

    bool updateBuffers();

    void draw(Shader *shader);

    GLuint getPontSize(){ return m_pointSize;}

	void setPointSize(const GLuint& s){	m_pointSize = s; }

	CTransform<T_real>& getTransform() {return m_transform;}

    template <typename PointT>
	void setPoints( std::vector< PointT >& vertices )
	{
		m_points.clear();
		for (auto & p:vertices)
		{
            drawpoint point;
            point.position[0] = p.x();
            point.position[1] = p.y();
            point.position[2] = p.z();
            point.color[0] = 128;
            point.color[1] = 128;
            point.color[2] = 128;
            point.color[3] = 128;
            m_points.push_back(point);
		}
	}

    void generateVertexData(){}

    void setTransform( const CTransform<T_real>& transform ) { m_transform = transform;}

    void rotate (const T_real r[4])		{m_transform.rotate(r);}

    void translate (const T_real t[3])	{m_transform.translate(t);}


protected:
    std::vector<drawpoint> m_points;

    GLuint m_pointSize;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO;

};


COpenGLPointCloud::COpenGLPointCloud()
{
    glGenBuffers (1, &VBO);
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    glBufferData (GL_ARRAY_BUFFER, GL_POINT_CLOUD_MAX_POINTS * sizeof (drawpoint), NULL, GL_STREAM_DRAW);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
}

COpenGLPointCloud::~COpenGLPointCloud()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool COpenGLPointCloud::updateBuffers()
{



}

//VBO based render: https://www.opengl.org/discussion_boards/showthread.php/178061-Buffer-vs-immediate
void COpenGLPointCloud::draw(Shader *shader)
{
    drawpoint *bufpoints = NULL;
    int pointcount = 0;

    glBindBuffer (GL_ARRAY_BUFFER, VBO);

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_COLOR_ARRAY);

    // because bufpoints points to NULL here, this will work (yes, I did test it)
    glVertexPointer (3, GL_FLOAT, sizeof (drawpoint), bufpoints->position);
    glColorPointer (4, GL_UNSIGNED_BYTE, sizeof (drawpoint), bufpoints->color);

    if ((bufpoints = (drawpoint *) glMapBufferRange (GL_ARRAY_BUFFER, 0, GL_POINT_CLOUD_MAX_POINTS * sizeof (drawpoint), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) == NULL)
    {
        // error handling here - suggest falling back to immediate mode?  don't forget to call glBindBuffer (GL_ARRAY_BUFFER, 0) and glDisableClientState!
        glDisableClientState (GL_VERTEX_ARRAY);
        glDisableClientState (GL_COLOR_ARRAY);
        glBindBuffer (GL_ARRAY_BUFFER, 0);
        return;
    }

    // you may prefer to use a proper iterator here
    for (int i = 0; i < m_points.size (); i++)
    {
        // adds current point to buffer
        memcpy (&bufpoints[pointcount], &m_points[i], sizeof (drawpoint));

        // go to next slot in buffer
        pointcount++;

        // check for potential overflow
        if (pointcount == GL_POINT_CLOUD_MAX_POINTS)
        {
            // draw what we got so far
            glDrawArrays (GL_POINTS, 0, pointcount);

            // re-init buffer and counter
            if ((bufpoints = (drawpoint *) glMapBufferRange (GL_ARRAY_BUFFER, 0, GL_POINT_CLOUD_MAX_POINTS * sizeof (drawpoint), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) == NULL)
            {
                // error handling here - if this fails we've got bigger problems on our hands than not being able to get a usable pointer!
            }

            pointcount = 0;
        }
    }

    // draw anything left over
    if (pointcount) glDrawArrays (GL_POINTS, 0, pointcount);

    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_COLOR_ARRAY);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
}

}

#endif
