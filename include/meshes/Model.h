#ifndef CG1_PROJECT_MODEL_H
#define CG1_PROJECT_MODEL_H

#include <GLHeader.h>
#include "Mesh.h"
#include "shaders/ShaderProgram.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model {
public:
    Model(std::vector<Mesh>& meshes, glm::vec3 position);
    Model(const std::string filename, glm::vec3 position);
    Model(const std::string filename, glm::vec3 position, glm::mat4 model);

    std::vector<Mesh> getMeshes() { return meshes; }

    void draw(ShaderProgram& shaderProgram);

private:
    const std::string filename;
    std::vector<Mesh> meshes;

    glm::mat4 model;
    glm::vec3 position;

    void updateModelMatrix();
    void loadModel();
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};


#endif //CG1_PROJECT_MODEL_H
