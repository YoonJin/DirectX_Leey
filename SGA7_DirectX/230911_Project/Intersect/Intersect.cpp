#include "pch.h"
#include "Intersect.h"
#include "Scene/Rendering/MyRect.h"

bool Intersect::IsIntersect(MyRect* lhs, MyRect* rhs)
{
	Vec3 lhs_min  = lhs->GetPosition() - (lhs->GetScale() * 0.5f);
	Vec3 lhs_max = lhs->GetPosition() + (lhs->GetScale() * 0.5f);

	Vec3 rhs_min  = rhs->GetPosition() - (rhs->GetScale() * 0.5f);
	Vec3 rhs_max  = rhs->GetPosition() + (rhs->GetScale() * 0.5f);

	if ( lhs_max.x < rhs_min.x || lhs_min.x > rhs_max.x ||
		 lhs_max.y < rhs_min.y || lhs_min.y > rhs_max.y )
	{
		// 안 박았을 때
		return false;
	}
	else if (lhs_min.x < rhs_min.x || lhs_max.x > rhs_max.x ||
		     lhs_min.y < rhs_min.y || lhs_max.y > rhs_max.y)
	{
		// 박았을 때
		return true;
	}

	return false;
}