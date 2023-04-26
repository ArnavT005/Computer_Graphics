#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "viewer.hpp"

namespace V = COL781::Viewer;

namespace COL781 {
    namespace Mesh {
        
        enum Entity {
            VERTEX,
            EDGE,
            FACE
        };

        struct Vertex;
        struct Edge;
        struct Face;
        class Mesh;

        struct HalfEdge {
            std::pair<int, int> id;
            std::pair<int, int> pairId, nextId;
            int headId;
            int edgeId;
            int faceId;
            Mesh *mesh;
        };
        
        struct Vertex {
            int id;
            bool isBoundary;
            glm::vec3 normal;
            glm::vec3 position;
            std::pair<int, int> halfEdgeId;
            Mesh *mesh;
            std::vector<int> getAdjacentVertices();
            std::vector<int> getAdjacentEdges();
            std::vector<int> getAdjacentFaces();
        };

        struct Edge {
            int id;
            bool isBoundary;
            std::pair<int, int> halfEdgeId;
            Mesh *mesh;
            std::vector<int> getAdjacentVertices();
            std::vector<int> getAdjacentFaces();
        };

        struct Face {
            int id;
            bool isVirtual;
            glm::vec3 normal;
            glm::ivec3 indices;
            std::pair<int, int> halfEdgeId;
            Mesh *mesh;
            std::vector<int> getAdjacentVertices();
            std::vector<int> getAdjacentEdges();
            std::vector<int> getAdjacentFaces();
        };
        
        class Mesh {
            protected:
                std::vector<std::vector<HalfEdge>> mHalfEdges;
                std::vector<Vertex> mVertices;
                std::vector<Edge> mEdges;
                std::vector<Face> mFaces;
                std::vector<int> mVirtualFaces;
                bool isConnected;
                float norm(glm::vec3);
                float sin(glm::vec3, glm::vec3);
                int find(int, glm::ivec3);
                bool setVertices(int, glm::vec3*, glm::vec3* = nullptr);
                bool setFaces(int, glm::ivec3*);
                bool computeAndSetVertexNormals();
                void destroy();
                void connect();
                std::vector<int> getAdjacentFaces(Entity, int);
                std::vector<glm::vec3> getVertices();
                std::vector<glm::vec3> getNormals();
                std::vector<glm::ivec3> getFaces();

            public:
                Mesh();
                void createRectangleMesh(int, int, float = 0.5, float = 0.5);
                void createSphereMesh(int, int, float = 0.5);
                void createCylinderMesh(int, float = 0.5, float = 0.25);
                void send(V::Viewer*);
                virtual void update(float) = 0;

                friend struct HalfEdge;
                friend struct Vertex;
                friend struct Edge;
                friend struct Face;
        };
    }
}

#endif // MESH_HPP