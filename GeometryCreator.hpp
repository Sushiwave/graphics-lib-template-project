#pragma once
#include <GraphicsLib.hpp>
#include <ThirdParty/CPPLib/C++/SFINAE.hpp>
#include "SphereWithImGuiComponents.hpp"
#include "PlaneWithImGuiComponents.hpp"
#include "BoxWithImGuiComponents.hpp"
#include <GraphicsLib/Context.hpp>





class GeometryCreator
{
private:
	DEFINE_SFINAE_HAS_MEMBER(normal);
	DEFINE_SFINAE_HAS_MEMBER(uv);
private:
	static inline constexpr bool m_leftHanded
#ifdef CONTEXT_D3D11
		= true;
#endif
private:
	template <typename Vertex_>
	static void assignPositionInfoTo(std::vector<Vertex_>& vertices, const cg::GeometryCalculator::PositionList& vertexPositionList)
	{
		const auto vertexCount = static_cast<unsigned int>(vertices.size());
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			const auto& srcPosition = vertexPositionList[i];
			auto& destPosition = vertices[i].position;

			destPosition[0] = srcPosition.x;
			destPosition[1] = srcPosition.y;
			destPosition[2] = srcPosition.z;
		}
		const auto& randomPosition = vertices[0].position;
		if (randomPosition.size() == 4)
		{
			for (unsigned int i = 0; i < vertexCount; ++i)
			{
				vertices[i].position.at(3) = 1.0;
			}
		}
	}
	template <typename Vertex_>
	static void assignNormalInfoTo(std::vector<Vertex_>& vertices, const cg::GeometryCalculator::NormalList& vertexNormalList)
	{
		const auto vertexCount = static_cast<unsigned int>(vertices.size());
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			const auto& srcNormal = vertexNormalList[i];
			auto& destNormal = vertices[i].normal;

			destNormal[0] = srcNormal.x;
			destNormal[1] = srcNormal.y;
			destNormal[2] = srcNormal.z;
		}
	}
	template <typename Vertex_>
	static void assignUVInfoTo(std::vector<Vertex_>& vertices, const cg::GeometryCalculator::UVList& vertexUVList)
	{
		const auto vertexCount = static_cast<unsigned int>(vertices.size());
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			const auto& srcUV = vertexUVList[i];
			auto& destUV = vertices[i].uv;

			destUV[0] = srcUV.x;
			destUV[1] = srcUV.y;
		}
	}

	template <typename Vertex_>
	static void assignGeometryInfoTo(std::vector<Vertex_>& vertices, std::function<cg::GeometryCalculator::PositionList()> calcPosition, std::function<cg::GeometryCalculator::NormalList()> calcNormal, std::function<cg::GeometryCalculator::UVList()> calcUV, SFINAE_has_normal<Vertex_> = nullptr, SFINAE_has_uv<Vertex_> = nullptr)
	{
		assignPositionInfoTo(vertices, calcPosition());
		assignNormalInfoTo(vertices, calcNormal());
		assignUVInfoTo(vertices, calcUV());
	}
	template <typename Vertex_>
	static void assignGeometryInfoTo(std::vector<Vertex_>& vertices, std::function<cg::GeometryCalculator::PositionList()> calcPosition, std::function<cg::GeometryCalculator::NormalList()> calcNormal, std::function<cg::GeometryCalculator::UVList()> calcUV, SFINAE_has_normal<Vertex_> = nullptr, SFINAE_hasnt_uv<Vertex_> = nullptr)
	{
		assignPositionInfoTo(vertices, calcPosition());
		assignNormalInfoTo(vertices, calcNormal());
	}
	template <typename Vertex_>
	static void assignGeometryInfoTo(std::vector<Vertex_>& vertices, std::function<cg::GeometryCalculator::PositionList()> calcPosition, std::function<cg::GeometryCalculator::NormalList()> calcNormal, std::function<cg::GeometryCalculator::UVList()> calcUV, SFINAE_hasnt_normal<Vertex_> = nullptr, SFINAE_has_uv<Vertex_> = nullptr)
	{
		assignPositionInfoTo(vertices, calcPosition());
		assignUVInfoTo(vertices, calcUV());
	}
	template <typename Vertex_>
	static void assignGeometryInfoTo(std::vector<Vertex_>& vertices, std::function<cg::GeometryCalculator::PositionList()> calcPosition, std::function<cg::GeometryCalculator::NormalList()> calcNormal, std::function<cg::GeometryCalculator::UVList()> calcUV, SFINAE_hasnt_normal<Vertex_> = nullptr, SFINAE_hasnt_uv<Vertex_> = nullptr)
	{
		assignPositionInfoTo(vertices, calcPosition());
	}



	template <typename Vertex_>
	static cg::Geometry createGeometry(std::vector<Vertex_>& vertices, const cg::GeometryCalculator::Indices& indices, std::shared_ptr<cg::Shape> shape, const cg::Material& material)
	{
		auto graphicsAPI = cg::API::shared.graphics();

		const auto vertexBuffer   = graphicsAPI->createVertexBuffer(&vertices[0], static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(sizeof(Vertex_)));
		const auto indexBuffer    = graphicsAPI->createIndexBuffer(indices);
		const auto geometryBuffer = graphicsAPI->createGeometryBuffer(vertexBuffer, indexBuffer);
		auto partName = "main";
		const auto mainPart = cg::Geometry::Part(partName, material, geometryBuffer, cg::PrimitiveTopology::TRIANGLELIST);
		cg::Geometry::Parts parts(cg::Geometry::Parts::PartDict{ { partName, mainPart } });
		
		return cg::Geometry(shape, parts);
	}










public:
	template <typename Vertex_>
	static cg::Geometry createWavefrontOBJModelGeometry(std::shared_ptr<cg::WavefrontOBJModel> model, const cg::Material& material = cg::Material())
	{
		cg::Geometry::Parts::PartDict partDict;

		auto offset = -model->minXYZ - model->size * 0.5;

		const auto groupCount = static_cast<unsigned int>(model->groupList.size());
		for (unsigned int i = 0; i < groupCount; ++i)
		{
			const auto& group = model->groupList.at(i);

			std::vector<Vertex_> vertices;
			group.copyVertexDataTo(&vertices, offset);
			const auto vertexBuffer = cg::API::shared.graphics()->createVertexBuffer(&vertices[0], group.vertexCount, group.vertexByteSize);
			const auto indexBuffer = cg::API::shared.graphics()->createIndexBuffer(group.indices);
			const auto geometryBuffer = cg::API::shared.graphics()->createGeometryBuffer(vertexBuffer, indexBuffer);

			std::string groupName = group.name;
			if (groupName.empty())
			{
				groupName = "part" + std::to_string(i + 1);
			}

			partDict.emplace(groupName, cg::Geometry::Part(groupName, material, geometryBuffer, cg::PrimitiveTopology::TRIANGLELIST));
		}

		return cg::Geometry(std::make_shared<cg::AnyModel>(model->size), cg::Geometry::Parts(partDict));
	}
	template <typename Vertex_>
	static void createWavefronOBJModelGeometryAsync(const std::string& filename, cg::Material material, const std::function<void(const cg::Geometry& geometry)>& processingAfterCreating)
	{
		cg::WavefrontOBJModelLoader::loadAsync
		(
			filename, 
			[=](std::shared_ptr<cg::WavefrontOBJModel> model)
			{
				processingAfterCreating(createWavefrontOBJModelGeometry<Vertex_>(model, material));
			}, 
			m_leftHanded
		);
	}




	template <typename Vertex_>
	static cg::Geometry createPlaneGeometry(float width, float height, cg::Material material = cg::Material())
	{
		std::vector<Vertex_> vertices;
		vertices.resize(cg::GeometryCalculator::calcPlaneVertexCountTRIANGLELIST());

		assignGeometryInfoTo(vertices, [=](){ return cg::GeometryCalculator::calcPlanePositionTRIANGLELIST(1.0, 1.0, 0.0); }, [](){ return cg::GeometryCalculator::calcPlaneNormalTRIANGLELIST(); }, [](){ return cg::GeometryCalculator::calcPlaneUVTRIANGLELIST(); });
		
		return createGeometry(vertices, cg::GeometryCalculator::calcPlaneIndexTRIANGLELIST(), std::make_shared<PlaneWithImGuiComponents>(width, height), material);
	}
	template <typename Vertex_>
	static cg::Geometry createBoxGeometry(float width, float height, float depth, cg::Material material = cg::Material())
	{
		std::vector<Vertex_> vertices;
		vertices.resize(cg::GeometryCalculator::calcBoxVertexCountTRIANGLELIST());

		assignGeometryInfoTo(vertices, [=]() { return cg::GeometryCalculator::calcBoxVertexPositionTRIANGLELIST(1.0, 1.0, 1.0); }, []() { return cg::GeometryCalculator::calcBoxNormalTRIANGLELIST(); }, []() { return cg::GeometryCalculator::calcBoxUVTRIANGLELIST(); });

		return createGeometry(vertices, cg::GeometryCalculator::calcBoxIndexTRIANGLELIST(), std::make_shared<BoxWithImGuiComponents>(width, height, depth), material);
	}
	template <typename Vertex_>
	static cg::Geometry createSphereGeometry(float radiusX, float radiusY, float radiusZ, unsigned int sliceCount, unsigned int stackCount, cg::Material& material)
	{
		std::vector<Vertex_> vertices;
		vertices.resize(cg::GeometryCalculator::calcSphereVertexCountTRIANGLELIST(sliceCount, stackCount));

		cg::GeometryCalculator::PositionList positionListRef;
	
		assignGeometryInfoTo(vertices, [&]() { positionListRef = cg::GeometryCalculator::calcSphereVertexPositionTRIANGLELIST(0.5, sliceCount, stackCount); return positionListRef; }, [&]() { return cg::GeometryCalculator::calcSphereNormal(positionListRef);  }, [=]() { return cg::GeometryCalculator::calcSphereUVTRIANGLELIST(sliceCount, stackCount); });

		return createGeometry(vertices, cg::GeometryCalculator::calcSphereIndexTRIANGLELIST(sliceCount, stackCount), std::make_shared<SphereWithImGuiComponents>(radiusX, radiusY, radiusZ), material);
	}
	template <typename Vertex_>
	static cg::Geometry createSphereGeometry(float radius, unsigned int sliceCount, unsigned int stackCount, cg::Material& material)
	{
		return createSphereGeometry<Vertex_>(radius, radius, radius, sliceCount, stackCount, material);
	}
};
