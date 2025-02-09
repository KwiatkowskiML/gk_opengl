//
// Created by michal on 2/8/25.
//

#include "includes/Models/Model.h"

//-----------------------------------------------------------------------------------
// Utility function to load a texture from file
//-----------------------------------------------------------------------------------
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);

    // Replace backslashes with forward slashes
    for (auto &c : filename) {
        if (c == '\\') {
            c = '/';
        }
    }
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    std::cout << filename << std::endl;
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//-----------------------------------------------------------------------------------
// Basic Model methods
//-----------------------------------------------------------------------------------
void NewModel::Draw(Shader &shader) const
{
    for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
}

void NewModel::loadModel(string const &path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, pFlags);  // consider adding GenNormals

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

//-----------------------------------------------------------------------------------
// Processes a node recursively
//-----------------------------------------------------------------------------------
void NewModel::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // process each child
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

//-----------------------------------------------------------------------------------
// Processing a mesh
//-----------------------------------------------------------------------------------
Mesh NewModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // positions
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;

        // normals
        if (mesh->HasNormals()) {
            vector.x      = mesh->mNormals[i].x;
            vector.y      = mesh->mNormals[i].y;
            vector.z      = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // vertex indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    // process materials
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material        = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

//-----------------------------------------------------------------------------------
// Loading texture
//-----------------------------------------------------------------------------------
vector<Texture> NewModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // skip if the texture has been already loaded
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++) {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        // if texture hasn't been loaded already, load it
        if (!skip) {
            Texture texture;
            texture.id   = TextureFromFile(str.C_Str(), this->directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);

            std::cout << "Texture loaded: " << texture.path << std::endl;
        }
    }
    return textures;
}
