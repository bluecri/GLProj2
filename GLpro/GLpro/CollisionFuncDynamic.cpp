#include "CollisionFuncDynamic.h"
#include "CollisionFuncStatic.h"

#include "OBBOb.h"
#include "AABBOb.h"
#include "SphereOb.h"
#include "LineOb.h"
#include "FrustumOb.h"

bool CollisionFuncDynamic::dynamicCheck_DLINE_DLINE(const LineOb & dLine1, const LineOb & dLine2, float & retTime)
{

	printf_s("[LOG] not implemented dynamicCheck_DLINE_DLINE\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_SLINE(const LineOb & dLine, const LineOb & sLine, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_SLINE\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_DSPHERE(const LineOb & dLine, const SphereOb & sSphere, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_DSPHERE\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_SSPHERE(const LineOb & dLine, const SphereOb & dSphere, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_SSPHERE\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_DAABB(const LineOb & dLine, const AABBOb & dAabb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_DAABB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_SAABB(const LineOb & dLine, const AABBOb & sAabb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_SAABB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_DOBB(const LineOb & dLine, const OBBOb & dObb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_DOBB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DLINE_SOBB(const LineOb & dLine, const OBBOb & sObb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DLINE_SOBB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_SLINE(const SphereOb & dSphere, const glm::vec3 & prevSpherePos, const LineOb & sLine, float & retTime)
{
	return CollisionFuncDynamic::check_LINE_CAPSULE(sLine, prevSpherePos, dSphere.getCenterConstRef(), dSphere.getRadius(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_DSPHERE(const SphereOb & dSphere1, const glm::vec3& prevSphere1Pos, const SphereOb & dSphere2, const glm::vec3& prevSphere2Pos, float & retTime)
{
	// creatia : dSphere1
	glm::vec3 moveStartPos = prevSphere2Pos - prevSphere1Pos;
	glm::vec3 moveEndPos = dSphere2.getCenterConstRef() - dSphere1.getCenterConstRef();

	LineOb moveLIne(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLIne.setStartEndPos(moveStartPos, moveEndPos);

	SphereOb creatiaSphere;
	creatiaSphere.updateSphereOb(glm::vec3());
	creatiaSphere.updateSphereOb(dSphere1.getRadius() + dSphere2.getRadius());

	float collisionDist;
	bool collisionResult = CollisionFuncStatic::staticCheck_LINE_SPHERE(moveLIne, creatiaSphere, collisionDist);

	if (collisionResult == false)
		return false;

	retTime = collisionDist / moveLIne.getLenRef();
	return true;
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_SSPHERE(const SphereOb & dSphere, const glm::vec3& prevDSphere1Pos, const SphereOb & sSphere, float & retTime)
{
	// creatia : sSphere
	glm::vec3 moveStartPos = sSphere.getCenterConstRef() - prevDSphere1Pos;
	glm::vec3 moveEndPos = sSphere.getCenterConstRef() - dSphere.getCenterConstRef();

	LineOb moveLIne(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLIne.setStartEndPos(moveStartPos, moveEndPos);

	SphereOb creatiaSphere;
	creatiaSphere.updateSphereOb(glm::vec3());
	creatiaSphere.updateSphereOb(dSphere.getRadius() + sSphere.getRadius());

	float collisionDist;
	bool collisionResult = CollisionFuncStatic::staticCheck_LINE_SPHERE(moveLIne, creatiaSphere, collisionDist);

	if (collisionResult == false)
		return false;

	retTime = collisionDist / moveLIne.getLenRef();
	return true;
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_DAABB(const SphereOb & dSphere, const glm::vec3& prevDSpherePos, const AABBOb & dAabb, const glm::vec3& prevDAabbPos, float & retTime)
{
	// use check_SAABB_SPHERE_Capsule : creatia = post AABB
	LineOb moveLine(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLine.setStartEndPos(prevDSpherePos - prevDAabbPos, dSphere.getCenterConstRef());
	return check_SAABB_SPHERE_Capsule(moveLine, dAabb, dSphere.getRadius(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_SAABB(const SphereOb & dSphere, const glm::vec3& prevDSpherePos, const AABBOb & sAabb, float & retTime)
{
	// use check_SAABB_SPHERE_Capsule : creatia = post AABB
	LineOb moveLine(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLine.setStartEndPos(prevDSpherePos, dSphere.getCenter());
	return check_SAABB_SPHERE_Capsule(moveLine, sAabb, dSphere.getRadius(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_DOBB(const SphereOb & dSphere, const OBBOb & dObb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DSPHERE_DOBB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DSPHERE_SOBB(const SphereOb & dSphere, const glm::vec3& prevDSpherePos, const OBBOb & sObb, float & retTime)
{
	// use check_SOBB_SPHERE_CAPSULE : creatia = OBB
	LineOb moveLine(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLine.setStartEndPos(prevDSpherePos, dSphere.getCenter());
	return check_SOBB_SPHERE_CAPSULE(moveLine, sObb, dSphere.getRadius(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_SLINE(const AABBOb & dAabb, const glm::vec3 & prevDAabbPos, const LineOb & sLine, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DAABB_SLINE\n");
	//use check_SLINE_DAABB
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_SSPHERE(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const SphereOb & sSphere, float & retTime)
{
	// use check_SAABB_SPHERE_Capsule : creatia = post AABB
	LineOb moveLine(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	moveLine.setStartEndPos(sSphere.getCenter() - prevDAabbPos, sSphere.getCenter());
	return check_SAABB_SPHERE_Capsule(moveLine, dAabb, sSphere.getRadius(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_DAABB(const AABBOb & dAabb1, const glm::vec3& prevDAabb1Pos, const AABBOb & dAabb2, const glm::vec3& prevDAabb2Pos, float & retTime)
{
	// use check_ABBB_ABBB
	LineOb line(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	line.setStartEndPos(prevDAabb1Pos - prevDAabb2Pos, dAabb1.getCenterConstRef());
	return check_ABBB_ABBB(line, dAabb2, dAabb1.getAxisConstRef() + dAabb2.getAxisConstRef(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_SAABB(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const AABBOb & sAabb, float & retTime)
{
	// use check_ABBB_ABBB
	LineOb line(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	line.setStartEndPos(prevDAabbPos, dAabb.getCenterConstRef());
	return check_ABBB_ABBB(line, sAabb, dAabb.getAxisConstRef() + sAabb.getAxisConstRef(), retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_DOBB(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const OBBOb & dObb, const glm::mat4& prevDObbMat, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DAABB_DOBB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DAABB_SOBB(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const OBBOb & sObb, float & retTime)
{
	return check_SOBB_DABBB(sObb, dAabb, prevDAabbPos, retTime);
}

bool CollisionFuncDynamic::dynamicCheck_DOBB_SSPHERE(const OBBOb & dAabb, const SphereOb & sSphere, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DOBB_SSPHERE\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DOBB_SAABB(const OBBOb & dObb1, const AABBOb& sAabb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DOBB_SAABB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DOBB_DOBB(const OBBOb & dObb1, const OBBOb & dObb2, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DOBB_DOBB\n");
	return false;
}

bool CollisionFuncDynamic::dynamicCheck_DOBB_SOBB(const OBBOb & dObb, const OBBOb & sObb, float & retTime)
{
	printf_s("[LOG] not implemented dynamicCheck_DOBB_SOBB\n");
	return false;
}

bool CollisionFuncDynamic::check_LINE_CAPSULE(const LineOb & lineOb, const glm::vec3 & center1, const glm::vec3 & center2, float radius, float & retTime)
{
	float retDist;
	bool collideResult = CollisionFuncStatic::staticCheck_LINE_CAPSULE(lineOb, center1, center2, radius, retDist);

	if (collideResult = false)
		return false;

	glm::vec3 collidePos = lineOb.getUnitVecConstRef() * retDist;
	glm::vec3 sphereStartToCollidePos = collidePos - center1;
	glm::vec3 sphereStartToEndUnitVec = glm::normalize(center2 - center1);
	float orthoOnSphereMoveLineUnitVec = glm::dot(sphereStartToCollidePos, sphereStartToEndUnitVec);

	retTime = orthoOnSphereMoveLineUnitVec / glm::length(sphereStartToCollidePos);
	retTime = std::max(retTime, 0.0f);
	retTime = std::min(retTime, 1.0f);

	return true;
}

bool CollisionFuncDynamic::check_SLINE_DAABB(const LineOb & sLine, const AABBOb & dAabb, const glm::vec3 & prevDAabbPos, float & retTime)
{
	printf_s("[LOG] not implemented check_SLINE_DAABB\n");
	return false;
}

// creatia = post AABB, [reference : realtime colliision Detection]
bool CollisionFuncDynamic::check_SAABB_SPHERE_Capsule(const LineOb & line, const AABBOb & aabb, float radius, float & retTime)
{
	if (line.getType() != LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN)
	{
		printf_s("[LOG] : wrong LINEOB_TYPE_ENUM type check_SAABB_SPHERE_Capsule\n");
		return false;
	}

	const glm::vec3& aabbCenter = aabb.getCenterConstRef();;
	const glm::vec3& aabbAxis = aabb.getAxisConstRef();;

	float firstPointDist;
	AABBOb aabbBig;

	glm::vec3 bigAxis = aabbAxis;
	for (int i = 0; i < 3; i++)
	{
		bigAxis[i] += radius;
	}
	aabbBig.updateAABBObCenter(aabbCenter);
	aabbBig.updateAABBObAxis(bigAxis);

	if (false == CollisionFuncStatic::staticCheck_LINE_AABB(line, aabbBig, firstPointDist))
		return false;

	// firstPointOnBigAABB
	glm::vec3 p = line.getStartPosConstRef() + line.getUnitVecConstRef() * firstPointDist;

	int u = 0, v = 0;	// pos check bit
	
	glm::vec3 minPos = aabbCenter - aabbAxis;
	glm::vec3 maxPos = aabbCenter + aabbAxis;

	if (p.x < minPos.x)	u += 1;
	if (p.x > maxPos.x)	v += 1;
	if (p.y < minPos.y)	u += 2;
	if (p.y > maxPos.y)	v += 2;
	if (p.z < minPos.z)	u += 4;
	if (p.z > maxPos.z)	v += 4;

	int checkBit = u + v;

	if (checkBit == 7)
	{
		// out axis count = 3
		// check 3 capsule at aabb point
		float testDist;
		float retDist = std::numeric_limits<float>::max();
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(line, corner(minPos, maxPos, v), corner(minPos, maxPos, v ^ 1), radius, testDist))
			retDist = min(retDist, testDist);
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(line, corner(minPos, maxPos, v), corner(minPos, maxPos, v ^ 2), radius, testDist))
			retDist = min(retDist, testDist);
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(line, corner(minPos, maxPos, v), corner(minPos, maxPos, v ^ 4), radius, testDist))
			retDist = min(retDist, testDist);
		if (retDist > line.getLen())
			return false;

		retTime = retDist / line.getLen();
		return true;
	}
	if ((checkBit & (checkBit - 1)) == 0)	// check only 1 bit exist
	{
		// out axis count = 1
		// collision on face. (point p)
		retTime = firstPointDist / line.getLen();
		return true;
	}

	// out axis count = 2
	// check 1 capsule at aabb edge
	float retDist;
	if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(line, corner(minPos, maxPos, v), corner(minPos, maxPos, u ^ 7), radius, retDist))
		return false;

	retTime = retDist / line.getLen();
	return true;
}

bool CollisionFuncDynamic::check_SOBB_SPHERE_CAPSULE(const LineOb & dLine, const OBBOb & sObb, float radius, float & retTime)
{
	if (dLine.getType() != LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN)
	{
		printf_s("[LOG] : wrong LINEOB_TYPE_ENUM type check_SAABB_SPHERE_Capsule\n");
		return false;
	}
	const glm::mat4& obbMat = sObb.getMatConstRef();
	const glm::vec3 obbCenter = glm::vec3(obbMat[3]);
	const glm::vec3& obbAxis = sObb.getAxisConstRef();;

	float firstPointDist;
	OBBOb obbBig;

	glm::vec3 bigAxis = obbAxis;
	for (int i = 0; i < 3; i++)
	{
		bigAxis[i] += radius;
	}
	obbBig.updateOBBObMat(obbMat);
	obbBig.updateOBBObAxis(bigAxis);

	if (false == CollisionFuncStatic::staticCheck_LINE_OBB(dLine, obbBig, firstPointDist))
		return false;

	// firstPointOnBigAABB
	glm::vec3 p = dLine.getStartPosConstRef() + dLine.getUnitVecConstRef() * firstPointDist;
	glm::vec3 centerToP = p - obbCenter;

	int u = 0, v = 0;	// pos check bit

	//glm::vec3 minPos = obbCenter - obbAxis;
	//glm::vec3 maxPos = obbCenter + obbAxis;

	float orthoToAxis;
	orthoToAxis = glm::dot(glm::vec3(obbMat[0]), centerToP);
	if (orthoToAxis < obbAxis.x)	u += 1;
	if (orthoToAxis > obbAxis.x)	v += 1;

	orthoToAxis = glm::dot(glm::vec3(obbMat[1]), centerToP);
	if (orthoToAxis < obbAxis.y)	u += 2;
	if (orthoToAxis > obbAxis.y)	v += 2;

	orthoToAxis = glm::dot(glm::vec3(obbMat[2]), centerToP);
	if (orthoToAxis < obbAxis.z)	u += 4;
	if (orthoToAxis > obbAxis.z)	v += 4;

	int checkBit = u + v;

	std::vector<glm::vec3> axisLenVec;

	for (int i = 0; i < 3; i++)
	{
		axisLenVec.push_back(obbMat[i] * obbAxis[i]);
	}

	if (checkBit == 7)
	{
		// out axis count = 3
		// check 3 capsule at aabb point
		float testDist;
		float retDist = std::numeric_limits<float>::max();
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(dLine, cornerObb(obbCenter, axisLenVec, v), cornerObb(obbCenter, axisLenVec, v ^ 1), radius, testDist))
			retDist = min(retDist, testDist);
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(dLine, cornerObb(obbCenter, axisLenVec, v), cornerObb(obbCenter, axisLenVec, v ^ 2), radius, testDist))
			retDist = min(retDist, testDist);
		if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(dLine, cornerObb(obbCenter, axisLenVec, v), cornerObb(obbCenter, axisLenVec, v ^ 4), radius, testDist))
			retDist = min(retDist, testDist);
		if (retDist > dLine.getLen())
			return false;

		retTime = retDist / dLine.getLen();
		return true;
	}
	if ((checkBit & (checkBit - 1)) == 0)	// check only 1 bit exist
	{
		// out axis count = 1
		// collision on face. (point p)
		retTime = firstPointDist / dLine.getLen();
		return true;
	}

	// out axis count = 2
	// check 1 capsule at aabb edge
	float retDist;
	if (CollisionFuncStatic::staticCheck_LINE_CAPSULE(dLine, cornerObb(obbCenter, axisLenVec, v), cornerObb(obbCenter, axisLenVec, u ^ 7), radius, retDist))
		return false;

	retTime = retDist / dLine.getLen();
	return true;
}

bool CollisionFuncDynamic::check_ABBB_ABBB(const LineOb & line, const AABBOb & creatiaAabb, glm::vec3& axisSum, float & retTime)
{
	float retDist;
	bool isCollide = CollisionFuncStatic::staticCheck_LINE_AABB(line, creatiaAabb, retDist);
	if (isCollide == false)
		return false;
	retTime = retDist / line.getLen();
	return true;
}

bool CollisionFuncDynamic::check_SOBB_DABBB(const OBBOb & sObb, const AABBOb & dAabb, const glm::vec3& prevDAabbPos, float & retTime)
{
	float retTimeMin = 0.0f;
	float retTimeMax = 1.0f;
	const glm::vec3& axisLen1 = dAabb.getAxisConstRef();
	const glm::vec3& axisLen2 = sObb.getAxisConstRef();
	glm::mat4 wolrd1 = glm::mat4();
	glm::mat4 wolrd2 = sObb.getMatConstRef();

	double c[3][3];	//ob1 3 axis vs otherOBB 3 axis consine value
	double absC[3][3];	//abs of c[][]
	double d[3][2];
	double r[2], r0, r1;	//r = 한 축을 기준으로 obBox 두 중심점의 거리
	const float cutoff = 0.9999f;
	bool existParallelPair = false;	//paralell 한가?

	glm::vec3 axisVec[3];
	glm::vec3 otheraxisVec[3];

	for (int i = 0; i < 3; i++)
	{
		axisVec[i] = wolrd1[i];
		otheraxisVec[i] = wolrd2[i];
	}
	glm::vec3 centerDiffC = (dAabb.getCenterConstRef() - prevDAabbPos);
	glm::vec3 centerDiffConst = -glm::vec3(wolrd2[3]);

	// centerDiff = centerDiffC * time([0, 1]) - centerDiffConst

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

	d[0][0] = glm::dot(centerDiffC, axisVec[0]);
	d[0][1] = glm::dot(centerDiffConst, axisVec[0]);
	r[0] = abs(d[0][0]);
	r[1] = abs(d[0][1]);
	r0 = axisLen1[0];
	r1 = axisLen2[0] * absC[0][0] + axisLen2[1] * absC[0][1] + axisLen2[2] * absC[0][2];

	float t = (r0 + r1 - r[1]) / r[0];
	if (r[0] > 0.0f)
	{
		// t <= ( r0 + r1 - r[1] ) / r[0]
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		// t >= ( r0 + r1 - r[1] ) / r[0]
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	for (int i = 0; i < 3; i++) {
		c[1][i] = glm::dot(axisVec[1], otheraxisVec[i]);
		absC[1][i] = glm::abs(c[1][i]);
		if (absC[1][i] > cutoff) {
			existParallelPair = true;
		}
	}
	d[1][0] = glm::dot(centerDiffC, axisVec[1]);
	d[0][1] = glm::dot(centerDiffConst, axisVec[1]);
	r[0] = abs(d[1][0]);
	r[1] = abs(d[1][1]);
	r0 = axisLen1[1];
	r1 = axisLen2[1] * absC[1][0] + axisLen2[1] * absC[1][1] + axisLen2[2] * absC[1][2];

	t = (r0 + r1 - r[1]) / r[0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	for (int i = 0; i < 3; i++) {
		c[2][i] = glm::dot(axisVec[2], otheraxisVec[i]);
		absC[2][i] = glm::abs(c[2][i]);
		if (absC[2][i] > cutoff) {
			existParallelPair = true;
		}
	}

	d[2][0] = glm::dot(centerDiffC, axisVec[2]);
	d[2][0] = glm::dot(centerDiffConst, axisVec[2]);
	r[0] = abs(d[2][0]);
	r[1] = abs(d[2][1]);
	r0 = axisLen1[2];
	r1 = axisLen2[0] * absC[2][0] + axisLen2[1] * absC[2][1] + axisLen2[2] * absC[2][2];
	t = (r0 + r1 - r[1]) / r[0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	//ob1 3 face vs otherOBB 1 faces
	r[0] = abs(glm::dot(centerDiffC, otheraxisVec[0]));
	r[1] = abs(glm::dot(centerDiffConst, otheraxisVec[0]));

	r0 = axisLen2[0];
	r1 = axisLen1[0] * absC[0][0] + axisLen1[1] * absC[1][0] + axisLen1[2] * absC[2][0];
	t = (r0 + r1 - r[1]) / r[0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(glm::dot(centerDiffC, otheraxisVec[1]));
	r[1] = abs(glm::dot(centerDiffConst, otheraxisVec[1]));
	r0 = axisLen2[1];
	r1 = axisLen1[0] * absC[0][1] + axisLen1[1] * absC[1][1] + axisLen1[2] * absC[2][1];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(glm::dot(centerDiffC, otheraxisVec[2]));
	r[1] = abs(glm::dot(centerDiffConst, otheraxisVec[2]));
	r0 = axisLen2[2];
	r1 = axisLen1[0] * absC[0][2] + axisLen1[1] * absC[1][2] + axisLen1[2] * absC[2][2];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	if (existParallelPair) {
		return true;	//pararell & r0, r1의 거리차가 존재하지 않음 => collision
	}

	//ob1의 axis와 otherOBB의 axis에 수직인 축을 기준으로 check. (3 axis * 3 axis)
	r[0] = abs(d[2][0] * c[1][0] - d[1][0] * c[2][0]);
	r[1] = abs(d[2][1] * c[1][0] - d[1][1] * c[2][0]);
	r0 = axisLen1[1] * absC[2][0] + axisLen1[2] * absC[1][0];
	r1 = axisLen2[1] * absC[0][2] + axisLen1[2] * absC[0][1];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[2][0] * c[1][1] - d[1][0] * c[2][1]);
	r[1] = abs(d[2][1] * c[1][1] - d[1][1] * c[2][1]);
	r0 = axisLen1[1] * absC[2][1] + axisLen1[2] * absC[1][1];
	r1 = axisLen2[0] * absC[0][2] + axisLen1[2] * absC[0][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[2][0] * c[1][2] - d[1][0] * c[2][2]);
	r[1] = abs(d[2][1] * c[1][2] - d[1][1] * c[2][2]);
	r0 = axisLen1[1] * absC[2][2] + axisLen1[2] * absC[1][2];
	r1 = axisLen2[0] * absC[0][1] + axisLen1[2] * absC[0][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[0][0] * c[2][0] - d[2][0] * c[0][0]);
	r[1] = abs(d[0][1] * c[2][0] - d[2][1] * c[0][0]);
	r0 = axisLen1[0] * absC[2][0] + axisLen1[2] * absC[0][0];
	r1 = axisLen2[1] * absC[1][2] + axisLen1[2] * absC[1][1];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[0][0] * c[2][1] - d[2][0] * c[0][1]);
	r[1] = abs(d[0][1] * c[2][1] - d[2][1] * c[0][1]);
	r0 = axisLen1[0] * absC[2][1] + axisLen1[2] * absC[0][1];
	r1 = axisLen2[0] * absC[1][2] + axisLen1[2] * absC[1][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[0][0] * c[2][2] - d[2][0] * c[0][2]);
	r[1] = abs(d[0][1] * c[2][2] - d[2][1] * c[0][2]);
	r0 = axisLen1[0] * absC[2][2] + axisLen1[2] * absC[0][2];
	r1 = axisLen2[0] * absC[1][1] + axisLen1[1] * absC[1][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[1][0] * c[0][0] - d[0][0] * c[1][0]);
	r[1] = abs(d[1][1] * c[0][0] - d[0][1] * c[1][0]);
	r0 = axisLen1[0] * absC[1][0] + axisLen1[1] * absC[0][0];
	r1 = axisLen2[1] * absC[2][2] + axisLen1[2] * absC[2][1];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[1][0] * c[0][1] - d[0][0] * c[1][1]);
	r[1] = abs(d[1][1] * c[0][1] - d[0][1] * c[1][1]);
	r0 = axisLen1[0] * absC[1][1] + axisLen1[1] * absC[0][1];
	r1 = axisLen2[0] * absC[2][2] + axisLen1[2] * absC[2][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	r[0] = abs(d[1][0] * c[0][2] - d[0][0] * c[1][2]);
	r[1] = abs(d[1][1] * c[0][2] - d[0][1] * c[1][2]);
	r0 = axisLen1[0] * absC[1][2] + axisLen1[1] * absC[0][2];
	r1 = axisLen2[0] * absC[2][1] + axisLen1[1] * absC[2][0];
	if (r[0] > 0.0f)
	{
		if (t < retTimeMin)
			return false;
		retTimeMax = std::min(retTimeMin, t);
	}
	else
	{
		if (t > retTimeMax)
			return false;
		retTimeMin = std::max(retTimeMin, t);
	}

	// set retTime
	retTime = retTimeMin;

	return true;
}

glm::vec3 CollisionFuncDynamic::corner(const glm::vec3 & minPos, const glm::vec3 & maxPos, int maxBit)
{
	glm::vec3 retPoint;

	retPoint.x = (maxBit & 1) ? maxPos.x : minPos.x;
	retPoint.y = (maxBit & 2) ? maxPos.y : minPos.y;
	retPoint.z = (maxBit & 4) ? maxPos.z : minPos.z;

	return retPoint;
}

glm::vec3 CollisionFuncDynamic::cornerObb(const glm::vec3 & center, const std::vector<glm::vec3>& axisLenVec, int maxBit)
{
	glm::vec3 retPoint = center;

	retPoint += (maxBit & 1) ? axisLenVec[0] : -axisLenVec[0];
	retPoint += (maxBit & 2) ? axisLenVec[1] : -axisLenVec[1];
	retPoint += (maxBit & 4) ? axisLenVec[2] : -axisLenVec[2];

	return retPoint;
}
