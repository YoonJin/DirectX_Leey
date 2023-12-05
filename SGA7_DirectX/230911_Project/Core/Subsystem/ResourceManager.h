#pragma once
#include "IObserver.h"
#include "Resource/IResource.h"
#include <filesystem>


enum class AssetType : uint
{
	Texture,		// 텍스처 리소스 유형
	Shader,		// 셰이더 리소스 유형
	Animation	// 애니메이션 리소스 유형
};

class ResourceManager final : public  IObserver
{
public:
	ResourceManager(class Context*  context);
	~ResourceManager() = default;

	bool Initialize() override;
	void Update() override;
	void ReceivedNotify() override;

	/*
		리소스 관련 메소드
	*/
	template <typename T>
	auto Load(const std::string& path) ->const std::shared_ptr<T>;

	// 이름을 기반으로 리소스를 가져오는 템플릿 메소드
	template <typename T>
	auto GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>;

	// 경로를 기반으로 리소스를 가져오는 템플릿 메소드
	template <typename T>
	auto GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>;

	// 리소스를 등록하는 템플릿 메소드
	template <typename T>
	auto RegisterResource(const std::shared_ptr<T>& resource); 

	// 주어진 이름과 유형의 리소스가 있는지 확인하는 메소드
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	/*
		디렉토리 관련 메소드
	*/
	// 기본 에셋 디렉토리를 반환하는 메소드
	auto GetAssetDirectory() const -> const std::string { return "Assets/"; }

	// 특정 AssetType에 대한 디렉토리 경로를 반환하는 메소드
	auto GetAssetDirectory(const AssetType& type) -> const std::string;

private:
	// 리소스 디렉토리를 등록하는 메소드
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	// 리소스 그룹을 관리하기 위한 맵
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	std::mutex resource_mutex;

	// 에셋 디렉토리 관리를 위한 맵
	std::map<AssetType, std::string> asset_directories;
};

template<typename T>
inline auto ResourceManager::Load(const std::string& path) -> const std::shared_ptr<T>
{
	// T가 IResource를 구현하는지 확인
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// 파일이 존재하는지 확인
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// 파일 경로에서 파일 이름과 리소스 이름 추출
	auto last_index = path.find_last_of('\\/');
	auto file_name = path.substr(last_index + 1, path.length());
	last_index = file_name.find_last_of('.');
	auto resource_name = file_name.substr(0, last_index);

	// 같은 이름과 유형의 리소스가 이미 있는지 확인하고 있으면 반환
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))
		return GetResourceFromName<T>(resource_name);

	// 새 리소스 생성 및 초기화
	auto resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	// 파일에서 리소스 로드
	if (!resource->LoadFromFile(path))
	{
		assert(false);
		return nullptr;
	}

	// 리소스 등록
	RegisterResource<T>(resource);

	return resource;
}

template<typename T>
inline auto ResourceManager::GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>
{
	// T가 IResource를 구현하는지 확인
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// 해당 유형의 리소스 그룹에서 경로로 리소스 검색
	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourceName() == name)
			return std::static_pointer_cast<T>(resource);
	}

	return nullptr;
}

template<typename T>
inline auto ResourceManager::GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource"); 

	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourcePath() == path)
			return std::static_pointer_cast<T>(resource);
	}

	return nullptr;
}

template<typename T>
inline auto ResourceManager::RegisterResource(const std::shared_ptr<T>& resource)
{
	static_assert(std::is_base_of<IResource, T>::value, "provided type does not implement IResource");

	if (!resource)
	{
		assert(false);
		return;
	}

	std::lock_guard<std::mutex> guard(resource_mutex);

	resource_groups[resource->GetResourceType()].push_back(resource);
}
