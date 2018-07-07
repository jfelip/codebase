#ifndef GLVIEWER_PARTICLE_SYSTEM_H
#define GLVIEWER_PARTICLE_SYSTEM_H

#include <vector>
#include <memory>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <TinyGLViz/Transform.h>
#include <TinyGLViz/CGLShader.hpp>

//TODO: Enable particle-wise sphere radius

namespace TinyGLViz
{

template<typename PointT, typename allocatorT = std::allocator<PointT> >
class CParticleSystem : public CGLPrimitive
{
public:
	typedef std::shared_ptr<CParticleSystem> Ptr;

	typedef std::shared_ptr<CParticleSystem> ConstPtr;

    typedef enum
    {
        POINTS,
        SPHERES
    } DRAW_PRIMITIVE;

public:
    CParticleSystem();

    ~CParticleSystem();

    bool updateBuffers();

    void draw(Shader *shader);

    T_real getPontSize(){ return m_pointSize;}

	void setPointSize(const T_real& s){	m_pointSize = s; }

    std::vector<PointT, allocatorT>& getParticleSystem() {return m_pPSystem;}

    void setParticleSystem( std::vector<PointT, allocatorT>& pS ) {m_pPSystem = pS;}

    std::vector<T_vertex>& getColors() {return m_colors;}

    CTransform<T_real>& getTransform() {return m_transform;}

    void setTransform(const CTransform<T_real>& transform) { m_transform = transform;}

    void setColors( const std::vector<T_vertex>& colors ) {m_colors = colors;}

    void setColor( const T_real& r,const T_real& g, const T_real& b )
    {
        m_colors.clear();
        for (uint p=0; p<m_pPSystem.size() ; ++p)
        {
            m_colors.push_back( r );
            m_colors.push_back( g );
            m_colors.push_back( b );
        }
    }

    void rotate (const T_real r[4])		{m_transform.rotate(r);}

    void translate (const T_real t[3])	{m_transform.translate(t);}

    void setDrawPrimitive( DRAW_PRIMITIVE p ){ m_drawPrimitive = p; }

    DRAW_PRIMITIVE getDrawPrimitive( ){ return m_drawPrimitive; }

    void generateVertexData(){ updateBuffers(); }

protected:
    std::vector<PointT, allocatorT> m_pPSystem;

    std::vector<T_vertex> m_colors;

    std::vector<T_vertex> m_vertexBufferDataPoints;

    T_real m_pointSize;

    CTransform<T_real> m_transform;

    GLuint VBO, VAO;

    GLuint VBOConstraints, VAOConstraints;

    DRAW_PRIMITIVE m_drawPrimitive = DRAW_PRIMITIVE::POINTS;

};


template<class PointT, class allocatorT>
CParticleSystem<PointT,allocatorT>::CParticleSystem()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOConstraints);
    glGenBuffers(1, &VBOConstraints);
	m_pointSize = 1.0;
}

template<class PointT, class allocatorT>
CParticleSystem<PointT,allocatorT>::~CParticleSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAOConstraints);
    glDeleteBuffers(1, &VBOConstraints);
}

template<class PointT, class allocatorT>
bool CParticleSystem<PointT,allocatorT>::updateBuffers()
{
    if( m_colors.size()*3 != m_pPSystem.size())
    {
        m_colors.resize(m_pPSystem.size()*3,0);

        uint i=0;
        for (uint p=0; p<m_pPSystem.size() ; ++p)
        {
            m_colors[i] = 0;
            m_colors[i+1] = 0;
            m_colors[i+2] = 0.3;
            i+=3;
        }
    }

    m_vertexBufferDataPoints.clear();
	uint i=0;
    for (uint p=0; p<m_pPSystem.size() ; ++p)
    {
        m_vertexBufferDataPoints.push_back(m_pPSystem[p].m_x);
        m_vertexBufferDataPoints.push_back(m_pPSystem[p].m_y);
        m_vertexBufferDataPoints.push_back(m_pPSystem[p].m_z);

        m_vertexBufferDataPoints.push_back(m_colors[i]);
        m_vertexBufferDataPoints.push_back(m_colors[i+1]);
        m_vertexBufferDataPoints.push_back(m_colors[i+2]);

		i += 3;
	}

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T_vertex) * m_vertexBufferDataPoints.size(), m_vertexBufferDataPoints.data(), GL_STATIC_DRAW);

		//TODO: Change the GL_FLOAT type according to the T_vertex template
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(T_vertex), (GLvoid*) (3 * sizeof(T_vertex)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	return true;
}



template<class PointT, class allocatorT>
void CParticleSystem<PointT,allocatorT>::draw(Shader * shader)
{
    //Update transformation matrix
    m_transform.m_data.computeMatrix();

    updateBuffers();

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    glEnable(GL_POINT_SPRITE_ARB);

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

    glDepthMask(GL_TRUE);

    shader->Use();

    GLuint SphereRadiusLoc = glGetUniformLocation(shader->Program, "SphereRadius");
    GLuint transformLoc = glGetUniformLocation(shader->Program, "model");
    glUniformMatrix4fv(transformLoc, 1,GL_FALSE, m_transform.m_data.m_pMatrix);
    glUniform1f(SphereRadiusLoc, m_pointSize);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, m_vertexBufferDataPoints.size() / 6);
    glBindVertexArray(0);

    glDisable(GL_POINT_SPRITE_ARB);
}

}

#endif
