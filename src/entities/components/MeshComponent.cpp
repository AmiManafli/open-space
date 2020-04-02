#include <entities/components/MeshComponent.h>


MeshComponent::MeshComponent(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices,
                             std::vector<MeshTexture>& textures, ShaderProgram *shaderProgram)
                                : vertices(vertices), indices(indices), textures(textures),
                                  shaderProgram(shaderProgram), mode(GL_TRIANGLES) {
    setupBuffers();
}

MeshComponent::~MeshComponent() {
    // TODO: Cleanup mesh buffers
}

std::vector<MeshComponent *> MeshComponent::createMeshComponentsFromFile(std::string filename, ShaderProgram *shaderProgram) {
    auto meshes = std::vector<MeshComponent *>();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }

    processNode(meshes, scene->mRootNode, scene, shaderProgram);

    return meshes;
}

void MeshComponent::processNode(std::vector<MeshComponent *>& meshes, aiNode *node, const aiScene *scene, ShaderProgram *shaderProgram) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(meshes, mesh, scene, shaderProgram));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(meshes, node->mChildren[i], scene, shaderProgram);
    }
}

void MeshComponent::setupBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, textureCoord));
}

MeshComponent* MeshComponent::processMesh(std::vector<MeshComponent *>& meshes, aiMesh *mesh, const aiScene *scene, ShaderProgram *shaderProgram) {
    std::vector<MeshComponent::MeshVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshComponent::MeshTexture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        MeshComponent::MeshVertex vertex;

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

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    shaderProgram = shaderProgram;

    return new MeshComponent(vertices, indices, textures, shaderProgram);
}

