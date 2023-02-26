#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/glm.hpp>
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
            int id;
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
            private:
                std::vector<std::vector<HalfEdge>> mHalfEdges;
                std::vector<Vertex> mVertices;
                std::vector<Edge> mEdges;
                std::vector<Face> mFaces;
                std::vector<int> mVirtualFaces;
                bool isConnected;
            public:
                Mesh();
                Mesh(int, int, glm::ivec3*, glm::vec3*, glm::vec3* = nullptr);
                bool setVertices(int, glm::vec3*, glm::vec3* = nullptr);
                bool setFaces(int, glm::ivec3*);
                bool computeAndSetVertexNormals();
                std::vector<int> getAdjacentVertices(Entity, int);
                std::vector<int> getAdjacentEdges(Entity, int);
                std::vector<int> getAdjacentFaces(Entity, int);
                glm::vec3* getVertices();
                glm::vec3* getNormals();
                glm::ivec3* getFaces();
                Vertex* getVertex(int);
                Edge* getEdge(int);
                Face* getFace(int);
                void connect();
                void send(V::Viewer);
                void destroy();

                friend class HalfEdge;
                friend class Vertex;
                friend class Edge;
                friend class Face;
        };
    }
}

#endif // MESH_HPP