#include "GeometryCreator.hpp"
#include "FullscreenQuad.hpp"





FullscreenQuad::FullscreenQuad()
{
	m_quad = std::make_shared<cg::DrawableObject>("Fullscreen Quad", GeometryCreator::createPlaneGeometry<vsinput::Position3UV2>(1.0, 1.0));
}

std::shared_ptr<cg::DrawableObject> FullscreenQuad::operator->() const noexcept
{
	return m_quad;
}
