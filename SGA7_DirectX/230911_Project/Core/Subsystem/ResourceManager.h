#pragma once
#include "ISubsystem.h"
#include "Resource/IResource.h"
#include <filesystem>

/*
* [filesystem]
* ���� �ý��� ���̺귯���� ���� ������ �� ������� ����ϴ� ���� ����� ���̺귯�� (<fstream>) ���� �ٸ���.
  <fstream> �� ���, ���� �ϳ��� �־����� �ش� ������ �����͸� �о�� ������ ������,
  �� �ܿ� ���Ͽ� ���� ���� (���� �̸�, ��ġ, ���) �� ���� �����͸� ������ �� �� ����.
  �ݸ鿡 ���� �ý��� ���̺귯���� ���, ���Ͽ� ���� ���� (���� ��Ÿ����Ÿ)�� ���� ������ �����ִ� ������ �����ϸ�,
  ���� ��ü�� �д� ���� �������� �ʴ´�.

   ���� ���ؼ� �ϵ� ��ũ ��򰡿� �ִ� a.txt ��� ������ ã�� �ʹٸ� filesystem ���̺귯���� ����ϰ� �ǰ�,
   �ش� ������ ã�� ���Ŀ� a.txt �� �а� �ʹٸ� fstream ���̺귯���� ����ϸ� �ȴ�.

   �Ӹ� �ƴ϶� ���� �ý��� ���̺귯���� ���ؼ� ���ϴ� ��ο� ������ �߰��Ѵٴ���, ������ ���� �Ѵٴ���,
   �ƴϸ� ������ ���� - ���� �� ������ ���� �ð��̷�����, �����̶������ ���� �͵��� ���� ������ ����� �� �ִ�.
*/

enum class AssetType : uint
{
	Texture,	// �ؽ�ó ���ҽ� ����
	Shader,     // ���̴� ���ҽ� ����
	Animation   // �ִϸ��̼� ���ҽ� ����
};

class ResourceManager final : public ISubsystem
{
public:
	ResourceManager(class Context* const context);
	~ResourceManager() = default;

	bool Initialize() override;
	void Update()     override;

	/*
		���ҽ� ���� �޼���
	*/

	template <typename T>
	auto Load(const std::string& path) -> const std::shared_ptr<T>;

	// �̸��� ������� ���ҽ��� �������� ���ø� �޼���
	template <typename T>
	auto GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>;

	// ��θ� ������� ���ҽ��� �������� ���ø� �޼���
	template <typename T>
	auto GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>;

	// ���ҽ��� ����ϴ� ���ø� �޼���
	template <typename T>
	auto RegisterResource(const std::shared_ptr<T>& resource);

	// �־��� �̸��� ������ ���ҽ��� �ִ��� Ȯ���ϴ� �޼���
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	/*
		Directory
	*/
	// �⺻ ���� ���丮�� ��ȯ�ϴ� �޼���
	auto GetAssetDirectory() const -> const std::string { return "Assets/"; }
	// Ư�� AssetType�� ���� ���丮 ��θ� ��ȯ�ϴ� �޼���
	auto GetAssetDirectory(const AssetType& type) -> const std::string;

private:
	// ���ҽ� ���丮�� ����ϴ� �޼���
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	// ���ҽ� �׷��� �����ϱ� ���� ��
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	// ���ҽ��� ���� ���ؽ�
	/*
	  std::mutex�� C++ ǥ�� ���̺귯������ �����ϴ� ����ȭ ����� �����ϴ� Ŭ���� �� �ϳ��̴�.
	  "mutex"�� "mutual exclusion"�� ���ڷ�, ���� �����尡 ������ ������ �Ǵ� �ڿ��� ���ÿ� �������� ���ϵ��� ��ȣ�ϴ� �� ���ȴ�.
	  std::mutex�� ������ ���� ���� ����(race condition)�� �����ϰ� ����ȭ�� �����ϱ� ���� �߿��� ���� �� �ϳ��̴�.
	*/
	std::mutex resource_mutex;

	// ���� ���丮 ������ ���� ��
	std::map<AssetType, std::string> asset_directories;
};

template<typename T>
inline auto ResourceManager::Load(const std::string& path) -> const std::shared_ptr<T>
{
	// T�� IReource�� �����ϴ��� Ȯ��
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// ������ �����ϴ��� Ȯ��
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// ���� ��ο��� ���� �̸��� ���ҽ� �̸� ����
	auto last_index    = path.find_last_of("\\/");
	auto file_name     = path.substr(last_index + 1, path.length());
	last_index         = file_name.find_last_of('.');
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
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	if (!resource)
	{
		assert(false);
		return;
	}

	// ���ҽ� ����� ������κ��� ��ȣ�ϱ� ���� ���ؽ� ���
	//���ؽ��� ������ ����� �ڵ����� ���ؽ��� �����ȴ�.
	std::lock_guard<std::mutex> guard(resource_mutex);

	resource_groups[resource->GetResourceType()].push_back(resource);
}
