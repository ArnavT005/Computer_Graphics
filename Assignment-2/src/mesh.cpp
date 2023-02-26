#include "mesh.hpp"

#include <iostream>
#include <map>

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
        
        glm::vec3 Mesh::crossProduct(glm::vec3 a, glm::vec3 b) {
            float x = a.y * b.z - a.z * b.y;
            float y = a.z * b.x - a.x * b.z;
            float z = a.x * b.y - a.y * b.x;
            float norm = sqrt(x * x + y * y + z * z);
            return glm::vec3(x, y, z) / norm;
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
                mFaces[i].normal = crossProduct(a, b);
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
                mFaces[i].normal = crossProduct(a, b);
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
                // insert code for computing vertex normals from adjacent faces
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
                return mVertices[id].getAdjacentEdges();
            } else if (entity == Entity::EDGE) {
                return mEdges[id].getAdjacentFaces();
            } else {
                return mFaces[id].getAdjacentFaces();
            }
        }

        glm::vec3* Mesh::getVertices() {
            if (mVertices.size() == 0) {
                return nullptr;
            }
            std::vector<glm::vec3> vertices(mVertices.size());
            for (int i = 0; i < vertices.size(); i ++) {
                vertices[i] = mVertices[i].position;
            }
            return vertices.data();
        }

        glm::vec3* Mesh::getNormals() {
            if (mVertices.size() == 0) {
                return nullptr;
            }
            std::vector<glm::vec3> normals(mVertices.size());
            for (int i = 0; i < normals.size(); i ++) {
                normals[i] = mVertices[i].normal;
            }
            return normals.data();
        }

        glm::ivec3* Mesh::getFaces() {
            if (mFaces.size() == 0) {
                return nullptr;
            }
            std::vector<glm::ivec3> faces(mFaces.size() - mVirtualFaces.size());
            for (int i = 0; i < faces.size(); i ++) {
                if (mFaces[i].isVirtual) {
                    continue;
                }
                faces[i] = mFaces[i].indices;
            }
            return faces.data();
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
            // deal with boundary edges (virtual face)
            isConnected = true;
        }

        void Mesh::send(V::Viewer viewer) {
            glm::vec3 *vertices = getVertices();
            glm::vec3 *normals = getNormals();
            glm::ivec3 *triangles = getFaces();
            viewer.setVertices(mVertices.size(), vertices);
            viewer.setNormals(mVertices.size(), normals);
            viewer.setTriangles(mFaces.size() - mVirtualFaces.size(), triangles);
        }

        void Mesh::destroy() {
            mHalfEdges.clear();
            mVertices.clear();
            mEdges.clear();
            mFaces.clear();
            mVirtualFaces.clear();
            isConnected = false;
        }

    }
}