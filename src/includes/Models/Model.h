//
// Created by michal on 2/7/25.
//

#ifndef MODEL_H
#define MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include "includes/Models/Mesh.h"
#include "includes/Shader.h"

//-----------------------------------------------------------------------------------
// Utility function to load a texture from file
//-----------------------------------------------------------------------------------
inline unsigned int TextureFromFile(const char *path, const string &directory);

class NewModel
{
    public:
    //-----------------------------------------------------------------------------------
    // Model data
    //-----------------------------------------------------------------------------------
    vector<Texture> texturesLoaded;
    vector<Mesh> meshes;
    string directory;
    unsigned int pFlags;

    //-----------------------------------------------------------------------------------
    // Constructor: expects a file path to a 3D model
    //-----------------------------------------------------------------------------------
    NewModel(string const &path, unsigned int pFlags = aiProcess_Triangulate) : pFlags(pFlags) { loadModel(path); }

    //-----------------------------------------------------------------------------------
    // Draw the model
    //-----------------------------------------------------------------------------------
    void Draw(Shader &shader) const;

    private:
    //-----------------------------------------------------------------------------------
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes
    //-----------------------------------------------------------------------------------
    void loadModel(string const &path);

    //-----------------------------------------------------------------------------------
    // Processes a node recursively, handling each mesh and its children nodes
    //-----------------------------------------------------------------------------------
    void processNode(aiNode *node, const aiScene *scene);

    //-----------------------------------------------------------------------------------
    // Processes a mesh and extracts relevant data to construct a Mesh object
    //-----------------------------------------------------------------------------------
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    //-----------------------------------------------------------------------------------
    // Checks and loads all material textures of a given type, returning them as Texture structs
    //-----------------------------------------------------------------------------------
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif  // MODEL_H
