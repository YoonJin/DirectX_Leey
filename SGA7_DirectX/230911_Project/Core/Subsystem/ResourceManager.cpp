#include "pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(Context*  context) : IObserver(context)
{
	auto root_directory = GetAssetDirectory();

	RegisterDirectory(AssetType::Animation, root_directory + "Animation/");
	RegisterDirectory(AssetType::Shader, root_directory + "Shader/");
	RegisterDirectory(AssetType::Texture, root_directory + "Texture/");
}

bool ResourceManager::Initialize()
{
	return true;
}

void ResourceManager::Update()
{

}

void ResourceManager::ReceivedNotify()
{
}

bool ResourceManager::HasResource(const std::string& resource_name, const ResourceType& resource_type)
{
	// 해당 유형의 리소스 그룹을 순회하면서 리소스 이름을 비교하여 확인
	for (const auto& resource : resource_groups[resource_type])
	{
		if (resource->GetResourceName() == resource_name)
			return true;
	}
	return false;
}

auto ResourceManager::GetAssetDirectory(const AssetType& type) -> const std::string
{
	return asset_directories[type];
}

void ResourceManager::RegisterDirectory(const AssetType& type, const std::string& directory)
{
	asset_directories[type] = directory;
}
