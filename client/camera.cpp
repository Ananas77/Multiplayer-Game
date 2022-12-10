#ifndef CAMERA_H
#define CAMERA_H

#include "lib/glm/glm.hpp"
#include "lib/glm/ext/matrix_transform.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 rotation, float s, float aspectRatio, float fov, float near, float far)
        : aspectRatio(aspectRatio), fov(fov), near(near), far(far)
    {
        projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
        translate(position);
        rotate(rotation);
        scale(s);
        setViewProj();
    }

    glm::mat4 getViewProj()
    {
        return viewProj;
    }

    glm::vec3 getPos()
    {
        return position;
    }

    void translate(glm::vec3 d)
    {
        position += d;
        view = glm::translate(view, d);
        setViewProj();
    }

    void rotate(glm::vec3 d)
    {
        rotation += d;
        view = glm::rotate(view, glm::radians(d.x), glm::vec3(1, 0, 0));
        view = glm::rotate(view, glm::radians(d.y), glm::vec3(0, 1, 0));
        view = glm::rotate(view, glm::radians(d.z), glm::vec3(0, 0, 1));
        setViewProj();
    }

    void scale(float d)
    {
        s += d;
        view = glm::scale(view, glm::vec3(d));
        setViewProj();
    }

private:
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 viewProj = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    float s;
    float fov, aspectRatio, near, far;

    void setViewProj()
    {
        viewProj = projection * view;
    }
};

#endif