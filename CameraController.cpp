#include "CameraController.hpp"
#include <GraphicsLib.hpp>





void CameraController::control(cg::Camera& target)
{
    if (m_isEnabled)
    {
        auto& transform = *target.transform;
        switch (m_method)
        {
        case ObjectControllingMethod::freeLookWASD:
            m_freeLookControl.execute(transform, angleDeltaRadian);
            m_WASDControl.execute(transform, movementDelta);
            break;
        case ObjectControllingMethod::lookAt:
            m_lookAtControl.execute(transform, angleDeltaRadian, movementDelta, m_isControllingMethodChanged);
            break;
        }

        m_isControllingMethodChanged = false;
    }
}

void CameraController::changeControllingMethod(ObjectControllingMethod method)
{
    if (m_method != method)
    {
        m_isControllingMethodChanged = true;
        m_method = method;
    }
}

ObjectControllingMethod CameraController::getControllingMethod() const noexcept
{
    return m_method;
}

bool CameraController::isEnabled() const noexcept
{
    return m_isEnabled;
}

void CameraController::enable()
{
    m_isEnabled = true;
}

void CameraController::disable()
{
    m_isEnabled = false;
}
