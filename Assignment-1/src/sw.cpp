#include "sw.hpp"

#include <iostream>
#include <vector>

namespace COL781 {
	namespace Software {

		// Forward declarations

		template <> float Attribs::get(int index) const;
		template <> glm::vec2 Attribs::get(int index) const;
		template <> glm::vec3 Attribs::get(int index) const;
		template <> glm::vec4 Attribs::get(int index) const;

		template <> void Attribs::set(int index, float value);
		template <> void Attribs::set(int index, glm::vec2 value);
		template <> void Attribs::set(int index, glm::vec3 value);
		template <> void Attribs::set(int index, glm::vec4 value);

		// Built-in shaders

		VertexShader Rasterizer::vsIdentity() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				return vertex;
			};
		}

		VertexShader Rasterizer::vsTransform() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::mat4 transform = uniforms.get<glm::mat4>("transform");
				return transform * vertex;
			};
		}

		VertexShader Rasterizer::vsColor() {
			return [](const Uniforms &uniforms, const Attribs &in, Attribs &out) {
				glm::vec4 vertex = in.get<glm::vec4>(0);
				glm::vec4 color = in.get<glm::vec4>(1);
				out.set<glm::vec4>(0, color);
				return vertex;
			};
		}

		FragmentShader Rasterizer::fsConstant() {
			return [](const Uniforms &uniforms, const Attribs &in) {
				glm::vec4 color = uniforms.get<glm::vec4>("color");
				return color;
			};
		}

		FragmentShader Rasterizer::fsIdentity() {
			return [](const Uniforms &uniforms, const Attribs &in) {
				glm::vec4 color = in.get<glm::vec4>(0);
				return color;
			};
		}

		// Implementation of Attribs and Uniforms classes

		void checkDimension(int index, int actual, int requested) {
			if (actual != requested) {
				std::cout << "Warning: attribute " << index << " has dimension " << actual << " but accessed as dimension " << requested << std::endl;
			}
		}

		template <> float Attribs::get(int index) const {
			checkDimension(index, dims[index], 1);
			return values[index].x;
		}

		template <> glm::vec2 Attribs::get(int index) const {
			checkDimension(index, dims[index], 2);
			return glm::vec2(values[index].x, values[index].y);
		}

		template <> glm::vec3 Attribs::get(int index) const {
			checkDimension(index, dims[index], 3);
			return glm::vec3(values[index].x, values[index].y, values[index].z);
		}

		template <> glm::vec4 Attribs::get(int index) const {
			checkDimension(index, dims[index], 4);
			return values[index];
		}

		void expand(std::vector<int> &dims, std::vector<glm::vec4> &values, int index) {
			if (dims.size() < index+1)
				dims.resize(index+1);
			if (values.size() < index+1)
				values.resize(index+1);
		}

		template <> void Attribs::set(int index, float value) {
			expand(dims, values, index);
			dims[index] = 1;
			values[index].x = value;
		}

		template <> void Attribs::set(int index, glm::vec2 value) {
			expand(dims, values, index);
			dims[index] = 2;
			values[index].x = value.x;
			values[index].y = value.y;
		}

		template <> void Attribs::set(int index, glm::vec3 value) {
			expand(dims, values, index);
			dims[index] = 3;
			values[index].x = value.x;
			values[index].y = value.y;
			values[index].z = value.z;
		}

		template <> void Attribs::set(int index, glm::vec4 value) {
			expand(dims, values, index);
			dims[index] = 4;
			values[index] = value;
		}

		template <typename T> T Uniforms::get(const std::string &name) const {
			return *(T*)values.at(name);
		}

		template <typename T> void Uniforms::set(const std::string &name, T value) {
			auto it = values.find(name);
			if (it != values.end()) {
				delete it->second;
			}
			values[name] = (void*)(new T(value));
		}

		bool Rasterizer::initialize(const std::string &title, int width, int height) {
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
				return false;
			}
			window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
			if (!window) {
				std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
				return false;
			}
			quit = false;
			return true;
		}

		bool Rasterizer::shouldQuit() {
			return quit;
		}

		ShaderProgram Rasterizer::createShaderProgram(const VertexShader &vs, const FragmentShader &fs) {
			ShaderProgram program;
			program.vs = vs;
			program.fs = fs;
			return program;
		}

		void Rasterizer::useShaderProgram(const ShaderProgram &program) {
			//glUseProgram(program);
			//glCheckError();
		}

		template <typename T> void Rasterizer::setUniform(const ShaderProgram &program, const std::string &name, T value) {
			program.uniforms.set(name, value);
		}

		void Rasterizer::deleteShaderProgram(ShaderProgram &program) {
			//glDeleteProgram(program);
			//glCheckError();
		}

		Object Rasterizer::createObject() {
			Object object;
			glGenVertexArrays(1, &object.vao);
			glCheckError();
			return object;
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const float* data) {
			setAttribs(object, attribIndex, n, 1, data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec2* data) {
			setAttribs(object, attribIndex, n, 2, (float*)data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec3* data) {
			setAttribs(object, attribIndex, n, 3, (float*)data);
		}

		template <> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const glm::vec4* data) {
			setAttribs(object, attribIndex, n, 4, (float*)data);
		}

		void Rasterizer::setTriangleIndices(Object &object, int n, glm::ivec3* indices) {
			GLuint ebo;
			glGenBuffers(1, &ebo);
			glBindVertexArray(object.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*n*sizeof(int), (float*)indices, GL_STATIC_DRAW);
			object.nTris = n;
			glCheckError();
		}

		void Rasterizer::clear(glm::vec4 color) {
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCheckError();
		}

		void Rasterizer::drawObject(const Object &object) {
			glBindVertexArray(object.vao);
			glDrawElements(GL_TRIANGLES, 3*object.nTris, GL_UNSIGNED_INT, 0);
			glCheckError();
		}

		void Rasterizer::show() {
			SDL_GL_SwapWindow(window);
			SDL_Event e;
			while (SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
			}
			glCheckError();
		}

	}
}
