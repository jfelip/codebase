#ifndef __CGLLIGHT_H_
#define __CGLLIGHT_H_

#include <GL/glew.h>

#include <TinyGLViz/Types.h>
#include <TinyGLViz/Transform.h>
#include <TinyGLViz/CGLShader.hpp>

namespace TinyGLViz {

	class CAttenuation {
	public:
		CAttenuation() : constant(1.0), linear(0.22), quadratic(0.2) {}

		CAttenuation(T_real c, T_real l, T_real q) : constant(c), linear(l), quadratic(q) {}

		//Attenuation parameter values for the light attenuation (spot and point lights)
        T_real constant = T_real(0);
        T_real linear = T_real(0);
        T_real quadratic = T_real(0);

		/*
        * Recommended attenuation values depending on the distance we want the light to have effect
        * Distance	Constant	Linear	Quadratic
        7			1.0			0.7		1.8
        13			1.0			0.35	0.44
        20			1.0			0.22	0.20
        32			1.0			0.14	0.07
        50			1.0			0.09	0.032
        65			1.0			0.07	0.017
        100			1.0			0.045	0.0075
        160			1.0			0.027	0.0028
        200			1.0			0.022	0.0019
        325			1.0			0.014	0.0007
        600			1.0			0.007	0.0002
        3250		1.0			0.0014	0.000007
        *
        * Table extracted from https://learnopengl.com/#!Lighting/Light-casters
        */

	};


	class CDirectionalLight {
	public:
		T_real direction[3] = {T_real(0), T_real(0), T_real(0)}; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)
		T_real ambient[3] = {T_real(0), T_real(0), T_real(0)};
		T_real diffuse[3] = {T_real(0), T_real(0), T_real(0)};
		T_real specular[3] = {T_real(0), T_real(0), T_real(0)};

		void setDirection(const T_real x, const T_real y, const T_real z) {
			direction[0] = x;
			direction[1] = y;
			direction[2] = z;
		}

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

		void setAmbient(const T_real r, const T_real g, const T_real b) {
			ambient[0] = r;
			ambient[1] = g;
			ambient[2] = b;
		}


		void use(Shader *s) {
			s->Use();

			//TODO: ADD LIGHT TYPE and pass it to the shader
			// Pass the light information to the shader
			GLint lightPosLoc = glGetUniformLocation(s->Program, "dirLight.direction");
			GLint lightAmbientLoc = glGetUniformLocation(s->Program, "dirLight.ambient");
			GLint lightDiffuseLoc = glGetUniformLocation(s->Program, "dirLight.diffuse");
			GLint lightSpecularLoc = glGetUniformLocation(s->Program, "dirLight.specular");
			GLint lightEnableLoc = glGetUniformLocation(s->Program, "dirLightEnabled");

			glUniform1i(lightEnableLoc, true);
			glUniform3f(lightPosLoc, direction[0], direction[1], direction[2]);
			glUniform3f(lightAmbientLoc, ambient[0], ambient[1], ambient[2]);
			glUniform3f(lightDiffuseLoc, diffuse[0], diffuse[1], diffuse[2]);
			glUniform3f(lightSpecularLoc, specular[0], specular[1], specular[2]);
		}

	};

//TODO: Implement point lights
	class CPointLight {
	public:

		T_real position[3];
		T_real direction[3]; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)

		T_real color_diffuse;
		T_real color_specular;

		CAttenuation attenuation;

		uint id;

		void use(Shader *s) {

		}

	};


//TODO: Implement spot lights
	class CSpotLight {
	public:

		T_real position[3];
		T_real direction[3]; ///> Position and orientation of the light (for directional light only orientation is used as the direction vector)

		T_real color_diffuse;
		T_real color_specular;

		CAttenuation attenuation;

		T_real inner_cutoff;
		T_real outer_cutoff;

		uint id;

		void use(Shader *s) {

		}

	};

}
#endif
