#include "ForwardZPrePassSampleRenderPipeline.hpp"
#include "ForwardSampleRenderPipeline.hpp"





const std::string ForwardZPrePassSampleRenderPipeline::targetRenderingGroupName = ForwardSampleRenderPipeline::targetRenderingGroupName;
const cg::RasterizationBasedRenderPipeline::TargetRenderingGroupNameList targetRenderingGroupNameList = { ForwardZPrePassSampleRenderPipeline::targetRenderingGroupName };

ForwardZPrePassSampleRenderPipeline::ForwardZPrePassSampleRenderPipeline(std::shared_ptr<ForwardRenderPipeline> forwardRenderPipeline)
	: ForwardZPrePassRenderPipeline("Forward Z Pre-Pass Sample Render Pipeline", DepthPass("Depth Pass", { std::make_shared<Position3Normal3DepthRenderPipeline>(targetRenderingGroupNameList, forwardRenderPipeline->getDepthStencilBuffer()) }, forwardRenderPipeline->getDepthStencilBuffer(), false), forwardRenderPipeline)
{
}

ForwardZPrePassSampleRenderPipeline::ForwardZPrePassSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> renderTarget)
	: ForwardZPrePassSampleRenderPipeline(std::make_shared<ForwardSampleRenderPipeline>(renderTarget, cg::API::shared.graphics()->createDepthStencilTesterState(cg::ComparisonFunction::equal, cg::ComparisonFunction::always, true, false, false), true, false))
{
}
