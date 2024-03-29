#include "mesh.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <set>

namespace COL781 {
    namespace Mesh {
        
        std::vector<int> Vertex::getAdjacentVertices() {
            std::vector<int> vertices;
            std::pair<int, int> hId = halfEdgeId;
            do {
                hId = mesh->mHalfEdges[hId.first][hId.second].pairId;
                vertices.push_back(mesh->mHalfEdges[hId.first][hId.second].headId);
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return vertices;
        }

        std::vector<int> Vertex::getAdjacentEdges() {
            std::vector<int> edges;
            std::pair<int, int> hId = halfEdgeId;
            do {
                edges.push_back(mesh->mHalfEdges[hId.first][hId.second].edgeId);
                hId = mesh->mHalfEdges[hId.first][hId.second].pairId;
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return edges;
        }

        std::vector<int> Vertex::getAdjacentFaces() {
            std::vector<int> faces;
            std::pair<int, int> hId = halfEdgeId;
            do {
                int faceId = mesh->mHalfEdges[hId.first][hId.second].faceId;
                if (!mesh->mFaces[faceId].isVirtual) {
                    faces.push_back(faceId);
                } 
                hId = mesh->mHalfEdges[hId.first][hId.second].pairId;
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return faces;
        }

        std::vector<int> Edge::getAdjacentVertices() {
            std::vector<int> vertices;
            std::pair<int, int> hId = halfEdgeId;
            vertices.push_back(mesh->mHalfEdges[hId.first][hId.second].headId);
            hId = mesh->mHalfEdges[hId.first][hId.second].pairId;
            vertices.push_back(mesh->mHalfEdges[hId.first][hId.second].headId);
            return vertices;
        }

        std::vector<int> Edge::getAdjacentFaces() {
            std::vector<int> faces;
            std::pair<int, int> hId = halfEdgeId;
            int faceId = mesh->mHalfEdges[hId.first][hId.second].faceId;
            if (!mesh->mFaces[faceId].isVirtual) {
                faces.push_back(faceId);
            }
            hId = mesh->mHalfEdges[hId.first][hId.second].pairId;
            faceId = mesh->mHalfEdges[hId.first][hId.second].faceId;
            if (!mesh->mFaces[faceId].isVirtual) {
                faces.push_back(faceId);
            }
            return faces;
        }

        std::vector<int> Face::getAdjacentVertices() {
            std::vector<int> vertices;
            std::pair<int, int> hId = halfEdgeId;
            do {
                vertices.push_back(mesh->mHalfEdges[hId.first][hId.second].headId);
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return vertices;
        }

        std::vector<int> Face::getAdjacentEdges() {
            std::vector<int> edges;
            std::pair<int, int> hId = halfEdgeId;
            do {
                edges.push_back(mesh->mHalfEdges[hId.first][hId.second].edgeId);
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return edges;
        }

        std::vector<int> Face::getAdjacentFaces() {
            std::vector<int> faces;
            std::pair<int, int> hId = halfEdgeId;
            do {
                std::pair<int, int> pId = mesh->mHalfEdges[hId.first][hId.second].pairId;
                int faceId = mesh->mHalfEdges[pId.first][pId.second].faceId;
                if (!mesh->mFaces[faceId].isVirtual) {
                    faces.push_back(faceId);
                }
                hId = mesh->mHalfEdges[hId.first][hId.second].nextId;
            } while (hId != halfEdgeId);
            return faces;
        }
        
        float Mesh::norm(glm::vec3 a) {
            return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        }

        glm::vec3 Mesh::normalize(glm::vec3 a) {
            return a / norm(a);
        }

        glm::vec3 Mesh::crossProduct(glm::vec3 a, glm::vec3 b) {
            float x = a.y * b.z - a.z * b.y;
            float y = a.z * b.x - a.x * b.z;
            float z = a.x * b.y - a.y * b.x;
            return glm::vec3(x, y, z);
        }

        float Mesh::sin(glm::vec3 a, glm::vec3 b) {
            return norm(crossProduct(a, b)) / (norm(a) * norm(b));
        }

        int Mesh::find(int x, glm::ivec3 indices) {
            if (indices[0] == x) {
                return 0;
            } else if (indices[1] == x) {
                return 1;
            } else {
                return 2;
            }
        }

        std::vector<int> Mesh::match(glm::ivec3 indices, std::vector<int> edges) {
            assert(edges.size() == 3);
            std::vector<int> ind = {indices[0], indices[1], indices[2], indices[0]};
            std::vector<int> matchedEdges;
            for (int i = 0; i < 3; i ++) {
                for (int j = 0; j < 3; j ++) {
                    std::vector<int> v = getAdjacentVertices(Entity::EDGE, edges[j]);
                    if (std::min(v[0], v[1]) == std::min(ind[i], ind[i + 1]) && std::max(v[0], v[1]) == std::max(ind[i], ind[i + 1])) {
                        matchedEdges.push_back(edges[j]);
                        break;
                    }
                }
            }
            return matchedEdges;
        }

        Mesh::Mesh() {
            mHalfEdges.clear();
            mVertices.clear();
            mEdges.clear();
            mFaces.clear();
            mVirtualFaces.clear();
            isConnected = false;
        }

        Mesh::Mesh(int numFaces, int numVertices, glm::ivec3 *pFaces, glm::vec3 *pVertices, glm::vec3 *pNormals) {
            mFaces.resize(numFaces);
            mVertices.resize(numVertices);
            for (int i = 0; i < numVertices; i ++) {
                mVertices[i] = {i, false, glm::vec3(0), pVertices[i], {-1, -1}, this};               
                if (pNormals != nullptr) {
                    mVertices[i].normal = pNormals[i];
                }
            }
            for (int i = 0; i < numFaces; i ++) {
                mFaces[i] = {i, false, glm::vec3(0), pFaces[i], {-1, -1}, this};
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = normalize(crossProduct(a, b));
            }
        }

        bool Mesh::setVertices(int numVertices, glm::vec3 *pVertices, glm::vec3 *pNormals) {
            if (isConnected) {
                std::cout << "Cannot set vertices. Mesh is already connected!" << std::endl;
                return false;
            }
            mVertices.resize(numVertices);
            for (int i = 0; i < numVertices; i ++) {
                mVertices[i] = {i, false, glm::vec3(0), pVertices[i], {-1, -1}, this};
                if (pNormals != nullptr) {
                    mVertices[i].normal = pNormals[i];
                }
            }
            return true;
        }

        bool Mesh::setFaces(int numFaces, glm::ivec3 *pFaces) {
            if (isConnected) {
                std::cout << "Cannot set faces. Mesh is already connected!" << std::endl;
                return false;
            }
            mFaces.resize(numFaces);
            for (int i = 0; i < numFaces; i ++) {
                mFaces[i] = {i, false, glm::vec3(0), pFaces[i], {-1, -1}, this};
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = normalize(crossProduct(a, b));
            }
            return true;
        }

        bool Mesh::computeAndSetVertexNormals() {
            if (!isConnected) {
                std::cout << "Cannot compute vertex normals. Mesh is not connected!" << std::endl;
                return false;
            }
            for (int i = 0; i < mVertices.size(); i ++) {
                std::vector<int> adjacentFacesIds = getAdjacentFaces(Entity::VERTEX, i);
                glm::vec3 normal = glm::vec3(0);
                for (int j = 0; j < adjacentFacesIds.size(); j ++) {
                    int index = find(i, mFaces[adjacentFacesIds[j]].indices);
                    glm::vec3 a = mVertices[mFaces[adjacentFacesIds[j]].indices[(index + 1) % 3]].position - mVertices[mFaces[adjacentFacesIds[j]].indices[index]].position;
                    glm::vec3 b = mVertices[mFaces[adjacentFacesIds[j]].indices[(index + 2) % 3]].position - mVertices[mFaces[adjacentFacesIds[j]].indices[index]].position;
                    normal += mFaces[adjacentFacesIds[j]].normal * sin(a, b) / (norm(a) * norm(b));
                }
                normal = normalize(normal);
                mVertices[i].normal = normal;
            }
            return true;
        }

        std::vector<int> Mesh::getAdjacentVertices(Entity entity, int id) {
            if (!isConnected) {
                std::cout << "Cannot get adjacent vertices. Mesh is not connected!" << std::endl;
                return std::vector<int>();
            }
            if (entity == Entity::VERTEX) {
                return mVertices[id].getAdjacentVertices();
            } else if (entity == Entity::EDGE) {
                return mEdges[id].getAdjacentVertices();
            } else {
                return mFaces[id].getAdjacentVertices();
            }
        }

        std::vector<int> Mesh::getAdjacentEdges(Entity entity, int id) {
            if (!isConnected) {
                std::cout << "Cannot get adjacent edges. Mesh is not connected!" << std::endl;
                return std::vector<int>();
            }
            if (entity == Entity::VERTEX) {
                return mVertices[id].getAdjacentEdges();
            } else if (entity == Entity::FACE) {
                return mFaces[id].getAdjacentEdges();
            } else {
                std::cout << "Cannot get adjacent edges for an edge. Not implemented!" << std::endl;
                return std::vector<int>();
            }
        }

        std::vector<int> Mesh::getAdjacentFaces(Entity entity, int id) {
            if (!isConnected) {
                std::cout << "Cannot get adjacent faces. Mesh is not connected!" << std::endl;
                return std::vector<int>();
            }
            if (entity == Entity::VERTEX) {
                return mVertices[id].getAdjacentFaces();
            } else if (entity == Entity::EDGE) {
                return mEdges[id].getAdjacentFaces();
            } else {
                return mFaces[id].getAdjacentFaces();
            }
        }

        std::vector<glm::vec3> Mesh::getVertices() {
            if (mVertices.size() == 0) {
                return std::vector<glm::vec3>();
            }
            std::vector<glm::vec3> vertices(mVertices.size());
            for (int i = 0; i < vertices.size(); i ++) {
                vertices[i] = mVertices[i].position;
            }
            return vertices;
        }

        std::vector<glm::vec3> Mesh::getNormals() {
            if (mVertices.size() == 0) {
                return std::vector<glm::vec3>();
            }
            std::vector<glm::vec3> normals(mVertices.size());
            for (int i = 0; i < normals.size(); i ++) {
                normals[i] = mVertices[i].normal;
            }
            return normals;
        }

        std::vector<glm::ivec3> Mesh::getFaces() {
            if (mFaces.size() == 0) {
                return std::vector<glm::ivec3>();
            }
            std::vector<glm::ivec3> faces(mFaces.size() - mVirtualFaces.size());
            for (int i = 0; i < faces.size(); i ++) {
                faces[i] = mFaces[i].indices;
            }
            return faces;
        }

        Vertex* Mesh::getVertex(int id) {
            if (id < mVertices.size()) {
                return &mVertices[id];
            }
            return nullptr;
        }

        Edge* Mesh::getEdge(int id) {
            if (id < mEdges.size()) {
                return &mEdges[id];
            }
            return nullptr;
        }

        Face* Mesh::getFace(int id) {
            if (id < mFaces.size()) {
                return &mFaces[id];
            }
            return nullptr;
        }

        void Mesh::connect() {
            if (isConnected) {
                return;
            }
            std::map<std::pair<int, int>, int> vertexToEdge;
            for (int i = 0; i < mFaces.size(); i ++) {
                int a = mFaces[i].indices[0];
                int b = mFaces[i].indices[1];
                int c = mFaces[i].indices[2];
                std::pair<int, int> faceHalfEdges[3];
                if (vertexToEdge.find({std::min(a, b), std::max(a, b)}) == vertexToEdge.end()) {
                    vertexToEdge.insert({{std::min(a, b), std::max(a, b)}, mEdges.size()});
                    int edgeId = mEdges.size();
                    int halfEdgeId = mHalfEdges.size();
                    mEdges.push_back(Edge());
                    mHalfEdges.push_back({HalfEdge(), HalfEdge()});
                    mEdges[edgeId] = {edgeId, true, {halfEdgeId, 0}, this};
                    mHalfEdges[halfEdgeId][0] = {{halfEdgeId, 0}, {halfEdgeId, 1}, {-1, -1}, a, edgeId, i, this};
                    mHalfEdges[halfEdgeId][1] = {{halfEdgeId, 1}, {halfEdgeId, 0}, {-1, -1}, b, edgeId, -1, this};
                    faceHalfEdges[0] = {halfEdgeId, 0};
                    mFaces[i].halfEdgeId = faceHalfEdges[0];
                    if (mVertices[a].halfEdgeId.first < 0) {
                        mVertices[a].halfEdgeId = {halfEdgeId, 0};
                    }
                    if (mVertices[b].halfEdgeId.first < 0) {
                        mVertices[b].halfEdgeId = {halfEdgeId, 1};
                    }
                } else {
                    int edgeId = vertexToEdge[{std::min(a, b), std::max(a, b)}];
                    mEdges[edgeId].isBoundary = false;
                    std::pair<int, int> hId = mEdges[edgeId].halfEdgeId;
                    faceHalfEdges[0] = mHalfEdges[hId.first][hId.second].pairId;
                    mFaces[i].halfEdgeId = faceHalfEdges[0];
                    mHalfEdges[faceHalfEdges[0].first][faceHalfEdges[0].second].faceId = i;
                    vertexToEdge.erase({std::min(a, b), std::max(a, b)});
                }
                if (vertexToEdge.find({std::min(b, c), std::max(b, c)}) == vertexToEdge.end()) {
                    vertexToEdge.insert({{std::min(b, c), std::max(b, c)}, mEdges.size()});
                    int edgeId = mEdges.size();
                    int halfEdgeId = mHalfEdges.size();
                    mEdges.push_back(Edge());
                    mHalfEdges.push_back({HalfEdge(), HalfEdge()});
                    mEdges[edgeId] = {edgeId, true, {halfEdgeId, 0}, this};
                    mHalfEdges[halfEdgeId][0] = {{halfEdgeId, 0}, {halfEdgeId, 1}, {-1, -1}, b, edgeId, i, this};
                    mHalfEdges[halfEdgeId][1] = {{halfEdgeId, 1}, {halfEdgeId, 0}, {-1, -1}, c, edgeId, -1, this};
                    faceHalfEdges[1] = {halfEdgeId, 0};
                    if (mVertices[c].halfEdgeId.first < 0) {
                        mVertices[c].halfEdgeId = {halfEdgeId, 1};
                    }
                } else {
                    int edgeId = vertexToEdge[{std::min(b, c), std::max(b, c)}];
                    mEdges[edgeId].isBoundary = false;
                    std::pair<int, int> hId = mEdges[edgeId].halfEdgeId;
                    faceHalfEdges[1] = mHalfEdges[hId.first][hId.second].pairId;
                    mHalfEdges[faceHalfEdges[1].first][faceHalfEdges[1].second].faceId = i;
                    vertexToEdge.erase({std::min(b, c), std::max(b, c)});
                }
                if (vertexToEdge.find({std::min(c, a), std::max(c, a)}) == vertexToEdge.end()) {
                    vertexToEdge.insert({{std::min(c, a), std::max(c, a)}, mEdges.size()});
                    int edgeId = mEdges.size();
                    int halfEdgeId = mHalfEdges.size();
                    mEdges.push_back(Edge());
                    mHalfEdges.push_back({HalfEdge(), HalfEdge()});
                    mEdges[edgeId] = {edgeId, true, {halfEdgeId, 0}, this};
                    mHalfEdges[halfEdgeId][0] = {{halfEdgeId, 0}, {halfEdgeId, 1}, {-1, -1}, c, edgeId, i, this};
                    mHalfEdges[halfEdgeId][1] = {{halfEdgeId, 1}, {halfEdgeId, 0}, {-1, -1}, a, edgeId, -1, this};
                    faceHalfEdges[2] = {halfEdgeId, 0};
                } else {
                    int edgeId = vertexToEdge[{std::min(c, a), std::max(c, a)}];
                    mEdges[edgeId].isBoundary = false;
                    std::pair<int, int> hId = mEdges[edgeId].halfEdgeId;
                    faceHalfEdges[2] = mHalfEdges[hId.first][hId.second].pairId;
                    mHalfEdges[faceHalfEdges[2].first][faceHalfEdges[2].second].faceId = i;
                    vertexToEdge.erase({std::min(c, a), std::max(c, a)});
                }
                mHalfEdges[faceHalfEdges[0].first][faceHalfEdges[0].second].nextId = faceHalfEdges[1];
                mHalfEdges[faceHalfEdges[1].first][faceHalfEdges[1].second].nextId = faceHalfEdges[2];
                mHalfEdges[faceHalfEdges[2].first][faceHalfEdges[2].second].nextId = faceHalfEdges[0];
            }
            std::map<int, std::vector<int>> boundaryVertices;
            for (auto iter = vertexToEdge.begin(); iter != vertexToEdge.end(); iter ++) {
                if (boundaryVertices.find(iter->first.first) == boundaryVertices.end()) {
                    boundaryVertices.insert({iter->first.first, std::vector<int>(1, iter->second)});
                } else {
                    boundaryVertices[iter->first.first].push_back(iter->second);
                }
                if (boundaryVertices.find(iter->first.second) == boundaryVertices.end()) {
                    boundaryVertices.insert({iter->first.second, std::vector<int>(1, iter->second)});
                } else {
                    boundaryVertices[iter->first.second].push_back(iter->second);
                }
            }
            vertexToEdge.clear();
            std::set<std::pair<int, int>> boundaryHalfEdges;
            for (auto iter = boundaryVertices.begin(); iter != boundaryVertices.end(); iter ++) {
                int vertexId = iter->first;
                int edgeId[] = {iter->second[0], iter->second[1]};
                std::pair<int, int> hId[] = {mEdges[edgeId[0]].halfEdgeId, mEdges[edgeId[1]].halfEdgeId};
                std::pair<int, int> pId[] = {mHalfEdges[hId[0].first][hId[0].second].pairId, mHalfEdges[hId[1].first][hId[1].second].pairId};
                if (vertexId == mHalfEdges[pId[0].first][pId[0].second].headId) {
                    mHalfEdges[pId[1].first][pId[1].second].nextId = pId[0];
                } else {
                    mHalfEdges[pId[0].first][pId[0].second].nextId = pId[1];
                }
                boundaryHalfEdges.insert(pId[0]);
                boundaryHalfEdges.insert(pId[1]);
            }
            boundaryVertices.clear();
            while (!boundaryHalfEdges.empty()) {
                std::pair<int, int> hId = *boundaryHalfEdges.begin();
                std::pair<int, int> tId = hId;
                int faceId = mFaces.size();
                mFaces.push_back(Face());
                mFaces[faceId] = {faceId, true, glm::vec3(0), glm::ivec3(0), hId, this};
                mVirtualFaces.push_back(faceId);
                do {
                    mHalfEdges[tId.first][tId.second].faceId = faceId;
                    boundaryHalfEdges.erase(tId);
                    tId = mHalfEdges[tId.first][tId.second].nextId;
                } while (tId != hId);
            }
            isConnected = true;
        }

        void Mesh::send(V::Viewer &viewer) {
            std::vector<glm::vec3> vertices = getVertices();
            std::vector<glm::vec3> normals = getNormals();
            std::vector<glm::ivec3> triangles = getFaces();
            viewer.setVertices(mVertices.size(), vertices.data());
            viewer.setNormals(mVertices.size(), normals.data());
            viewer.setTriangles(mFaces.size() - mVirtualFaces.size(), triangles.data());
        }

        void Mesh::destroy() {
            mHalfEdges.clear();
            mVertices.clear();
            mEdges.clear();
            mFaces.clear();
            mVirtualFaces.clear();
            isConnected = false;
        }

        void Mesh::createSquareMesh(int m, int n, float s) {
            destroy();
            std::vector<glm::vec3> vertices, normals;
            std::vector<glm::ivec3> triangles;
            for (int i = 0; i <= m; i ++) {
                for (int j = 0; j <= n; j ++) {
                    vertices.push_back(glm::vec3(-s + j / (float) n, s - i / (float) m, 0));
                    normals.push_back(glm::vec3(0, 0, 1));
                }
            }
            for (int i = 0; i < m; i ++) {
                for (int j = 0; j < n; j ++) {
                    triangles.push_back(glm::ivec3((i + 1) * (n + 1) + j, i * (n + 1) + j + 1, i * (n + 1) + j));
                    triangles.push_back(glm::ivec3((i + 1) * (n + 1) + j, (i + 1) * (n + 1) + j + 1, i * (n + 1) + j + 1));
                }
            }
            setVertices((m + 1) * (n + 1), vertices.data(), normals.data());
            setFaces(2 * m * n, triangles.data());
            connect();
        }

        void Mesh::createSphereMesh(int m, int n, float r) {
            destroy();
            std::vector<glm::vec3> vertices(1, glm::vec3(0, 0, r)), normals(1, glm::vec3(0, 0, 1));
            std::vector<glm::ivec3> triangles;
            for (int i = 1; i < n; i ++) {
                for (int j = 0; j < m; j ++) {
                    double phi = 2 * j * glm::pi<double>() / m;
                    double theta = i * glm::pi<double>() / n;
                    vertices.push_back(glm::vec3(r * glm::sin(theta) * glm::cos(phi), r * glm::sin(theta) * glm::sin(phi), r * glm::cos(theta)));
                    normals.push_back(vertices.back() / r);
                }
            }
            vertices.push_back(glm::vec3(0, 0, -r));
            normals.push_back(vertices.back() / r);
            for (int i = 1; i < m; i ++) {
                triangles.push_back(glm::ivec3(0, i, i + 1));
            }
            triangles.push_back(glm::ivec3(0, m, 1));
            for (int i = 1; i < n - 1; i ++) {
                for (int j = 0; j < m - 1; j ++) {
                    triangles.push_back(glm::ivec3((i - 1) * m + 1 + j, i * m + 1 + j, (i - 1) * m + j + 2));
                    triangles.push_back(glm::ivec3(i * m + 1 + j, i * m + j + 2, (i - 1) * m + j + 2));
                }
                triangles.push_back(glm::ivec3(i * m, (i + 1) * m, (i - 1) * m + 1));
                triangles.push_back(glm::ivec3((i + 1) * m, i * m + 1, (i - 1) * m + 1));
            }
            for (int i = m * (n - 2) + 1; i < m * (n - 1); i ++) {
                triangles.push_back(glm::ivec3(i, m * (n - 1) + 1, i + 1));
            }
            triangles.push_back(glm::ivec3(m * (n - 1), m * (n - 1) + 1, m * (n - 2) + 1));
            setVertices(m * (n - 1) + 2, vertices.data(), normals.data());
            setFaces(2 * m * (n - 1), triangles.data());
            connect();
        }

        bool Mesh::load(std::string filename){
            destroy();
            std::fstream file;
            file.open(filename);
            if(!file.is_open()){
                std::cout << "Cannot load mesh. Error in opening file!" << std::endl;
                return false;
            }
            std::string line;
            std::vector<glm::vec3> vertices, normals;
            std::map<int, int> v_idx_to_obj_v_idx, n_idx_to_obj_n_idx;
            std::vector<glm::vec3> objectVertices, objectNormals;
            std::vector<glm::ivec3> objectFaces;
            while(getline(file, line)){
                if(line[0] == 'v' && line[1] == ' '){
                    glm::vec3 vertex;
                    sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    vertices.push_back(vertex);
                }
                else if(line[0] == 'v' && line[1] == 'n'){
                    glm::vec3 normal;
                    sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                    normals.push_back(normalize(normal));
                }
                else if(line[0] == 'f'){
                    int v_idx[3], t_idx[3], n_idx[3] = {-1, -1, -1};
                    glm::ivec3 face;
                    if (sscanf(line.c_str(), "f %d %d %d", &v_idx[0], &v_idx[1], &v_idx[2]) == 3);
                    else if (sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v_idx[0], &t_idx[0], &v_idx[1], &t_idx[1], &v_idx[2], &t_idx[2]) == 6);
                    else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v_idx[0], &t_idx[0], &n_idx[0], &v_idx[1], &t_idx[1], &n_idx[1], &v_idx[2], &t_idx[2], &n_idx[2]) == 9);
                    else sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v_idx[0], &n_idx[0], &v_idx[1], &n_idx[1], &v_idx[2], &n_idx[2]);
                    for (int i = 0; i < 3; i ++) {
                        if (v_idx_to_obj_v_idx.find(v_idx[i]) == v_idx_to_obj_v_idx.end()) {
                            v_idx_to_obj_v_idx.insert({v_idx[i], objectVertices.size()});
                            objectVertices.push_back(vertices[v_idx[i] - 1]);
                        }
                        v_idx[i] = v_idx_to_obj_v_idx[v_idx[i]];
                        if (n_idx[i] == -1) {
                            continue;
                        }
                        if (n_idx_to_obj_n_idx.find(n_idx[i]) == n_idx_to_obj_n_idx.end()) {
                            n_idx_to_obj_n_idx.insert({n_idx[i], objectNormals.size()});
                            objectNormals.push_back(normals[n_idx[i] - 1]);
                        }
                    }
                    face = glm::ivec3(v_idx[0], v_idx[1], v_idx[2]);
                    objectFaces.push_back(face);
                }
            }
            file.close();
            setVertices(objectVertices.size(), objectVertices.data(), objectNormals.data());
            setFaces(objectFaces.size(), objectFaces.data());
            connect();
            if (objectNormals.size() == 0) {
                computeAndSetVertexNormals();
            }
            return true;
        }

        bool Mesh::smooth(int numIter, float lambda, bool taubin, float mu) {
            if (!isConnected) {
                std::cout << "Cannot smooth mesh. Mesh is not connected!" << std::endl;
                return false;
            }
            std::vector<glm::vec3> displacement(mVertices.size());
            std::vector<std::vector<int>> adjacentVertices(mVertices.size());
            for (int i = 0; i < mVertices.size(); i ++) {
                adjacentVertices[i] = getAdjacentVertices(Entity::VERTEX, i);
            }
            for (int i = 0; i < numIter; i ++) {
                for (int j = 0; j < mVertices.size(); j ++) {
                    glm::vec3 sum(0);
                    for (int id : adjacentVertices[j]) {
                        sum += mVertices[id].position - mVertices[j].position;
                    }
                    displacement[j] = sum / (float) adjacentVertices[j].size();
                }
                for (int j = 0; j < mVertices.size(); j ++) {
                    mVertices[j].position += lambda * displacement[j];
                }
                if (!taubin) {
                    continue;
                }
                for (int j = 0; j < mVertices.size(); j ++) {
                    glm::vec3 sum(0);
                    for (int id : adjacentVertices[j]) {
                        sum += mVertices[id].position - mVertices[j].position;
                    }
                    displacement[j] = sum / (float) adjacentVertices[j].size();
                }
                for (int j = 0; j < mVertices.size(); j ++) {
                    mVertices[j].position += mu * displacement[j];
                }
            }
            for (int i = 0; i < mFaces.size() - mVirtualFaces.size(); i ++) {
                glm::vec3 a = mVertices[mFaces[i].indices[1]].position - mVertices[mFaces[i].indices[0]].position;
                glm::vec3 b = mVertices[mFaces[i].indices[2]].position - mVertices[mFaces[i].indices[0]].position;
                mFaces[i].normal = normalize(crossProduct(a, b));
            }
            computeAndSetVertexNormals();
            return true;
        }

        bool Mesh::subdivide() {
            if (!isConnected) {
                std::cout << "Cannot subdivide mesh. Mesh is not connected!" << std::endl;
                return false;
            }
            int numVertices = mVertices.size(), numEdges = mEdges.size(), numFaces = mFaces.size() - mVirtualFaces.size();
            std::vector<glm::vec3> oddVertices(numEdges, glm::vec3(0)), evenVertices(numVertices, glm::vec3(0));
            for (int i = 0; i < numEdges; i ++) {
                std::vector<int> adj_v = getAdjacentVertices(Entity::EDGE, i);
                if (mEdges[i].isBoundary) {
                    oddVertices[i] = (mVertices[adj_v[0]].position + mVertices[adj_v[1]].position) / 2.0f;
                } else {
                    std::vector<int> adj_f = getAdjacentFaces(Entity::EDGE, i);
                    std::vector<int> adj_f_v[2] = {getAdjacentVertices(Entity::FACE, adj_f[0]), getAdjacentVertices(Entity::FACE, adj_f[1])};
                    int opp_v[2], j = 0;
                    for (std::vector<int> f_v : adj_f_v) {
                        for (int v : f_v) {
                            if (v != adj_v[0] && v != adj_v[1]) {
                                opp_v[j ++] = v;
                            }
                        }
                    }
                    oddVertices[i] = (3.0f * mVertices[adj_v[0]].position + 3.0f * mVertices[adj_v[1]].position + mVertices[opp_v[0]].position + mVertices[opp_v[1]].position) / 8.0f;
                }                
            }
            for (int i = 0; i < numVertices; i ++) {
                std::vector<int> adj_v = getAdjacentVertices(Entity::VERTEX, i);
                int n = adj_v.size();
                glm::vec3 sum_adj_v = glm::vec3(0);
                for (int v : adj_v) {
                    sum_adj_v += mVertices[v].position;
                }
                float weight = 0.0f;
                if (mVertices[i].isBoundary) {
                    weight = 1.0f / 8.0f;
                } else {
                    if (n == 3) {
                        weight = 3.0f / 16.0f;
                    } else {
                        weight = 3.0f / (8.0f * n);
                    }
                }
                evenVertices[i] = (1 - n * weight) * mVertices[i].position + weight * sum_adj_v;
            }
            std::vector<glm::vec3> new_vertices(numVertices + numEdges, glm::vec3(0));
            for (int i = 0; i < numVertices + numEdges; i ++) {
                new_vertices[i] = (i < numVertices) ? evenVertices[i] : oddVertices[i - numVertices];
            }
            std::vector<glm::ivec3> new_faces(4 * numFaces, glm::ivec3(0));
            for (int i = 0; i < numFaces; i ++) {
                glm::ivec3 indices = mFaces[i].indices;
                std::vector<int> adj_e = getAdjacentEdges(Entity::FACE, i);
                adj_e = match(indices, adj_e);
                new_faces[4 * i] = glm::ivec3(indices[0], numVertices + adj_e[0], numVertices + adj_e[2]);
                new_faces[4 * i + 1] = glm::ivec3(numVertices + adj_e[0], indices[1], numVertices + adj_e[1]);
                new_faces[4 * i + 2] = glm::ivec3(numVertices + adj_e[1], indices[2], numVertices + adj_e[2]);
                new_faces[4 * i + 3] = numVertices + glm::ivec3(adj_e[2], adj_e[0], adj_e[1]);
            }
            destroy();
            setVertices(new_vertices.size(), new_vertices.data());
            setFaces(new_faces.size(), new_faces.data());
            connect();
            computeAndSetVertexNormals();
            return true;
        }

    }
}