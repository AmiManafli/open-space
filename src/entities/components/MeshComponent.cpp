#include <entities/components/MeshComponent.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void processNode(std::vector<MeshComponent *>& meshes, aiNode *node, const aiScene *scene);
MeshComponent* processMesh(std::vector<MeshComponent *>& meshes, aiMesh *mesh, const aiScene *scene);

std::vector<MeshComponent *> createMeshComponentsFromFile(std::string filename) {
    auto meshes = std::vector<MeshComponent *>();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    processNode(meshes, scene->mRootNode, scene);

    return meshes;
}

void processNode(std::vector<MeshComponent *>& meshes, aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(meshes, mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(meshes, node->mChildren[i], scene);
    }
}

void setupBuffers(MeshComponent *component) {
    glGenVertexArrays(1, &component->vao);
    glGenBuffers(1, &component->vbo);
    glGenBuffers(1, &component->ebo);

    glBindVertexArray(component->vao);

    glBindBuffer(GL_ARRAY_BUFFER, component->vbo);
    glBufferData(GL_ARRAY_BUFFER, component->vertices.size() * sizeof(MeshVertex), &component->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, component->indices.size() * sizeof(uint32_t), &component->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, textureCoord));
}

MeshComponent* processMesh(std::vector<MeshComponent *>& meshes, aiMesh *mesh, const aiScene *scene) {
    auto component = new MeshComponent {};

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        MeshVertex vertex;

        auto position = mesh->mVertices[i];
        vertex.position = glm::vec3(position.x, position.y, position.z);

        if (mesh->mNormals) {
            auto normal = mesh->mNormals[i];
            vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
        }

        if (mesh->mTextureCoords[0]) {
            auto coords = mesh->mTextureCoords[0][i];
            vertex.textureCoord = glm::vec2(coords.x, coords.y);
        } else {
            vertex.textureCoord = glm::vec2(0.0f, 0.0f);
        }

        component->vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            component->indices.push_back(face.mIndices[j]);
        }
    }

    setupBuffers(component);

    return component;
}
