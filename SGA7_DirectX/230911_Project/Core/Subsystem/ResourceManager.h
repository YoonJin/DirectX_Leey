#pragma once
#include "IObserver.h"
#include "Resource/IResource.h"
#include <filesystem>


enum class AssetType : uint
{
	Texture,		// �ؽ�ó ���ҽ� ����
	Shader,		// ���̴� ���ҽ� ����
	Animation	// �ִϸ��̼� ���ҽ� ����
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
		���ҽ� ���� �޼ҵ�
	*/
	template <typename T>
	auto Load(const std::string& path) ->const std::shared_ptr<T>;

	// �̸��� ������� ���ҽ��� �������� ���ø� �޼ҵ�
	template <typename T>
	auto GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>;

	// ��θ� ������� ���ҽ��� �������� ���ø� �޼ҵ�
	template <typename T>
	auto GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>;

	// ���ҽ��� ����ϴ� ���ø� �޼ҵ�
	template <typename T>
	auto RegisterResource(const std::shared_ptr<T>& resource); 

	// �־��� �̸��� ������ ���ҽ��� �ִ��� Ȯ���ϴ� �޼ҵ�
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	/*
		���丮 ���� �޼ҵ�
	*/
	// �⺻ ���� ���丮�� ��ȯ�ϴ� �޼ҵ�
	auto GetAssetDirectory() const -> const std::string { return "Assets/"; }

	// Ư�� AssetType�� ���� ���丮 ��θ� ��ȯ�ϴ� �޼ҵ�
	auto GetAssetDirectory(const AssetType& type) -> const std::string;

private:
	// ���ҽ� ���丮�� ����ϴ� �޼ҵ�
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	// ���ҽ� �׷��� �����ϱ� ���� ��
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	std::mutex resource_mutex;

	// ���� ���丮 ������ ���� ��
	std::map<AssetType, std::string> asset_directories;
};

template<typename T>
inline auto ResourceManager::Load(const std::string& path) -> const std::shared_ptr<T>
{
	// T�� IResource�� �����ϴ��� Ȯ��
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// ������ �����ϴ��� Ȯ��
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// ���� ��ο��� ���� �̸��� ���ҽ� �̸� ����
	auto last_index = path.find_last_of('\\/');
	auto file_name = path.substr(last_index + 1, path.length());
	last_index = file_name.find_last_of('.');
	auto resource_name = file_name.substr(0, last_index);

	// ���� �̸��� ������ ���ҽ��� �̹� �ִ��� Ȯ���ϰ� ������ ��ȯ
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))
		return GetResourceFromName<T>(resource_name);

	// �� ���ҽ� ���� �� �ʱ�ȭ
	auto resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	// ���Ͽ��� ���ҽ� �ε�
	if (!resource->LoadFromFile(path))
	{
		assert(false);
		return nullptr;
	}

	// ���ҽ� ���
	RegisterResource<T>(resource);

	return resource;
}

template<typename T>
inline auto ResourceManager::GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>
{
	// T�� IResource�� �����ϴ��� Ȯ��
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// �ش� ������ ���ҽ� �׷쿡�� ��η� ���ҽ� �˻�
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
