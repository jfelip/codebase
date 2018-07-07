#ifndef __CMATERIAL_H_
#define __CMATERIAL_H_

#include <GL/glew.h>
#include<TinyGLViz/Types.h>
#include<TinyGLViz/CGLShader.hpp>

namespace TinyGLViz {

	class CMaterial {
	public:
		typedef std::shared_ptr<CMaterial> Ptr;

		typedef const std::shared_ptr<CMaterial> ConstPtr;


		T_real diffuse[3] = {T_real(0), T_real(0), T_real(0)};
		T_real specular[3] = {T_real(0), T_real(0), T_real(0)};
		T_real shininess = T_real(0);

		void setDiffuse(const T_real r, const T_real g, const T_real b) {
			diffuse[0] = r;
			diffuse[1] = g;
			diffuse[2] = b;
		}

		void setSpecular(const T_real r, const T_real g, const T_real b) {
			specular[0] = r;
			specular[1] = g;
			specular[2] = b;
		}

		void use(Shader *s) {
			s->Use();
			GLint matDiffuseLoc = glGetUniformLocation(s->Program, "material.diffuse");
			GLint matSpecularLoc = glGetUniformLocation(s->Program, "material.specular");
			GLint matShineLoc = glGetUniformLocation(s->Program, "material.shininess");
			glUniform3f(matDiffuseLoc, diffuse[0], diffuse[1], diffuse[2]);
			glUniform3f(matSpecularLoc, specular[0], specular[1], specular[2]);
			glUniform1f(matShineLoc, shininess);
		}
	};
}
#endif
