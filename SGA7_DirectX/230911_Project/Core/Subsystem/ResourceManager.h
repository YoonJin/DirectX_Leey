#pragma once
#include "ISubsystem.h"
#include "Resource/IResource.h"
#include <filesystem>

/*
* [filesystem]
* 파일 시스템 라이브러리는 파일 데이터 의 입출력을 담당하는 파일 입출력 라이브러리 (<fstream>) 과는 다르다.
  <fstream> 의 경우, 파일 하나가 주어지면 해당 파일의 데이터를 읽어내는 역할을 하지만,
  그 외에 파일에 관한 정보 (파일 이름, 위치, 등등) 에 관한 데이터를 수정할 수 는 없다.
  반면에 파일 시스템 라이브러리의 경우, 파일에 관한 정보 (파일 메타데이타)에 대한 접근을 도와주는 역할을 수행하며,
  파일 자체를 읽는 일은 수행하지 않는다.

   쉽게 말해서 하드 디스크 어딘가에 있는 a.txt 라는 파일을 찾고 싶다면 filesystem 라이브러리를 사용하게 되고,
   해당 파일을 찾은 이후에 a.txt 를 읽고 싶다면 fstream 라이브러리를 사용하면 된다.

   뿐만 아니라 파일 시스템 라이브러리를 통해서 원하는 경로에 폴더를 추가한다던지, 파일을 삭제 한다던지,
   아니면 파일의 정보 - 예를 들어서 파일의 생성 시간이러던지, 권한이라던지와 같은 것들을 보는 데에도 사용할 수 있다.
*/

enum class AssetType : uint
{
	Texture,	// 텍스처 리소스 유형
	Shader,     // 셰이더 리소스 유형
	Animation   // 애니메이션 리소스 유형
};

class ResourceManager final : public ISubsystem
{
public:
	ResourceManager(class Context* const context);
	~ResourceManager() = default;

	bool Initialize() override;
	void Update()     override;

	/*
		리소스 관련 메서드
	*/

	template <typename T>
	auto Load(const std::string& path) -> const std::shared_ptr<T>;

	// 이름을 기반으로 리소스를 가져오는 템플릿 메서드
	template <typename T>
	auto GetResourceFromName(const std::string& name) -> const std::shared_ptr<T>;

	// 경로를 기반으로 리소스를 가져오는 템플릿 메서드
	template <typename T>
	auto GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>;

	// 리소스를 등록하는 템플릿 메서드
	template <typename T>
	auto RegisterResource(const std::shared_ptr<T>& resource);

	// 주어진 이름과 유형의 리소스가 있는지 확인하는 메서드
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	/*
		Directory
	*/
	// 기본 에셋 디렉토리를 반환하는 메서드
	auto GetAssetDirectory() const -> const std::string { return "Assets/"; }
	// 특정 AssetType에 대한 디렉토리 경로를 반환하는 메서드
	auto GetAssetDirectory(const AssetType& type) -> const std::string;

private:
	// 리소스 디렉토리를 등록하는 메서드
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	// 리소스 그룹을 관리하기 위한 맵
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	// 리소스에 대한 뮤텍스
	/*
	  std::mutex는 C++ 표준 라이브러리에서 제공하는 동기화 기능을 제공하는 클래스 중 하나이다.
	  "mutex"는 "mutual exclusion"의 약자로, 여러 스레드가 동일한 데이터 또는 자원에 동시에 접근하지 못하도록 보호하는 데 사용된다.
	  std::mutex는 스레드 간의 경쟁 조건(race condition)을 방지하고 동기화를 제어하기 위한 중요한 도구 중 하나이다.
	*/
	std::mutex resource_mutex;

	// 에셋 디렉토리 관리를 위한 맵
	std::map<AssetType, std::string> asset_directories;
};

template<typename T>
inline auto ResourceManager::Load(const std::string& path) -> const std::shared_ptr<T>
{
	// T가 IReource를 구현하는지 확인
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// 파일이 존재하는지 확인
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// 파일 경로에서 파일 이름과 리소스 이름 추출
	auto last_index    = path.find_last_of("\\/");
	auto file_name     = path.substr(last_index + 1, path.length());
	last_index         = file_name.find_last_of('.');
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
	return std::shared_ptr<T>();
}

template<typename T>
inline auto ResourceManager::GetResourceFromPath(const std::string& path) -> const std::shared_ptr<T>
{
	return std::shared_ptr<T>();
}

template<typename T>
inline auto ResourceManager::RegisterResource(const std::shared_ptr<T>& resource)
{
}
