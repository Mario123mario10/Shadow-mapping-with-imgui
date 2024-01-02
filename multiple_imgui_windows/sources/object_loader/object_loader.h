#pragma once
#include <glm/glm.hpp>

#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
#include <iostream>

#include "primitives.h"

template<typename T>
class ObjectLoader {
	Mesh<Vertex3D, T> mesh;
public:
	ObjectLoader(std::string_view filePath);
	const Mesh<Vertex3D, T>& getMesh() const { return mesh; };
};

template<typename T>
ObjectLoader<T>::ObjectLoader(std::string_view filePath) {
	std::fstream file(filePath.data());
	std::string line;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	std::unordered_map<std::string, T> st;
	T maxIndex = 0;

	while (getline(file, line)) {
		std::stringstream ss;
		std::string tmp;
		ss << line;
		ss >> tmp;
		if (tmp == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			vertices.emplace_back(x, y, z);
		}
		else if (tmp == "vt") {
			float u, v;
			ss >> u >> v;
			texCoords.emplace_back(u, v);
		}
		else if (tmp == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			normals.emplace_back(x, y, z);
		}
		else if (tmp == "f") {
			int i = 3;
			std::string v, vt, n;
			char trial;
			ss.get(trial); // clear 1 space
			while (i-- > 0) {
				getline(ss, v, '/');
				getline(ss, vt, '/');
				getline(ss, n, ' ');
				T index;
				auto tmp = v + '/' + vt + '/' + n;
				if (auto it = st.find(tmp); it == st.cend()) {
					int vi = std::stoi(v) - 1;
					int vti = std::stoi(vt) - 1;
					int ni = std::stoi(n) - 1;
					mesh.vertices.emplace_back(vertices[vi], texCoords[vti], normals[ni]);
					st.insert({ tmp, maxIndex });
					mesh.indices.emplace_back(maxIndex++);
				}
				else
					mesh.indices.emplace_back(it->second);
			}
		}

	}
}
