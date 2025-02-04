#include "GFX/Visual/Material.h"
#include "GFX/Resource/GfxResources.h"

namespace ZE::GFX::Visual
{
	Material::Material(Graphics& gfx, const ColorF3& color, const std::string& name)
	{
		AddBind(Resource::PixelShader::Get(gfx, "SolidPS"));
		vertexLayout = std::make_shared<Data::VertexLayout>();
		vertexLayout->Append(VertexAttribute::Normal);
		auto vertexShader = Resource::VertexShader::Get(gfx, "PhongVS");
		AddBind(Resource::InputLayout::Get(gfx, vertexLayout, vertexShader));
		AddBind(std::move(vertexShader));

		GFX::Data::CBuffer::DCBLayout cbufferLayout;
		cbufferLayout.Add(DCBElementType::Color3, "solidColor");
		Data::CBuffer::DynamicCBuffer cbuffer(std::move(cbufferLayout));
		cbuffer["solidColor"] = color;
		pixelBuffer = Resource::ConstBufferExPixelCache::Get(gfx, name, std::move(cbuffer));
	}

	Material::Material(Graphics& gfx, const ColorF4& color, const std::string& name)
	{
		translucent = Math::NotEquals(color.RGBA.w, 1.0f);
		AddBind(Resource::PixelShader::Get(gfx, "PhongPS"));

		vertexLayout = std::make_shared<Data::VertexLayout>();
		vertexLayout->Append(VertexAttribute::Normal);
		auto vertexShader = Resource::VertexShader::Get(gfx, "PhongVS");
		AddBind(Resource::InputLayout::Get(gfx, vertexLayout, vertexShader));
		AddBind(std::move(vertexShader));

		GFX::Data::CBuffer::DCBLayout cbufferLayout;
		cbufferLayout.Add(DCBElementType::Color3, "specularColor");
		cbufferLayout.Add(DCBElementType::Float, "specularIntensity");
		cbufferLayout.Add(DCBElementType::Float, "specularPower");
		cbufferLayout.Add(DCBElementType::Color4, "materialColor");
		Data::CBuffer::DynamicCBuffer cbuffer(std::move(cbufferLayout));

		cbuffer["specularColor"] = ColorF3(1.0f, 1.0f, 1.0f);
		cbuffer["specularIntensity"] = 0.9f;
		cbuffer["specularPower"] = 0.409f;
		cbuffer["materialColor"] = color;
		pixelBuffer = Resource::ConstBufferExPixelCache::Get(gfx, name, std::move(cbuffer));
	}

	Material::Material(Graphics& gfx, aiMaterial& material, const std::string& path)
	{
		GFX::Data::CBuffer::DCBLayout cbufferLayout;
		cbufferLayout.Add(DCBElementType::Color3, "specularColor");
		cbufferLayout.Add(DCBElementType::Float, "specularIntensity");
		cbufferLayout.Add(DCBElementType::Float, "specularPower");

		std::string shaderCodePS = "PhongPS";
		std::string shaderCodeVS = "PhongVS";
		vertexLayout = std::make_shared<Data::VertexLayout>();
		vertexLayout->Append(VertexAttribute::Normal);

		aiString texFile;
		// Get diffuse texture
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFile) == aiReturn_SUCCESS)
		{
			diffuseTexture = Resource::Texture::Get(gfx, path + std::string(texFile.C_Str()), 32, true);
			translucent = diffuseTexture->HasAlpha();
			shaderCodePS += "Texture";
			shaderCodeVS += "Texture";
			vertexLayout->Append(VertexAttribute::Texture2D);
		}
		else
			cbufferLayout.Add(DCBElementType::Color4, "materialColor");

		// Get normal map texture
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFile) == aiReturn_SUCCESS)
		{
			normalMap = Resource::Texture::Get(gfx, path + std::string(texFile.C_Str()), 33);
			shaderCodePS += "Normal";
			shaderCodeVS += "Normal";
			vertexLayout->Append(VertexAttribute::Texture2D).Append(VertexAttribute::Bitangent);
		}

		// Get parallax map texture
		if (material.GetTexture(aiTextureType_HEIGHT, 0, &texFile) == aiReturn_SUCCESS)
		{
			parallaxMap = Resource::Texture::Get(gfx, path + std::string(texFile.C_Str()), 35);
			shaderCodePS += "Parallax";
			shaderCodeVS += "Parallax";
			cbufferLayout.Add(DCBElementType::Float, "parallaxScale");
		}

		// Get specular data
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFile) == aiReturn_SUCCESS)
		{
			specularMap = Resource::Texture::Get(gfx, path + std::string(texFile.C_Str()), 34, true);
			shaderCodePS += "Specular";
			vertexLayout->Append(VertexAttribute::Texture2D);
			cbufferLayout.Add(DCBElementType::Bool, "useSpecularPowerAlpha");
		}

		// Common elements
		AddBind(Resource::PixelShader::Get(gfx, shaderCodePS));
		auto vertexShader = Resource::VertexShader::Get(gfx, shaderCodeVS);
		AddBind(Resource::InputLayout::Get(gfx, vertexLayout, vertexShader));
		AddBind(std::move(vertexShader));

		// Material elements
		Data::CBuffer::DynamicCBuffer cbuffer(std::move(cbufferLayout));
		if (material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(static_cast<ColorF3&>(cbuffer["specularColor"]))) != aiReturn_SUCCESS)
			cbuffer["specularColor"] = ColorF3(1.0f, 1.0f, 1.0f);
		if (material.Get(AI_MATKEY_SHININESS_STRENGTH, static_cast<float&>(cbuffer["specularIntensity"])) != aiReturn_SUCCESS)
			cbuffer["specularIntensity"] = 0.9f;
		float specularPower;
		if (material.Get(AI_MATKEY_SHININESS, specularPower) == aiReturn_SUCCESS)
		{
			if (specularPower > 1.0f)
				specularPower = static_cast<float>(log(static_cast<double>(specularPower)) / log(8192.0));
			cbuffer["specularPower"] = specularPower;
		}
		else
			cbuffer["specularPower"] = 0.409f;
		if (cbuffer["materialColor"].Exists())
			if (material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor4D&>(static_cast<ColorF4&>(cbuffer["materialColor"]))) != aiReturn_SUCCESS)
				cbuffer["materialColor"] = ColorF4(0.0f, 0.8f, 1.0f);
		if (cbuffer["parallaxScale"].Exists())
			if (material.Get(AI_MATKEY_BUMPSCALING, static_cast<float&>(cbuffer["parallaxScale"])) != aiReturn_SUCCESS)
				cbuffer["parallaxScale"] = 0.1f;
		if (specularMap != nullptr)
			cbuffer["useSpecularPowerAlpha"] = specularMap->HasAlpha();
		// Maybe path needed too, TODO: Check this
		pixelBuffer = Resource::ConstBufferExPixelCache::Get(gfx, material.GetName().C_Str(), std::move(cbuffer));
	}

	void Material::SetDepthOnly(Graphics& gfx)
	{
		depthOnlyInputLayout = Resource::InputLayout::Get(gfx, vertexLayout, Resource::VertexShader::Get(gfx, "SolidVS"));
	}

	void Material::Bind(Graphics& gfx, RenderChannel mode) const
	{
		if (mode & RenderChannel::Main)
		{
			IVisual::Bind(gfx);
			pixelBuffer->Bind(gfx);
			if (diffuseTexture != nullptr)
				diffuseTexture->Bind(gfx);
			if (normalMap != nullptr)
				normalMap->Bind(gfx);
			if (parallaxMap != nullptr)
				parallaxMap->Bind(gfx);
			if (specularMap != nullptr)
				specularMap->Bind(gfx);
		}
		else if (mode & RenderChannel::Depth)
		{
			transformBuffer->Bind(gfx);
			depthOnlyInputLayout->Bind(gfx);
		}
	}
}