#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct SpaceCoords
{
    float width;
    float height;
};

struct Viewport
{
public:
    Viewport(const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec2& size = { 1.0f, 1.0f }, bool isOrthographic = true);

    void Update();

    void SetOrthographic(float orthoSize, float nearPlane, float farPlane);
    void SetPerspective(float fov, float nearPlane, float farPlane);

    void SetPosition(const glm::vec2& position);
    void SetSize(const glm::vec2& size);

    const bool IsOrthographic() const;

    glm::mat4 GetProjectionMatrix() const;

    glm::vec2 GetPosition() const;
    glm::vec2 GetSize() const;
    SpaceCoords GetWindowSpaceCoords() const;
    int GetLeftPixel() const;
    int GetTopPixel() const;
    int GetBottomPixel() const;

private:
    glm::vec2 m_position;
    glm::vec2 m_size;

    bool m_isOrthographic;
    float m_orthoSize;
    float m_nearPlane;
    float m_farPlane;
    float m_fov;
    float m_aspect;

    float m_leftPixel;
    float m_rightPixel;
    float m_topPixel;
    float m_bottomPixel;

    glm::mat4 m_perspectiveMatrix;
    glm::mat4 m_orthographicMatrix;

    SpaceCoords m_windowSpaceCoords;

    void UpdateProjectionMatrices();
};