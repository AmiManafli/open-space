#include "meshes/Model.h"

Model::Model(std::vector<Mesh>& meshes, glm::vec3 position) : position(position) {
    for (auto mesh : meshes) {
        this->meshes.push_back(mesh);
    }

    updateModelMatrix();
}

Model::Model(const std::string filename, glm::vec3 position) : Model(filename, position, glm::mat4(1.0f)) {
}

Model::Model(const std::string filename, glm::vec3 position, glm::mat4 model)
        : filename(filename), position(position), model(model) {
    loadModel(filename, meshes);

    auto modelTranslated = glm::translate(glm::mat4(1.0f), position);
    this->model = model * modelTranslated;
}

void Model::draw(ShaderProgram& shaderProgram) {
    for (auto &mesh : meshes) {
        mesh.draw(shaderProgram, model);
    }
}

void Model::updateModelMatrix() {
//    model = glm::mat4(1.0f);
}

void Model::loadModel(std::string filename, std::vector<Mesh>& meshes) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    processNode(meshes, scene->mRootNode, scene);
}

void Model::processNode(std::vector<Mesh>& meshes, aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(meshes, mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(meshes, node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(std::vector<Mesh>& meshes, aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        auto position = mesh->mVertices[i];
        vertex.position = glm::vec3(position.x, position.y, position.z);

        auto normal = mesh->mNormals[i];
        vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

        if (mesh->mTextureCoords[0]) {
            auto coords = mesh->mTextureCoords[0][i];
            vertex.textureCoord = glm::vec2(coords.x, coords.y);
        } else {
            vertex.textureCoord = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices, textures);
}
