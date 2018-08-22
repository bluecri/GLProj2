#include "CollisionFuncStatic.h"
#include "OBBOb.h"
#include "AABBOb.h"
#include "SphereOb.h"
#include "LineOb.h"
#include "FrustumOb.h"
/*
bool CollisionFuncStatic::staticCheck_LINE_LINE(const LineOb & lineOb1, const LineOb & lineOb2)
{
	return false;
}

bool CollisionFuncStatic::staticCheck_Dist_LINE_LINE(const LineOb & lineOb1, const LineOb & lineOb2, float & retDist)
{
	return false;
}

bool CollisionFuncStatic::staticCheck_CLOSEST_LINE_LINE(const LineOb & lineOb1, const LineOb & lineOb2, glm::vec3 & pointOnLine1, glm::vec3 & pointOnLine2)
{
	return false;
}
*/

bool CollisionFuncStatic::staticCheck_LINE_SPHERE(const LineOb & lineOb, const SphereOb & sphereOb)
{
	int lineType = static_cast<int>(lineOb.getType());
	float tempFloat;
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return checkSphereOneINFLineIntersection(sphereOb, lineOb, tempFloat);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return checkSphereLineIntersection(sphereOb, lineOb, tempFloat);
		break;
	}
	return false;
}


bool CollisionFuncStatic::staticCheck_LINE_SPHERE(const LineOb & lineOb, const SphereOb & sphereOb, float & retDist)
{
	int lineType = static_cast<int>(lineOb.getType());
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return checkSphereOneINFLineIntersection(sphereOb, lineOb, retDist);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return checkSphereLineIntersection(sphereOb, lineOb, retDist);
		break;
	}
	return false;
}

/*
bool CollisionFuncStatic::staticCheck_CLOSEST_LINE_SPHERE(const LineOb & lineOb, const SphereOb & sphereOb, glm::vec3 & pointOnLine, glm::vec3 & pointOnSphere)
{
	int lineType = static_cast<int>(lineOb.getType());
	switch (lineType)
	{
	case 0:
		// LINEOB_TYPE_INFINITE_ONE_DIRTECTION
		break;
	case 1:
		// LINEOB_TYPE_INFINITE_TWO_DIRECTION
		break;
	case 2:
		// LINEOB_TYPE_LEN
		break;
	}

	return false;
}
*/

bool CollisionFuncStatic::staticCheck_LINE_AABB(const LineOb & lineOb, const AABBOb & aabbOb)
{
	int lineType = static_cast<int>(lineOb.getType());
	float tempFloat;
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return checkAABBOneINFLineIntersection(aabbOb, lineOb, tempFloat);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return checkAABBLineIntersection(aabbOb, lineOb, tempFloat);
		break;
	}

	return false;
}

bool CollisionFuncStatic::staticCheck_LINE_AABB(const LineOb & lineOb, const AABBOb & aabbOb, float & retDist)
{
	int lineType = (int)(lineOb.getType());
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return checkAABBOneINFLineIntersection(aabbOb, lineOb, retDist);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return checkAABBLineIntersection(aabbOb, lineOb, retDist);
		break;
	}
	return false;
}

/*
bool CollisionFuncStatic::staticCheck_CLOSEST_LINE_AABB(const LineOb & lineOb, const AABBOb & aabbOb, glm::vec3 & pointOnLine, glm::vec3 & pointOnABB)
{
	return false;
}
*/

bool CollisionFuncStatic::staticCheck_LINE_OBB(const LineOb & lineOb, const OBBOb & obbOb)
{
	int lineType = static_cast<int>(lineOb.getType());
	float tempDist;
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return checkOBBOneINFLineIntersection(obbOb, lineOb, tempDist);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return checkOBBLineIntersection(obbOb, lineOb, tempDist);
		break;
	}
	return false;
}

bool CollisionFuncStatic::staticCheck_LINE_OBB(const LineOb & lineOb, const OBBOb & obbOb, float & retDist)
{
	int lineType = static_cast<int>(lineOb.getType());
	switch (lineType)
	{
	case 0:
		// LINEOB_TYPE_INFINITE_ONE_DIRTECTION
		return checkOBBOneINFLineIntersection(obbOb, lineOb, retDist);
		break;
	case 1:
		// LINEOB_TYPE_INFINITE_TWO_DIRECTION
		break;
	case 2:
		// LINEOB_TYPE_LEN
		return checkOBBLineIntersection(obbOb, lineOb, retDist);
		break;
	}
	return false;
}

/*
bool CollisionFuncStatic::staticCheck_CLOSEST_LINE_OBB(const LineOb & lineOb, const OBBOb & obbOb, glm::vec3 & pointOnLine, glm::vec3 & pointOnOBB)
{
	return false;
}
*/

bool CollisionFuncStatic::staticCheck_SPHERE_SPHERE(const SphereOb & sphereOb1, const SphereOb & sphereOb2)
{
	float radDist = sphereOb1.getRadius() + sphereOb2.getRadius();
	float centerDist2 = glm::length2(sphereOb1.getCenterConstRef() - sphereOb2.getCenterConstRef());

	if (centerDist2 > radDist * radDist)
		return false;

	return true;
}

bool CollisionFuncStatic::staticCheck_SPHERE_AABB_OVERLAP(const SphereOb & sphereOb, const AABBOb & aabb)
{
	float dist;
	staticCheck_Dist_POINT_AABB(sphereOb.getCenterConstRef(), aabb, dist);

	if (dist > sphereOb.getRadius())
		return false;
	return true;
}

bool CollisionFuncStatic::staticCheck_SPHERE_OBB(const SphereOb & sphereOb, const OBBOb & obbOb)
{
	float tempDIst;
	return staticCheck_Dist_POINT_OBB(sphereOb.getCenterConstRef(), obbOb, tempDIst);
}

bool CollisionFuncStatic::staticCheck_AABB_LINE_IN(const AABBOb & aabbObBig, const LineOb & lineOb)
{
	int lineType = static_cast<int>(lineOb.getType());
	bool cond1, cond2;
	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return false;
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		return false;
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		cond1 = staticCheck_POINT_AABB(lineOb.getEndPos(), aabbObBig);
		cond2 = staticCheck_POINT_AABB(lineOb.getStartPosConstRef(), aabbObBig);
		return (cond1 && cond2);
		break;
	}
	return true;
}

bool CollisionFuncStatic::staticCheck_AABB_LINE_OVERLAP(const AABBOb & aabbObBig, const LineOb & lineOb)
{
	int lineType = static_cast<int>(lineOb.getType());
	float tempFloat;

	switch (lineType)
	{
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION:
		return staticCheck_LINE_AABB(lineOb, aabbObBig, tempFloat);
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_TWO_DIRECTION:
		return false;
		break;
	case LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN:
		return staticCheck_LINE_AABB(lineOb, aabbObBig, tempFloat);
		break;
	}
	return true;
}

bool CollisionFuncStatic::staticCheck_AABB_SPHERE_IN(const AABBOb & aabbObBig, const SphereOb & sphereOb)
{
	AABBOb sphereAABBOb;
	sphereAABBOb.updateAABBObCenter(sphereOb.getCenterConstRef());
	sphereAABBOb.updateAABBObAxis(sphereOb.getRadius());

	return staticCheck_AABB_AABB_IN(aabbObBig, sphereAABBOb);
}

/*
bool CollisionFuncStatic::staticCheck_AABB_SPHERE_INOUTOVERLAP(const AABBOb & aabbObBig, const SphereOb & sphereOb)
{
	return false;
}
*/

bool CollisionFuncStatic::staticCheck_AABB_AABB_IN(const AABBOb & aabbObBig, const AABBOb & aabbObSmall)
{
	const glm::vec3& aabbBigCemter		= aabbObBig.getCenterConstRef();
	const glm::vec3& aabbBigAxis		= aabbObBig.getAxisConstRef();
	const glm::vec3& aabbSmallCemter	= aabbObSmall.getCenterConstRef();
	const glm::vec3& aabbSmallAxis		= aabbObSmall.getAxisConstRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(aabbBigCemter[i] - aabbSmallCemter[i]) > fabs(aabbBigAxis[i] - aabbSmallAxis[i]))
			return false;	// outside
	}

	for (int i = 0; i < 3; i++)
	{
		if (aabbBigAxis[i] < aabbSmallAxis[i])
			return false;
	}

	return true;
}

bool CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(const AABBOb & aabbOb1, const AABBOb & aabbOb2)
{
	const glm::vec3& aabb1Center	= aabbOb1.getCenterConstRef();
	const glm::vec3& aabb1Axis		= aabbOb1.getAxisConstRef();
	const glm::vec3& aabb2Cemter	= aabbOb2.getCenterConstRef();
	const glm::vec3& aabb2Axis		= aabbOb2.getAxisConstRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(aabb1Center[i] - aabb2Cemter[i]) > aabb1Axis[i] + aabb2Axis[i])
			return false;	// outside
	}

	return true;
}

int CollisionFuncStatic::staticCheck_AABB_AABB_INOUTOVERLAP(const AABBOb & aabbObBig, const AABBOb & aabbObSmall)
{
	const glm::vec3& aabbBigCemter = aabbObBig.getCenterConstRef();
	const glm::vec3& aabbBigAxis = aabbObBig.getAxisConstRef();
	const glm::vec3& aabbSmallCemter = aabbObSmall.getCenterConstRef();
	const glm::vec3& aabbSmallAxis = aabbObSmall.getAxisConstRef();

	glm::vec3 centerDiffAbs;
	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		centerDiffAbs[i] = fabs(aabbBigCemter[i] - aabbSmallCemter[i]);
		if (centerDiffAbs[i] > aabbBigAxis[i] + aabbSmallAxis[i])
			return -1;	// outside
	}

	for (int i = 0; i < 3; i++)
	{
		bool isOverlapCond1 = (aabbBigAxis[i] < aabbSmallAxis[i]);
		bool isOverlapCond2 = (centerDiffAbs[i] > aabbBigAxis[i] - aabbSmallAxis[i]);

		if (isOverlapCond1 || isOverlapCond2)
			return 0;	// Overlap
	}

	return 1;	// in
}


bool CollisionFuncStatic::staticCheck_AABB_OBB_OVERLAP(const AABBOb & aabbOb, const OBBOb & obbOb)
{
	return staticCheck_BOX(aabbOb.getCenterConstRef(), obbOb.getMatConstRef(), aabbOb.getAxisConstRef(), obbOb.getAxisConstRef());
}

// inside(octree totally in frustum), outside, overlap( + frustum totally in octree)
int CollisionFuncStatic::staticCheck_FRUSTUM_AABB_INOUTOVERLAP(const FrustumOb & frustumObBig, const AABBOb & aabbObSmall)
{
	const glm::vec3& aabbSmallCemter = aabbObSmall.getCenterConstRef();
	const glm::vec3& aabbSmallAxis = aabbObSmall.getAxisConstRef();
	const std::vector<glm::vec4>& frustum = frustumObBig.getFrustumConstRef();

	glm::vec3 octreeMinPoint, octreeMaxPoint;
	glm::vec3 minPoint, maxPoint;

	int ret = 1;	// inside

	octreeMinPoint = aabbSmallCemter - aabbSmallAxis;
	octreeMaxPoint = aabbSmallCemter + aabbSmallAxis;

	for (int i = 0; i < 6; i++)
	{
		//frustum[i] = -frustum[i];

		if (frustum[i].x > 0.0)
		{
			minPoint.x = octreeMinPoint.x;
			maxPoint.x = octreeMaxPoint.x;
		}
		else
		{
			minPoint.x = octreeMaxPoint.x;
			maxPoint.x = octreeMinPoint.x;
		}

		if (frustum[i].y > 0.0)
		{
			minPoint.y = octreeMinPoint.y;
			maxPoint.y = octreeMaxPoint.y;
		}
		else
		{
			minPoint.y = octreeMaxPoint.y;
			maxPoint.y = octreeMinPoint.y;
		}

		if (frustum[i].z > 0.0)
		{
			minPoint.z = octreeMinPoint.z;
			maxPoint.z = octreeMaxPoint.z;
		}
		else
		{
			minPoint.z = octreeMaxPoint.z;
			maxPoint.z = octreeMinPoint.z;
		}

		// get plane <> point dist
		float dotMinVal = 0.0f;
		float dotMaxVal = 0.0f;
		for (int k = 0; k < 3; k++)
		{
			dotMinVal += frustum[i][k] * minPoint[k];
			dotMaxVal += frustum[i][k] * maxPoint[k];
		}
		dotMinVal += frustum[i][3];
		dotMaxVal += frustum[i][3];

		if (dotMinVal > 0.0f)
			return -1;	// outside
		if (dotMaxVal >= 0.0f)
			ret = 0;	// intersect
	}

	return ret;
}

bool CollisionFuncStatic::staticCheck_OBB_OBB_OUT(const OBBOb & obbOb1, const OBBOb & obbOb2)
{
	return staticCheck_BOX(obbOb1.getMatConstRef(), obbOb2.getMatConstRef(), obbOb1.getAxisConstRef(), obbOb2.getAxisConstRef());
}

void CollisionFuncStatic::CreateAAABBLAP_SPHERE(const SphereOb & sphereOb, AABBOb & retAabb)
{
	retAabb.updateAABBObCenter(sphereOb.getCenter());
	retAabb.updateAABBObAxis(sphereOb.getRadius());
}

void CollisionFuncStatic::CreateAAABBLAP_LINE(const LineOb & lineOb, AABBOb & retAabb)
{
	glm::vec3 lineStartToEndMiddleVec = (lineOb.getEndPos() - lineOb.getStartPosConstRef()) / 2.0f;
	retAabb.updateAABBObCenter(lineStartToEndMiddleVec);

	for (int i = 0; i < 3; i++)
		lineStartToEndMiddleVec[i] = fabsf(lineStartToEndMiddleVec[i]);
	retAabb.updateAABBObAxis(lineStartToEndMiddleVec);
}

void CollisionFuncStatic::staticCheck_CLOSEST_POINT_AABB(const glm::vec3 & point, const AABBOb & aabbOb, glm::vec3 & retClosestPoint)
{
	const glm::vec3& aabbCenter = aabbOb.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbOb.getAxisConstRef();
	retClosestPoint = aabbCenter;

	glm::vec3 oneDist = aabbCenter - aabbAxis;
	glm::vec3 twoDist = aabbCenter + aabbAxis;

	for (int i = 0; i < 3; i++)
	{
		if (point[i] < oneDist[i])
			retClosestPoint[i] = oneDist[i];
		if (point[i] > twoDist[i])
			retClosestPoint[i] = twoDist[i];
	}

	return;
}

bool CollisionFuncStatic::staticCheck_POINT_OBB(const glm::vec3 & point, const OBBOb & obbOb)
{
	const glm::mat4& obbObMat = obbOb.getMatConstRef();
	const glm::vec3& obbAxis = obbOb.getAxisConstRef();
	glm::vec3 obbCenterPointVec;	// vector from obbCenter to Point
	for (int i = 0; i < 3; i++)
		obbCenterPointVec[i] = point[i] - obbObMat[3][i];

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 rotateAxis = glm::vec3(obbObMat[i]);
		float rotatedDiff = glm::dot(rotateAxis, obbCenterPointVec);

		if (!(-obbAxis[i] < rotatedDiff && rotatedDiff < obbAxis[i]))
		{
			return false;
		}
	}
	
	return true;
}

bool CollisionFuncStatic::staticCheck_Dist_POINT_OBB(const glm::vec3 & point, const OBBOb & obbOb, float & retDist)
{
	bool isCollide = true;
	float retDist = 0.0f;
	const glm::mat4& obbObMat = obbOb.getMatConstRef();
	glm::vec3 obbCenterToPointVec;	// vector from obbCenter to Point

	for (int i = 0; i < 3; i++)
		obbCenterToPointVec[i] = point[i] - obbObMat[3][i];

	const glm::vec3& obbAxis = obbOb.getAxisConstRef();

	for (int i = 0; i < 3; i++)
	{
		float oneDist = -obbAxis[i];
		float twoDist = obbAxis[i];
		float dotDIst = glm::dot(obbCenterToPointVec, glm::vec3(obbObMat[i]));

		if (dotDIst < oneDist)
		{
			isCollide = false;
			retDist += (dotDIst - oneDist) * (dotDIst - oneDist);
		}

		if (dotDIst > twoDist)
		{
			isCollide = false;
			retDist += (dotDIst - twoDist) * (dotDIst - twoDist);
		}
	}

	retDist = std::sqrtf(retDist);
	return isCollide;
}


void CollisionFuncStatic::staticCheck_CLOSEST_POINT_OBB(const glm::vec3 & point, const OBBOb & obbOb, glm::vec3 & retClosestPoint)
{
	const glm::mat4& obbObMat = obbOb.getMatConstRef();
	glm::vec3 obbCenterToPointVec;

	for (int i = 0; i < 3; i++)
		obbCenterToPointVec[i] = point[i] - obbObMat[3][i];

	retClosestPoint = glm::vec3(obbObMat[3]);

	const glm::vec3& obbAxis = obbOb.getAxisConstRef();

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 unitVec = glm::vec3(obbObMat[i]);
		float oneDist = -obbAxis[i];
		float twoDist = obbAxis[i];
		float dotDIst = glm::dot(obbCenterToPointVec, unitVec);

		if (dotDIst < oneDist)
			retClosestPoint += oneDist * unitVec;
		if (dotDIst > twoDist)
			retClosestPoint += twoDist * unitVec;
	}
	
	return;

}

bool CollisionFuncStatic::staticCheck_BOX(const glm::mat4 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2)
{
	double c[3][3];	//ob1 3 axis vs otherOBB 3 axis consine value
	double absC[3][3];	//abs of c[][]
	double d[3];
	double r, r0, r1;	//r = 한 축을 기준으로 obBox 두 중심점의 거리
	const float cutoff = 0.9999f;
	bool existParallelPair = false;	//paralell 한가?

	glm::vec3 axisVec[3];
	glm::vec3 otheraxisVec[3];

	for (int i = 0; i < 3; i++)
	{
		axisVec[i] = wolrd1[i];
		otheraxisVec[i] = wolrd2[i];
	}
	glm::vec3 centerDiff = wolrd1[3] - wolrd2[3];

	//case COLLISION_TYPE::COLLISION_AABB:
	//case COLLISION_TYPE::COLLISION_OBB:

	//ob1 1 face vs otherOBB 3 faces
	for (int i = 0; i < 3; i++) {
		c[0][i] = glm::dot(axisVec[0], otheraxisVec[i]);
		absC[0][i] = glm::abs(c[0][i]);
		if (absC[0][i] > cutoff) {
			existParallelPair = true;
		}
	}

	d[0] = glm::dot(centerDiff, axisVec[0]);
	r = abs(d[0]);
	r0 = axisLen1[0];
	r1 = axisLen2[0] * absC[0][0] + axisLen2[1] * absC[0][1] + axisLen2[2] * absC[0][2];
	if (r > r0 + r1) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		c[1][i] = glm::dot(axisVec[1], otheraxisVec[i]);
		absC[1][i] = glm::abs(c[1][i]);
		if (absC[1][i] > cutoff) {
			existParallelPair = true;
		}
	}
	d[1] = glm::dot(centerDiff, axisVec[1]);
	r = abs(d[1]);
	r0 = axisLen1[1];
	r1 = axisLen2[1] * absC[1][0] + axisLen2[1] * absC[1][1] + axisLen2[2] * absC[1][2];
	if (r > r0 + r1) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		c[2][i] = glm::dot(axisVec[2], otheraxisVec[i]);
		absC[2][i] = glm::abs(c[2][i]);
		if (absC[2][i] > cutoff) {
			existParallelPair = true;
		}
	}
	d[2] = glm::dot(centerDiff, axisVec[2]);
	r = abs(d[2]);
	r0 = axisLen1[2];
	r1 = axisLen2[0] * absC[2][0] + axisLen2[1] * absC[2][1] + axisLen2[2] * absC[2][2];
	if (r > r0 + r1) {
		return false;
	}

	//ob1 3 face vs otherOBB 1 faces
	r = abs(glm::dot(centerDiff, otheraxisVec[0]));
	r0 = axisLen2[0];
	r1 = axisLen1[0] * absC[0][0] + axisLen1[1] * absC[1][0] + axisLen1[2] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(glm::dot(centerDiff, otheraxisVec[1]));
	r0 = axisLen2[1];
	r1 = axisLen1[0] * absC[0][1] + axisLen1[1] * absC[1][1] + axisLen1[2] * absC[2][1];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(glm::dot(centerDiff, otheraxisVec[2]));
	r0 = axisLen2[2];
	r1 = axisLen1[0] * absC[0][2] + axisLen1[1] * absC[1][2] + axisLen1[2] * absC[2][2];
	if (r > r0 + r1) {
		return false;
	}

	if (existParallelPair) {
		return true;	//pararell & r0, r1의 거리차가 존재하지 않음 => collision
	}

	//ob1의 axis와 otherOBB의 axis에 수직인 축을 기준으로 check. (3 axis * 3 axis)
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = axisLen1[1] * absC[2][0] + axisLen1[2] * absC[1][0];
	r1 = axisLen2[1] * absC[0][2] + axisLen1[2] * absC[0][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = axisLen1[1] * absC[2][1] + axisLen1[2] * absC[1][1];
	r1 = axisLen2[0] * absC[0][2] + axisLen1[2] * absC[0][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = axisLen1[1] * absC[2][2] + axisLen1[2] * absC[1][2];
	r1 = axisLen2[0] * absC[0][1] + axisLen1[2] * absC[0][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = axisLen1[0] * absC[2][0] + axisLen1[2] * absC[0][0];
	r1 = axisLen2[1] * absC[1][2] + axisLen1[2] * absC[1][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = axisLen1[0] * absC[2][1] + axisLen1[2] * absC[0][1];
	r1 = axisLen2[0] * absC[1][2] + axisLen1[2] * absC[1][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = axisLen1[0] * absC[2][2] + axisLen1[2] * absC[0][2];
	r1 = axisLen2[0] * absC[1][1] + axisLen1[1] * absC[1][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = axisLen1[0] * absC[1][0] + axisLen1[1] * absC[0][0];
	r1 = axisLen2[1] * absC[2][2] + axisLen1[2] * absC[2][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = axisLen1[0] * absC[1][1] + axisLen1[1] * absC[0][1];
	r1 = axisLen2[0] * absC[2][2] + axisLen1[2] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = axisLen1[0] * absC[1][2] + axisLen1[1] * absC[0][2];
	r1 = axisLen2[0] * absC[2][1] + axisLen1[1] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}
	return true;
}

bool CollisionFuncStatic::staticCheck_BOX(const glm::vec3 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2)
{
	glm::mat4 tempAABBWorld = glm::mat4();

	for (int i = 0; i < 3; i++)
		tempAABBWorld[3][i] = wolrd1[i];

	return staticCheck_BOX(tempAABBWorld, wolrd2, axisLen1, axisLen2);
}

bool CollisionFuncStatic::checkOBBOneINFLineIntersection(const OBBOb & obbOb, const LineOb & lineOb, float& retDist)
{
	const glm::mat4& worldMat = obbOb.getMatConstRef();
	const glm::vec3& halfAxisSize = obbOb.getAxisConstRef();
	const glm::vec3& lineVec = lineOb.getUnitVecConstRef();
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();

	glm::vec3 worldPos = worldMat[3];
	glm::vec3 worldCenterDiff = worldPos - lineStartPos;

	float rotatedDiff[3];
	glm::vec3 rotateAxis[3];

	// check start pos in OBB
	bool checkStartPosIn = true;
	for (int i = 0; i < 3; i++)
	{
		rotateAxis[i] = glm::vec3(worldMat[i]);
		rotatedDiff[i] = glm::dot(rotateAxis[i], worldCenterDiff);

		if (!(-halfAxisSize[i] < rotatedDiff[i] && rotatedDiff[i] < halfAxisSize[i]))
		{
			checkStartPosIn = false;
			break;
		}
	}

	if (checkStartPosIn)
	{
		retDist = 0.0f;
		return true;
	}

	float minDist = std::numeric_limits<float>::lowest();
	float maxDist = std::numeric_limits<float>::infinity();
	glm::vec3 T_vec1, T_vec2;

	for (int i = 0; i < 3; i++)
	{
		// rotatedLineVec = lineVec[0] * _worldMat[i][0] + lineVec[1] * _worldMat[i][1] + lineVec[2] * _worldMat[i][2];	// dot
		float rotatedLineVec = glm::dot(rotateAxis[i], lineVec);	// dot

		if (rotatedLineVec < 0.001f)
		{
			// pararell
			if ((0.0f < worldCenterDiff[i] - halfAxisSize[i]) || (0.0f > worldCenterDiff[i] + halfAxisSize[i]))
				return false;
		}
		else
		{
			T_vec1[i] = (rotatedDiff[i] - halfAxisSize[i]) / rotatedLineVec;
			T_vec2[i] = (rotatedDiff[i] + halfAxisSize[i]) / rotatedLineVec;

			if (T_vec1[i] > T_vec2[i])
			{
				float temp = T_vec1[i];
				T_vec1[i] = T_vec2[i];
				T_vec2[i] = temp;
			}

			if (T_vec1[i] > minDist)
			{
				minDist = T_vec1[i];
			}

			if (T_vec2[i] < maxDist)
			{
				maxDist = T_vec2[i];
			}

			if ((minDist > maxDist) || (maxDist < 0))
			{
				return false;
			}
		}
	}

	retDist = minDist;
	return true;
}

bool CollisionFuncStatic::checkOBBLineIntersection(const OBBOb & obbOb, const LineOb & lineOb, float& retDist)
{
	const glm::mat4& worldMat = obbOb.getMatConstRef();
	const glm::vec3& halfAxisSize = obbOb.getAxisConstRef();
	const glm::vec3& lineVec = lineOb.getUnitVecConstRef();
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();
	float lineLen = lineOb.getLen();

	float minDist = std::numeric_limits<float>::lowest();
	float maxDist = std::numeric_limits<float>::infinity();
	glm::vec3 T_vec1, T_vec2;

	if (staticCheck_POINT_OBB(lineStartPos, obbOb))
	{
		retDist = 0.0f;
		return true;
	}

	glm::vec3 worldPos = worldMat[3];
	glm::vec3 worldCenterDiff = worldPos - lineStartPos;

	float rotatedDiff[3];
	glm::vec3 rotateAxis[3];

	// check start pos in OBB
	bool checkStartPosIn = true;
	for (int i = 0; i < 3; i++)
	{
		rotateAxis[i] = worldMat[i];
		rotatedDiff[i] = glm::dot(rotateAxis[i], worldCenterDiff);

		if (!(-halfAxisSize[i] < rotatedDiff[i] && rotatedDiff[i] < halfAxisSize[i]))
		{
			checkStartPosIn = false;
			break;
		}
	}

	if (checkStartPosIn)
	{
		retDist = 0.0f;
		return true;
	}

	for (int i = 0; i < 3; i++)
	{
		// rotatedLineVec = lineVec[0] * worldMat[i][0] + lineVec[1] * worldMat[i][1] + lineVec[2] * worldMat[i][2];	// dot
		float rotatedLineVec = glm::dot(rotateAxis[i], lineVec);	// dot

		if (rotatedLineVec < 0.001f)
		{
			// pararell
			if ((0.0f < worldCenterDiff[i] - halfAxisSize[i]) || (0.0f > worldCenterDiff[i] + halfAxisSize[i]))
				return false;
		}
		else
		{
			T_vec1[i] = (rotatedDiff[i] - halfAxisSize[i]) / rotatedLineVec;
			T_vec2[i] = (rotatedDiff[i] + halfAxisSize[i]) / rotatedLineVec;

			if (T_vec1[i] > T_vec2[i])
			{
				float temp = T_vec1[i];
				T_vec1[i] = T_vec2[i];
				T_vec2[i] = temp;
			}

			if (T_vec1[i] > minDist)
			{
				minDist = T_vec1[i];
			}

			if (T_vec2[i] < maxDist)
			{
				maxDist = T_vec2[i];
			}

			if ((minDist > maxDist) || (maxDist < 0))
			{
				return false;
			}
		}
	}

	retDist = minDist;

	if (retDist > lineLen)
		return false;

	return true;
}

bool CollisionFuncStatic::checkAABBOneINFLineIntersection(const AABBOb & aabbOb, const LineOb & lineOb, float& retDist)
{
	const glm::vec3& worldVec = aabbOb.getCenterConstRef();
	const glm::vec3& halfAxisSize = aabbOb.getAxisConstRef();
	const glm::vec3& lineVec = lineOb.getUnitVecConstRef();
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();

	float minDist = std::numeric_limits<float>::lowest();
	float maxDist = std::numeric_limits<float>::infinity();
	glm::vec3 T_vec1, T_vec2;

	if (staticCheck_POINT_AABB(lineStartPos, aabbOb))
	{
		retDist = 0.0f;
		return true;
	}

	for (int i = 0; i < 3; i++)
	{
		if (lineVec[i] < 0.001f)
		{
			// pararell
			if ((lineStartPos[i] < worldVec[i] - halfAxisSize[i]) || (lineStartPos[i] > worldVec[i] + halfAxisSize[i]))
				return false;
		}
		else
		{
			T_vec1[i] = (worldVec[i] - halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			T_vec2[i] = (worldVec[i] + halfAxisSize[i] - lineStartPos[i]) / lineVec[i];

			if (T_vec1[i] > T_vec2[i])
			{
				float temp = T_vec1[i];
				T_vec1[i] = T_vec2[i];
				T_vec2[i] = temp;
			}

			if (T_vec1[i] > minDist)
			{
				minDist = T_vec1[i];
			}

			if (T_vec2[i] < maxDist)
			{
				maxDist = T_vec2[i];
			}

			if ((minDist > maxDist) || (maxDist < 0))
			{
				return false;
			}
		}
	}

	retDist = minDist;
	return true;
}

bool CollisionFuncStatic::checkAABBLineIntersection(const AABBOb & aabbOb, const LineOb & lineOb, float& retDist)
{
	const glm::vec3& worldVec = aabbOb.getCenterConstRef();
	const glm::vec3& halfAxisSize = aabbOb.getAxisConstRef();
	const glm::vec3& lineVec = lineOb.getUnitVecConstRef();
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();
	float lineLen = lineOb.getLen();
	glm::vec3 T_vec1;
	int longestIdx;

	if (staticCheck_POINT_AABB(lineStartPos, aabbOb))
	{
		retDist = 0.0f;
		return true;
	}


	for (int i = 0; i < 3; i++)
	{
		if (lineVec[i] < 0.001f)
		{
			// pararell
			if ((lineStartPos[i] < worldVec[i] - halfAxisSize[i]) || (lineStartPos[i] > worldVec[i] + halfAxisSize[i]))
				return false;
		}
		else
		{
			if (lineVec[i] > 0.0f)
			{
				T_vec1[i] = (worldVec[i] - halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			}
			else
			{
				T_vec1[i] = (worldVec[i] + halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			}

			if (retDist < T_vec1[i])
			{
				retDist = T_vec1[i];
				longestIdx = i;
			}
		}
	}

	// check dist in range [0 ~ len]
	if (!(0 <= retDist && retDist <= lineLen))
	{
		return false;
	}

	// check hit point is in AABB
	int idx1 = (longestIdx + 1) % 3;
	int idx2 = (longestIdx + 2) % 3;

	float otherLen1 = lineStartPos[idx1] + lineVec[idx1] * lineLen;
	float otherLen2 = lineStartPos[idx2] + lineVec[idx2] * lineLen;

	if (!(worldVec[idx1] - halfAxisSize[idx1] <= otherLen1 && otherLen1 <= worldVec[idx1] + halfAxisSize[idx1]))
	{
		return false;
	}

	if (!(worldVec[idx2] - halfAxisSize[idx2] <= otherLen2 && otherLen2 <= worldVec[idx2] + halfAxisSize[idx2]))
	{
		return false;
	}

	return true;
}

/*
bool CollisionFuncStatic::checkINFLineIntersection(const AABBOb & aabbOb, const LineOb & lineOb, float& retDist)
{
	return false;
}

bool CollisionFuncStatic::checkINFLineIntersection(const OBBOb & obbOb, const  LineOb & lineOb, float & retDist)
{
	return false;
}
*/

// refence : https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
bool CollisionFuncStatic::checkSphereOneINFLineIntersection(const SphereOb & sphereOb, const LineOb & lineOb, float & retDist)
{
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();
	const glm::vec3& lineUnitVec = lineOb.getUnitVecConstRef();
	float lineLen = lineOb.getLen();

	const glm::vec3& sphereCenter = sphereOb.getCenterConstRef();
	float radius = sphereOb.getRadius();

	if (staticCheck_POINT_SPHERE(lineStartPos, sphereOb))
	{
		retDist = 0.0f;
		return true;
	}

	glm::vec3 centerToStartPosVec = lineStartPos - sphereCenter;
	float distOrthoUnit = glm::dot(centerToStartPosVec, lineUnitVec);

	// centerToStartPosVecSize - radius^2
	float c = glm::dot(centerToStartPosVec, centerToStartPosVec) - radius * radius;

	if (c > 0.0f && distOrthoUnit > 0.0f)
	{
		return false;
	}

	float discr = distOrthoUnit * distOrthoUnit - c;
	if (discr < 0.0f)
		return false;

	retDist = -distOrthoUnit - std::sqrtf(discr);
	if (retDist < 0.0f)
		retDist = 0.0f;

	return true;
}

bool CollisionFuncStatic::checkSphereLineIntersection(const SphereOb & sphereOb, const LineOb & lineOb, float & retDist)
{
	const glm::vec3& lineStartPos = lineOb.getStartPosConstRef();
	const glm::vec3& lineUnitVec = lineOb.getUnitVecConstRef();
	float lineLen = lineOb.getLen();

	const glm::vec3& sphereCenter = sphereOb.getCenterConstRef();
	float radius = sphereOb.getRadius();

	if (staticCheck_POINT_SPHERE(lineStartPos, sphereOb))
	{
		retDist = 0.0f;
		return true;
	}

	glm::vec3 centerToStartPosVec = lineStartPos - sphereCenter;
	float distOrthoUnit = glm::dot(centerToStartPosVec, lineUnitVec);
	
	// centerToStartPosVecSize - radius^2
	float c = glm::dot(centerToStartPosVec, centerToStartPosVec) - radius * radius;

	if (c > 0.0f && distOrthoUnit > 0.0f)
	{
		return false;
	}

	float discr = distOrthoUnit * distOrthoUnit - c;
	if (discr < 0.0f)
		return false;

	retDist = -distOrthoUnit - std::sqrtf(discr);
	if (retDist < 0.0f)
		retDist = 0.0f;

	if (retDist > lineLen)
		return false;

	return true;
}

/*
bool CollisionFuncStatic::staticCheck_POINT_LINE(const glm::vec3 & point,const  LineOb & lineOb)
{
	return false;
}

bool CollisionFuncStatic::staticCheck_Dist_POINT_LINE(const glm::vec3 & point, const LineOb & lineOb, float & retDist)
{
	return false;
}

bool CollisionFuncStatic::staticCheck_CLOSEST_POINT_LINE(const glm::vec3 & point, const LineOb & lineOb, glm::vec3 & retClosestPoint)
{
	return false;
}
*/

bool CollisionFuncStatic::staticCheck_POINT_SPHERE(const glm::vec3 & point, const SphereOb & sphereOb)
{
	const glm::vec3& center = sphereOb.getCenterConstRef();
	float radius = sphereOb.getRadius();
	float dist2 = (point[0] - center[0]) * (point[0] - center[0]) + (point[1] - center[1]) * (point[1] - center[1])
		+ (point[2] - center[2]) * (point[2] - center[2]);

	if (dist2 <= radius * radius)
		return true;

	return false;
}

// dist from shpere shell ~ point
bool CollisionFuncStatic::staticCheck_Dist_POINT_SPHERE(const glm::vec3 & point, const SphereOb & sphereOb, float & retDist)
{
	const glm::vec3& center	= sphereOb.getCenterConstRef();
	float radius		= sphereOb.getRadius();
	glm::vec3 centerToPoint = point - center;

	float dist = (centerToPoint[0]) * (centerToPoint[0]) + (centerToPoint[1]) * (centerToPoint[1])
		+ (centerToPoint[2]) * (centerToPoint[2]);
	dist = std::sqrt(dist);

	if (std::sqrt(dist) > radius)
	{
		// point out
		retDist = dist - radius;
		return false;
	}
	else
	{
		// in
		retDist = 0.0f;
		return true;
	}
}

bool CollisionFuncStatic::staticCheck_CLOSEST_POINT_SPHERE(const glm::vec3 & point, const SphereOb & sphereOb, glm::vec3 & retClosestPoint)
{
	const glm::vec3& center = sphereOb.getCenterConstRef();
	float radius = sphereOb.getRadius();
	glm::vec3 centerToPoint = point - center;
	float dist2 = (centerToPoint[0]) * (centerToPoint[0]) + (centerToPoint[1]) * (centerToPoint[1])
		+ (centerToPoint[2]) * (centerToPoint[2]);

	if (dist2 <= radius * radius)
	{
		// in
		retClosestPoint = point;
		return true;
	}

	// out
	centerToPoint = glm::normalize(centerToPoint);
	retClosestPoint = center + std::sqrt(dist2) * centerToPoint;
	return false;
}

bool CollisionFuncStatic::staticCheck_POINT_AABB(const glm::vec3 & point, const AABBOb & aabbOb)
{
	const glm::vec3& worldVec = aabbOb.getCenterConstRef();
	const glm::vec3& halfAxisSize = aabbOb.getAxisConstRef();

	for (int i = 0; i < 3; i++)
	{
		if (!(worldVec[i] - halfAxisSize[i] <= point[i] && point[i] <= worldVec[i] + halfAxisSize[i]))
		{
			return false;
		}
	}
	return true;
}

bool CollisionFuncStatic::staticCheck_Dist_POINT_AABB(const glm::vec3 & point, const AABBOb & aabbOb, float & retDist)
{
	bool collide = true;
	retDist = 0.0f;

	const glm::vec3& aabbCenter = aabbOb.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbOb.getAxisConstRef();

	glm::vec3 oneDist = aabbCenter - aabbAxis;
	glm::vec3 twoDist = aabbCenter + aabbAxis;

	for (int i = 0; i < 3; i++)
	{
		if (point[i] < oneDist[i])
		{
			collide = false;
			retDist += (point[i] - oneDist[i]) * (point[i] - oneDist[i]);
		}

		if (point[i] > twoDist[i])
		{
			collide = false;
			retDist += (point[i] - twoDist[i]) * (point[i] - twoDist[i]);
		}
	}

	retDist = std::sqrt(retDist);
	return collide;
}
