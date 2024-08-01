#include "Model.h"
#include "assimp/scene.h"
#include <string>

Model::Model(const char *path)
{
    LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(const std::string &path)
{
    // todo: load model from file
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    // todo: process node
}

void Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    // todo: process mesh
}