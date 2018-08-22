#pragma once
#include "stdafx.h"

class LineOb;
class SphereOb;
class AABBOb;
class OBBOb;
class FrustumOb;


class CollisionFuncStatic
{
public:
	// return true if collision

	//static bool staticCheck_POINT_LINE(const glm::vec3& point, const LineOb& lineOb);
	//static bool staticCheck_Dist_POINT_LINE(const glm::vec3& point, const LineOb& lineOb, float& retDist);
	//static bool staticCheck_CLOSEST_POINT_LINE(const glm::vec3& point, const LineOb& lineOb, glm::vec3& retClosestPoint);

	static bool staticCheck_POINT_SPHERE(const glm::vec3& point, const SphereOb& sphereOb);
	static bool staticCheck_Dist_POINT_SPHERE(const glm::vec3& point, const SphereOb& sphereOb, float& retDist);
	static bool staticCheck_CLOSEST_POINT_SPHERE(const glm::vec3& point, const SphereOb& sphereOb, glm::vec3& retClosestPoint);

	static bool staticCheck_POINT_AABB(const glm::vec3& point, const AABBOb& aabbOb);
	static bool staticCheck_Dist_POINT_AABB(const glm::vec3& point, const AABBOb& aabbOb, float& retDist);
	static void staticCheck_CLOSEST_POINT_AABB(const glm::vec3& point, const AABBOb& aabbOb, glm::vec3& retClosestPoint);

	static bool staticCheck_POINT_OBB(const glm::vec3& point, const OBBOb& obbOb);
	static bool staticCheck_Dist_POINT_OBB(const glm::vec3& point, const OBBOb& obbOb, float& retDist);
	static void staticCheck_CLOSEST_POINT_OBB(const glm::vec3& point, const OBBOb& obbOb, glm::vec3& retClosestPoint);


	//static bool staticCheck_LINE_LINE(const LineOb& lineOb1, const LineOb& lineOb2);
	//static bool staticCheck_Dist_LINE_LINE(const LineOb& lineOb1, const LineOb& lineOb2, float& retDist);
	//static bool staticCheck_CLOSEST_LINE_LINE(const LineOb& lineOb1, const LineOb& lineOb2, glm::vec3& pointOnLine1, glm::vec3& pointOnLine2);

	static bool staticCheck_LINE_SPHERE(const LineOb& lineOb, const SphereOb& sphereOb);
	static bool staticCheck_LINE_SPHERE(const LineOb& lineOb, const SphereOb& sphereOb, float& retDist);	// return dist if collision occur
	//static bool staticCheck_CLOSEST_LINE_SPHERE(const LineOb& lineOb, const SphereOb& sphereOb, glm::vec3& pointOnLine, glm::vec3& pointOnSphere);

	static bool staticCheck_LINE_AABB(const LineOb& lineOb, const AABBOb& aabbOb);
	static bool staticCheck_LINE_AABB(const LineOb& lineOb, const AABBOb& aabbOb, float& retDist);	// return dist if collision occur
	//static bool staticCheck_CLOSEST_LINE_AABB(const LineOb& lineOb, const AABBOb& aabbOb, glm::vec3& pointOnLine, glm::vec3& pointOnABB);

	static bool staticCheck_LINE_OBB(const LineOb& lineOb, const OBBOb& obbOb);
	static bool staticCheck_LINE_OBB(const LineOb& lineOb, const OBBOb& obbOb, float& retDist);	// return dist if collision occur
	//static bool staticCheck_CLOSEST_LINE_OBB(const LineOb& lineOb, const OBBOb& obbOb, glm::vec3& pointOnLine, glm::vec3& pointOnOBB);

	static bool staticCheck_SPHERE_SPHERE(const SphereOb& sphereOb1, const SphereOb& sphereOb2);
	static bool staticCheck_SPHERE_AABB_OVERLAP(const SphereOb& sphereOb, const AABBOb& aabbOb);
	static bool staticCheck_SPHERE_OBB(const SphereOb& sphereOb, const OBBOb& obbOb);

	static bool staticCheck_AABB_LINE_IN(const AABBOb& aabbObBig, const LineOb& lineOb);
	static bool staticCheck_AABB_LINE_OVERLAP(const AABBOb& aabbObBig, const LineOb& lineOb);
	static bool staticCheck_AABB_SPHERE_IN(const AABBOb& aabbObBig, const SphereOb& sphereOb);
	//static bool staticCheck_AABB_SPHERE_INOUTOVERLAP(const AABBOb& aabbObBig, const SphereOb& sphereOb);
	static bool staticCheck_AABB_AABB_IN(const AABBOb& aabbObBig, const AABBOb& aabbObSmall);
	static bool staticCheck_AABB_AABB_OVERLAP(const AABBOb& aabbOb1, const AABBOb& aabbOb2);
	static int	staticCheck_AABB_AABB_INOUTOVERLAP(const AABBOb& aabbObBig, const AABBOb& aabbObSmall);
	static bool staticCheck_AABB_OBB_OVERLAP(const AABBOb& aabbOb, const OBBOb& obbOb);

	static int	staticCheck_FRUSTUM_AABB_INOUTOVERLAP(const FrustumOb& frustumObBig, const AABBOb& aabbObSmall);
	
	static bool staticCheck_OBB_OBB_OUT(const OBBOb& obbOb1, const OBBOb& obbOb2);

	static void CreateAAABBLAP_SPHERE(const SphereOb& sphereOb, AABBOb& retAabb);
	static void CreateAAABBLAP_LINE(const LineOb& lineOb, AABBOb& retAabb);

private:
	static bool staticCheck_BOX(const glm::mat4 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2);
	static bool staticCheck_BOX(const glm::vec3 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2);	// AABB - OBB

	/*
	*	reference
	*	https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	*	http://www.opengl-tutorial.org/kr/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
	*/
	// if collision, reutrn line dist :: line startPos ~ collision point
	static bool checkOBBOneINFLineIntersection(const OBBOb& obbOb, const LineOb& lineOb, float& retDist);
	static bool checkOBBLineIntersection(const OBBOb& obbOb, const LineOb& lineOb, float& retDist);
	static bool checkAABBOneINFLineIntersection(const AABBOb& aabbOb, const LineOb& lineOb, float& retDist);
	static bool checkAABBLineIntersection(const AABBOb& aabbOb, const LineOb& lineOb, float& retDist);
	//static bool checkINFLineIntersection(const AABBOb& aabbOb, const LineOb& lineOb, float& retDist);
	//static bool checkINFLineIntersection(const OBBOb& obbOb, const LineOb& lineOb, float& retDist);

	static bool checkSphereOneINFLineIntersection(const SphereOb& sphereOb, const LineOb& lineOb, float& retDist);
	static bool checkSphereLineIntersection(const SphereOb& sphereOb, const LineOb& lineOb, float& retDist);
};