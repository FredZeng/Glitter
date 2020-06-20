//
//  mesh.h
//  Glitter
//
//  Created by Tsang on 2020/6/16.
//

#ifndef mesh_h
#define mesh_h

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.hpp"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        
        setupMesh();
    }
    void Draw(Shader shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++) {
            glad_glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);
            
            // shader.setFloat(("material." + name + number).c_str(), i);
            glad_glUniform1i(glad_glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glad_glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glad_glActiveTexture(GL_TEXTURE0);
        
        // 绘制网络
        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glad_glBindVertexArray(0);
    }
private:
    unsigned int VBO, EBO;
    void setupMesh() {
        glad_glGenVertexArrays(1, &VAO);
        glad_glGenBuffers(1, &VBO);
        glad_glGenBuffers(1, &EBO);
        
        glad_glBindVertexArray(VAO);
        glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glad_glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // 顶点位置
        glad_glEnableVertexAttribArray(0);
        glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // 顶点法线
        glad_glEnableVertexAttribArray(1);
        glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // 顶点纹理坐标
        glad_glEnableVertexAttribArray(2);
        glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glad_glEnableVertexAttribArray(3);
        glad_glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glad_glEnableVertexAttribArray(4);
        glad_glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        
        glad_glBindVertexArray(0);
    }
};

#endif /* mesh_h */
