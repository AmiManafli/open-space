#ifndef CG1_PROJECT_MODEL_H
#define CG1_PROJECT_MODEL_H

#include <GLHeader.h>
#include "Mesh.h"
#include "shaders/ShaderProgram.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Axis.h>


class Model {
public:
    Model(const std::string filename, glm::vec3 position);
    Model(const std::string filename, glm::vec3 position, glm::mat4 model);
    Model(std::vector<Mesh> meshes, glm::vec3 position, glm::mat4 model);

    std::vector<Mesh> getMeshes() { return meshes; }

    void draw(ShaderProgram& shaderProgram);
    void draw(ShaderProgram& shaderProgram);

    static std::vector<Mesh> loadModel(std::string filename);

private:
    const std::string filename;
    std::vector<Mesh> meshes;

    glm::mat4 model;
    glm::vec3 position;

    void updateModelMatrix();
    static void processNode(std::vector<Mesh>& meshes, aiNode *node, const aiScene *scene);
    static Mesh processMesh(std::vector<Mesh>& meshes, aiMesh *mesh, const aiScene *scene);
};


#endif //CG1_PROJECT_MODEL_H
