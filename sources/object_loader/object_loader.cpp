#include <glad/glad.h>

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>

#include "object_loader.h"
#include <primitives.h>

template<>
ObjectLoader<unsigned char>::ObjectLoader(std::string_view filePath) : type(GL_UNSIGNED_BYTE) {
	init(filePath);
}

template<>
ObjectLoader<unsigned short>::ObjectLoader(std::string_view filePath) : type(GL_UNSIGNED_SHORT) {
	init(filePath);
}

template<>
ObjectLoader<unsigned int>::ObjectLoader(std::string_view filePath) : type(GL_UNSIGNED_INT) {
	init(filePath);
}

template<typename T>
void ObjectLoader<T>::init(std::string_view filePath) {
	std::fstream file(filePath.data());
	std::string line;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	std::unordered_map<std::string, unsigned int> st;
	T maxIndex = 0;

	while (getline(file, line)) {
		std::stringstream ss;
		std::string tmp;
		ss << line;
		ss >> tmp;
		if (tmp == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			vertices.emplace_back(glm::vec3{ x, y, z });
		}
		else if (tmp == "vt") {
			float u, v;
			ss >> u >> v;
			texCoords.emplace_back(glm::vec2{ u, v });
		}
		else if (tmp == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			normals.emplace_back(glm::vec3{ x, y, z });
		}
		else if (tmp == "f") {
			int i = 3;
			std::string v, vt, n;
			char trial;
			ss.get(trial); // clear 1 space
			while (i --> 0) {
				getline(ss, v, '/');
				getline(ss, vt, '/');
				getline(ss, n, ' ');
				T index;
				auto tmp = v + '/' + vt + '/' + n;
				if (auto it = st.find(tmp); it == st.cend()) {
					int vi = std::stoi(v) - 1;
					int vti = std::stoi(vt) - 1;
					int ni = std::stoi(n) - 1;
					this->vertices.emplace_back(Vertex3D{ vertices[vi], texCoords[vti], normals[ni] });
					st.insert({ tmp, maxIndex });
					this->indices.emplace_back(maxIndex++);
				}
				else
					this->indices.emplace_back(it->second);
			}
		}
		
	}
}
