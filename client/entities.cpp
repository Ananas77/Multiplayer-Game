#ifndef ENTITIES_H
#define ENTITIES_H

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"

#include "shader.cpp"
#include "camera.cpp"

class Entity
{
public:
    Entity(float *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices, unsigned int texture, glm::vec3 pos, float w, float h, float d, glm::vec3 rot, float s)
        : w(w), h(h), d(d), texture(texture), numVertices(numVertices), numIndices(numIndices)
    {
        /*
         * Apply parameters
         */
        translate(pos);
        rotate(rot);
        scale(s);

        // Vertex Array
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertices, GL_STATIC_DRAW);

        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);

        // VAO
        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)3);
        glEnableVertexAttribArray(1);

        // texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void render(Shader shader, Camera camera)
    {
        // shader
        shader.use();

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // modelViewProj matrix
        shader.setMat4("transform", camera.getViewProj() * model);
        shader.setMat4("model", model);
        shader.setMat3("normalTransform", glm::mat3(glm::transpose(glm::inverse(model))));
        shader.setVec3("viewPos", camera.getPos());

        // Vertex Array
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Render
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void translate(glm::vec3 d)
    {
        position += d;
        model = glm::translate(model, d);
    }

    void rotate(glm::vec3 d)
    {
        rotation += d;
        model = glm::rotate(model, glm::radians(d.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(d.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(d.z), glm::vec3(0, 0, 1));
    }

    void scale(float d)
    {
        s *= d;
        model = glm::scale(model, glm::vec3(s));
    }

private:
    unsigned int numVertices,
        numIndices;
    float *vertices;
    float *indices;

    float w, h, d;
    float s = 1.0f;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);

    unsigned int texture;

    glm::mat4 model = glm::mat4(1.0f);

    unsigned int VBO, VAO, EBO;
};

#endif