#include "Viewport.h"

#include "../Backend/Backend.h"

Viewport::Viewport(const glm::vec2& position, const glm::vec2& size, bool isOrthographic)
    : m_position(position),
    m_size(size),
    m_isOrthographic(isOrthographic),
    m_orthoSize(1.0f),
    m_nearPlane(0.1f),
    m_farPlane(1000.0f),
    m_fov(1.0f),
    m_perspectiveMatrix(glm::mat4(1.0f)),
    m_orthographicMatrix(glm::mat4(1.0f))
{
    UpdateProjectionMatrices();
}

void Viewport::Update()
{
    // Pixel bounds
    m_leftPixel = m_position.x * Backend::GetCurrentWindowWidth();
    m_rightPixel = m_leftPixel + m_windowSpaceCoords.width;
    m_topPixel = Backend::GetCurrentWindowHeight() - (m_position.y * Backend::GetCurrentWindowHeight());
    m_bottomPixel = m_topPixel - m_windowSpaceCoords.height;

    // Window space co-ordinates
    m_windowSpaceCoords.width = m_size.x * Backend::GetCurrentWindowWidth();
    m_windowSpaceCoords.height = m_size.y * Backend::GetCurrentWindowHeight();
}

void Viewport::SetPerspective(float fov, float nearPlane, float farPlane)
{
    m_isOrthographic = false;
    m_fov = fov;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    UpdateProjectionMatrices();
}

void Viewport::SetOrthographic(float orthoSize, float nearPlane, float farPlane)
{
    m_isOrthographic = true;
    m_orthoSize = orthoSize;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    UpdateProjectionMatrices();
}

void Viewport::UpdateProjectionMatrices()
{
    int renderTargetWidth = Backend::GetCurrentWindowWidth();
    int renderTargetHeight = Backend::GetCurrentWindowHeight();

    float viewportWidth = m_size.x * renderTargetWidth;
    float viewportHeight = m_size.y * renderTargetHeight;

    if (viewportHeight <= 0)
    {
        m_aspect = 1.0f;
    }
    else
    {
        m_aspect = viewportWidth / viewportHeight;
    }

    m_perspectiveMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);

    float left = -m_orthoSize * m_aspect;
    float right = m_orthoSize * m_aspect;
    float bottom = -m_orthoSize;
    float top = m_orthoSize;
    m_orthographicMatrix = glm::orthoZO(left, right, bottom, top, m_nearPlane, m_farPlane);
}

void Viewport::SetPosition(const glm::vec2& position)
{
    m_position = position;
}

void Viewport::SetSize(const glm::vec2& size)
{
    m_size = size;
    UpdateProjectionMatrices();
}

glm::mat4 Viewport::GetProjectionMatrix() const
{
    if (IsOrthographic())
    {
        return m_orthographicMatrix;
    }
    else
    {
        return m_perspectiveMatrix;
    }
}

glm::vec2 Viewport::GetPosition() const
{
    return m_position;
}

glm::vec2 Viewport::GetSize() const
{
    return m_size;
}

SpaceCoords Viewport::GetWindowSpaceCoords() const
{
    return m_windowSpaceCoords;
}

int Viewport::GetLeftPixel() const
{
    return m_leftPixel;
}

int Viewport::GetTopPixel() const
{
    return m_topPixel;
}

int Viewport::GetBottomPixel() const
{
    return m_bottomPixel;
}

const bool Viewport::IsOrthographic() const
{
    return m_isOrthographic;
}