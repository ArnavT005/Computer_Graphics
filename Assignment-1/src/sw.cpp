#include "sw.hpp"

#include <iostream>
#include <vector>

namespace COL781 {
	namespace Software {

		namespace {
			// software rasterizer object
			SoftwareRasterizer softwareRasterizer;
		}

		// Forward declarations

		template <> float Attribs::get(int index) const;
		template <> glm::vec2 Attribs::get(int index) const;
		template <> glm::vec3 Attribs::get(int index) const;
		template <> glm::vec4 Attribs::get(int index) const;

		template <> void Attribs::set(int index, float value);
		template <> void Attribs::set(int index, glm::vec2 value);
		template <> void Attribs::set(int index, glm::vec3 value);
		template <> void Attribs::set(int index, glm::vec4 value);

		template void Rasterizer::setUniform<float>(ShaderProgram &program, const std::string &name, float value);
		template void Rasterizer::setUniform<int>(ShaderProgram &program, const std::string &name, int value);
		template void Rasterizer::setUniform<glm::vec2>(ShaderProgram &program, const std::string &name, glm::vec2 value);
		template void Rasterizer::setUniform<glm::vec3>(ShaderProgram &program, const std::string &name, glm::vec3 value);
		template void Rasterizer::setUniform<glm::vec4>(ShaderProgram &program, const std::string &name, glm::vec4 value);
		template void Rasterizer::setUniform<glm::mat2>(ShaderProgram &program, const std::string &name, glm::mat2 value);
		template void Rasterizer::setUniform<glm::mat3>(ShaderProgram &program, const std::string &name, glm::mat3 value);
		template void Rasterizer::setUniform<glm::mat4>(ShaderProgram &program, const std::string &name, glm::mat4 value);

		template void Rasterizer::setVertexAttribs<float>(Object &object, int attribIndex, int n, const float* data);
		template void Rasterizer::setVertexAttribs<glm::vec2>(Object &object, int attribIndex, int n, const glm::vec2* data);
		template void Rasterizer::setVertexAttribs<glm::vec3>(Object &object, int attribIndex, int n, const glm::vec3* data);
		template void Rasterizer::setVertexAttribs<glm::vec4>(Object &object, int attribIndex, int n, const glm::vec4* data);

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
			window = nullptr;
			quit = false;
			bool widthFlag = softwareRasterizer.setFrameWidth(width);
			bool heightFlag = softwareRasterizer.setFrameHeight(height);
			bool sdlFlag = softwareRasterizer.initializeSDL(title);
			return widthFlag && heightFlag && sdlFlag;
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
			softwareRasterizer.setShader(program);
		}

		template <typename T> void Rasterizer::setUniform(ShaderProgram &program, const std::string &name, T value) {
			program.uniforms.set<T>(name, value);
		}

		void Rasterizer::deleteShaderProgram(ShaderProgram &program) {
			softwareRasterizer.deleteShader();
		}

		Object Rasterizer::createObject() {
			Object object;
			return object;
		}

		template <typename T> void Rasterizer::setVertexAttribs(Object &object, int attribIndex, int n, const T* data) {
			if (object.attributes.size() < n) {
				object.attributes.resize(n);
			}
			for (int i = 0; i < n; i ++) {
				object.attributes[i].set<T>(attribIndex, data[i]);
			}
		}

		void Rasterizer::setTriangleIndices(Object &object, int n, glm::ivec3* indices) {
			if (object.indices.size() < n) {
				object.indices.resize(n);
			}
			for (int i = 0; i < n; i ++) {
				object.indices[i] = indices[i];
			}
		}

		void Rasterizer::clear(glm::vec4 color) {
			softwareRasterizer.clearFramebuffer(color);
		}

		void Rasterizer::drawObject(const Object &object) {
			ShaderProgram shader = *softwareRasterizer.getShader();
			int n = object.indices.size() + 2;
			glm::vec4 vertices[n], color[n];
			Attribs attributes[n];
			for (int i = 0; i < n; i ++) {
				vertices[i] = shader.vs(shader.uniforms, object.attributes[i], attributes[i]);
				color[i] = shader.fs(shader.uniforms, attributes[i]);
			}
			softwareRasterizer.rasterizeObject2D(vertices, object.indices.data(), color, n - 2);
		}

		void Rasterizer::show() {
			SDL_Event e;
			while (SDL_PollEvent(&e) != 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
			}
			softwareRasterizer.drawFramebuffer();
		}

	}
}
