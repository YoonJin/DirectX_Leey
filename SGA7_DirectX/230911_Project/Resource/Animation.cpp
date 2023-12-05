#include "pch.h"
#include "Animation.h"

Animation::Animation(Context* const context) : IResource(context, ResourceType::Animation)
{
}

Animation::~Animation()
{
	keyframes.clear();
}

bool Animation::SaveToFile(const std::string& path)
{
	//Xml::XMLDocument doc;

	//Xml::XMLDeclaration* decl = doc.NewDeclaration();
	//doc.LinkEndChild(decl);
	//
	//Xml::XMLElement* root = doc.NewElement("Animation");
	//doc.LinkEndChild(root);

	//root->SetAttribute("Name", resource_name.c_str());
	//root->SetAttribute("Type", static_cast<uint>(repeat_type));
	//root->SetAttribute("TexturePath", resource_path.c_str());
	//root->SetAttribute("TextureSizeX", sprite_texture_size.x);
	//root->SetAttribute("TextureSizeY", sprite_texture_size.y);

	//for (const auto& keyframe : keyframes)
	//{
	//	Xml::XMLElement* keyframe_element = doc.NewElement("Keyframe");
	//	root->LinkEndChild(keyframe_element);

	//	keyframe_element->SetAttribute("OffsetX", keyframe.offset.x);
	//	keyframe_element->SetAttribute("OffsetY", keyframe.offset.y);
	//	keyframe_element->SetAttribute("SizeX", keyframe.size.x);
	//	keyframe_element->SetAttribute("SizeY", keyframe.size.y);
	//	keyframe_element->SetAttribute("Time", keyframe.time);
	//}

	//return Xml::XMLError::XML_SUCCESS == doc.SaveFile(path.c_str());
	return true;
}

bool Animation::LoadFromFile(const std::string& path)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(path.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();
	resource_name = root->Attribute("Name");
	repeat_type = static_cast<RepeatType>(root->UnsignedAttribute("Type"));
	resource_path = root->Attribute("TexturePath");
	sprite_texture_size.x = root->FloatAttribute("TextureSizeX");
	sprite_texture_size.y = root->FloatAttribute("TextureSizeY");

	SetSpriteTexture(resource_path);

	Xml::XMLElement* direction_element = root->FirstChildElement();

	for (; direction_element != nullptr; direction_element = direction_element->NextSiblingElement())
	{
		int index = direction_element->IntAttribute("Name");
		PlayerDirection dir = static_cast<PlayerDirection>(index);

		Xml::XMLElement* keyframe_element = direction_element->FirstChildElement();
		for (; keyframe_element != nullptr; keyframe_element = keyframe_element->NextSiblingElement())
		{
			Vec2 offset;
			offset.x = keyframe_element->FloatAttribute("OffsetX");
			offset.y = keyframe_element->FloatAttribute("OffsetY");

			Vec2 size;
			size.x = keyframe_element->FloatAttribute("SizeX");
			size.y = keyframe_element->FloatAttribute("SizeY");

			float time = keyframe_element->FloatAttribute("Time");

			AddKeyframe(dir, offset, size, time);
		}
	}


	return true;
}

void Animation::SetSpriteTexture(const std::string& path)
{
	// wstring º¯È¯
	std::wstring texture_path;
	texture_path.append(path.begin(), path.end());
	
	sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	sprite_texture->Create(texture_path);

}

auto Animation::GetKeyframeFromIndex(PlayerDirection eDirection, const uint& index) -> const Keyframe* const
{
	assert(index < keyframes[eDirection].size());

	return  &keyframes[eDirection][index];
}

void Animation::AddKeyframe(PlayerDirection eDirection, const Keyframe& keyframe)
{
	keyframes[eDirection].push_back(keyframe);
}

void Animation::AddKeyframe(PlayerDirection eDirection, const Vec2& offset, const Vec2& size, const double& time)
{
	keyframes[eDirection].push_back(Keyframe(offset, size, time));
}
