#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "texture.h"
#include "mesh.h"
#include "kdtree.h"
#include "TriangoloPt.h"

using namespace std;
using namespace kd;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;


    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool terrain = false, bool gamma = false) : gammaCorrection(gamma)
    {
        //loadModel(path);
        if (terrain) {
            cout << "terreno" << endl;
            loadModel(path, VAOterreno, &IndexTerreno);
            alberoKD.costruisciAlbero(convertMapToTrPt());
            minMaxValues = getMinMaxXZFromMap();
        }
        else loadModel(path);
    }

    Model() {

    }

    // draws the model, and thus all its meshes
    void Draw(Shader* shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    vector<TriangoloPt> convertMapToTrPt() {
        vector<TriangoloPt> listaTrangoloPt;
        for (int i = 0; i < mappaTriangoloVertici.size(); i++) {
            std::vector<Vertex> listaVertTriangolo = mappaTriangoloVertici[i];
            TriangoloPt triangoloPt = TriangoloPt(listaVertTriangolo[0].Position, listaVertTriangolo[1].Position, listaVertTriangolo[2].Position);
            listaTrangoloPt.push_back(triangoloPt);
        }
        return listaTrangoloPt;
    }

    glm::vec3 updateCameraPositionOnMap(glm::vec3 posCamera, glm::vec3 posVecchia, float offset, bool tutti) {
        if (posCamera.x < minMaxValues.minX || posCamera.x > minMaxValues.maxX || posCamera.z < minMaxValues.minZ || posCamera.z > minMaxValues.maxZ) {
            return posVecchia;
        }

        TriangoloPt puntoCamera(glm::vec3(posCamera.x, posCamera.y - offset, posCamera.z));

        vector<TriangoloPt> vicino;
        if (tutti == true) {
            vicino = alberoKD.getPuntiEntroRaggio(puntoCamera, 8);
        }
        else {
            vicino = alberoKD.getPuntiEntroRaggio(puntoCamera, 2.5);
        }
        for (int i = 0; i < vicino.size(); i++) {

            //cout << "Triangolo " << i + 1 << endl;
            float y = getYOnPlaneFromXZ(vicino[i].listaVertici[0], vicino[i].listaVertici[1], vicino[i].listaVertici[2], posCamera.x, posCamera.z);

            if (isPointInsideTriangle(glm::vec3(posCamera.x, y, posCamera.z), vicino[i].listaVertici[0], vicino[i].listaVertici[1], vicino[i].listaVertici[2])) {
                posCamera.y = y + offset;
            }
        }
        /*cout << "punto: (" << posCamera.x << ", " << posCamera.y << ", " << posCamera.z << ")" << endl;*/
        return posCamera;
    }



private:

    struct MinMaxValues {
        float minX;
        float maxX;
        float minZ;
        float maxZ;
    };

    unsigned int VAOterreno, IndexTerreno, texture;
    map<int, vector<Vertex>> mappaTriangoloVertici;
    AlberoKD <TriangoloPt> alberoKD = AlberoKD <TriangoloPt>();
    MinMaxValues minMaxValues;

    //Restituisce la lista di tutti i vertici della mesh
    std::vector<Vertex> getVerticesFromMap() {
        std::vector<Vertex> listaVertici;
        for (int i = 0; i < mappaTriangoloVertici.size(); i++) {
            std::vector<Vertex> listaVertTriangolo = mappaTriangoloVertici[i];
            listaVertici.push_back(listaVertTriangolo[0]);
            listaVertici.push_back(listaVertTriangolo[1]);
            listaVertici.push_back(listaVertTriangolo[2]);
        }
        return listaVertici;
    }

    // Dati un punto e un triangolo rappresentato dai suoi tre vertici, questa funzione restituisce true se il punto è all'interno del triangolo, false altrimenti
    bool isPointInsideTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        // Calcoliamo le normali dei tre sottotriangoli che si ottengono aggiungendo il punto p ai tre vertici del triangolo
        glm::vec3 normal1 = glm::cross(b - a, p - a);
        glm::vec3 normal2 = glm::cross(c - b, p - b);
        glm::vec3 normal3 = glm::cross(a - c, p - c);

        // Se i prodotti scalari tra le normali e la normale del triangolo hanno lo stesso segno, allora il punto è all'interno del triangolo
        return (glm::dot(normal1, glm::cross(b - a, c - a)) >= 0.0f) &&
            (glm::dot(normal2, glm::cross(c - b, a - b)) >= 0.0f) &&
            (glm::dot(normal3, glm::cross(a - c, b - c)) >= 0.0f);
    }

    // Dati i vertici di un triangolo e le coordinate x e z di un punto P sul piano del triangolo,
    // questa funzione restituisce la coordinata y di P.
    float getYOnPlaneFromXZ(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float x, float z) {
        // Calcoliamo il vettore normale al piano del triangolo
        glm::vec3 n = glm::cross(v2 - v1, v3 - v1);

        // Calcoliamo l'equazione del piano che contiene il triangolo
        float a = n.x;
        float b = n.y;
        float c = n.z;
        float d = -glm::dot(n, v1);

        // Calcoliamo la coordinata y del punto P
        float y = (-a * x - c * z - d) / b;

        return y;
    }

    void loadModel(const std::string& filename, GLuint& VAO, unsigned int* sizeIndex) {

        // Caricamento del modelloAssimp::Importer importer;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

        std::vector<GLuint> indices;

        // Estrae i vertici e gli indici dal modello
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                const aiFace& face = mesh->mFaces[j];
                vector<Vertex> triangoloVertici;
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    unsigned int index = face.mIndices[k];
                    Vertex vertex;
                    vertex.Position = glm::vec3(mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z);
                    vertex.Normal = glm::vec3(mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z);
                    if (mesh->mTextureCoords[0]) {
                        vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
                    }
                    triangoloVertici.push_back(vertex);
                    indices.push_back(indices.size());
                }
                mappaTriangoloVertici[j] = triangoloVertici;
            }
        }

        std::vector<Vertex> vertices = getVerticesFromMap();

        // Crea i VBO e i VAO
        GLuint VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);

        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        *sizeIndex = indices.size();
    }


    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }


    MinMaxValues getMinMaxXZFromMap() {
        std::vector<Vertex> vertices = getVerticesFromMap();

        float minX = vertices[0].Position.x;
        float maxX = vertices[0].Position.x;
        float minZ = vertices[0].Position.z;
        float maxZ = vertices[0].Position.z;

        for (const auto& vertex : vertices) {
            if (vertex.Position.x < minX) {
                minX = vertex.Position.x;
            }
            if (vertex.Position.x > maxX) {
                maxX = vertex.Position.x;
            }
            if (vertex.Position.z < minZ) {
                minZ = vertex.Position.z;
            }
            if (vertex.Position.z > maxZ) {
                maxZ = vertex.Position.z;
            }
        }
        return { minX, maxX, minZ, maxZ };
    }


    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
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
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif
