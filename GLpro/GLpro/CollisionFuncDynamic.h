#pragma once
#include "stdafx.h"

class LineOb;
class SphereOb;
class AABBOb;
class OBBOb;
class FrustumOb;

class CollisionFuncDynamic
{
public:
	// return true if collision
	static bool dynamicCheck_DLINE_DLINE(const LineOb& dLine1, const LineOb& dLine2, float& retTime);
	static bool dynamicCheck_DLINE_SLINE(const LineOb& dLine, const LineOb& sLine, float& retTime);
	static bool dynamicCheck_DLINE_DSPHERE(const LineOb& dLine, const SphereOb& sSphere, float& retTime);
	static bool dynamicCheck_DLINE_SSPHERE(const LineOb& dLine, const SphereOb& dSphere, float& retTime);
	static bool dynamicCheck_DLINE_DAABB(const LineOb& dLine, const AABBOb& dAabb, float& retTime);
	static bool dynamicCheck_DLINE_SAABB(const LineOb& dLine, const AABBOb& sAabb, float& retTime);
	static bool dynamicCheck_DLINE_DOBB(const LineOb& dLine, const OBBOb& dObb, float& retTime);
	static bool dynamicCheck_DLINE_SOBB(const LineOb& dLine, const OBBOb& sObb, float& retTime);

	static bool dynamicCheck_DSPHERE_SLINE(const SphereOb& dSphere, const glm::vec3& prevSpherePos, const LineOb& sLine, float& retTime);
	static bool dynamicCheck_DSPHERE_DSPHERE(const SphereOb & dSphere1, const glm::vec3& prevSphere1Pos, const SphereOb & dSphere2, const glm::vec3& prevSphere2Pos, float & retTime);	//*
	static bool dynamicCheck_DSPHERE_SSPHERE(const SphereOb & dSphere, const glm::vec3& prevDSphere1Pos, const SphereOb & sSphere, float & retTime);	//*
	static bool dynamicCheck_DSPHERE_DAABB(const SphereOb & dSphere, const glm::vec3& prevDSpherePos, const AABBOb & dAabb, const glm::vec3& prevDAabbPos, float & retTime);	//*
	static bool dynamicCheck_DSPHERE_SAABB(const SphereOb& dSphere, const glm::vec3& prevDSpherePos, const AABBOb& sAabb, float& retTime);	//*
	static bool dynamicCheck_DSPHERE_DOBB(const SphereOb& dSphere, const OBBOb& dObb, float& retTime);
	static bool dynamicCheck_DSPHERE_SOBB(const SphereOb& dSphere, const glm::vec3& prevDSpherePos, const OBBOb& sObb, float& retTime);	//*

	static bool dynamicCheck_DAABB_SLINE(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const LineOb& sLine, float & retTime);	//*
	static bool dynamicCheck_DAABB_SSPHERE(const AABBOb & dAabb, const glm::vec3& prevDAabbPos, const SphereOb & sSphere, float & retTime);	//*
	static bool dynamicCheck_DAABB_DAABB(const AABBOb& dAabb1, const glm::vec3& prevDAabb1Pos, const AABBOb& dAabb2, const glm::vec3& prevDAabb2Pos, float& retTime);	//*
	static bool dynamicCheck_DAABB_SAABB(const AABBOb& dAabb, const glm::vec3& prevDAabbPos, const AABBOb& sAabb, float& retTime);	//*
	static bool dynamicCheck_DAABB_DOBB(const AABBOb& dAabb, const glm::vec3& prevDAabbPos, const OBBOb& dObb, const glm::mat4& prevDObbMap, float& retTime);
	static bool dynamicCheck_DAABB_SOBB(const AABBOb& dAabb, const glm::vec3& prevDAabbPos, const OBBOb& sObb, float& retTime);	//*

	static bool dynamicCheck_DOBB_SSPHERE(const OBBOb& dAabb, const SphereOb& sSphere, float& retTime);
	static bool dynamicCheck_DOBB_SAABB(const OBBOb& dObb1, const AABBOb& sAabb, float& retTime);
	static bool dynamicCheck_DOBB_DOBB(const OBBOb& dObb1, const OBBOb& dObb2, float& retTime);
	static bool dynamicCheck_DOBB_SOBB(const OBBOb& dObb, const OBBOb& sObb, float& retTime);

private:
	static bool check_LINE_CAPSULE(const LineOb& lineOb, const glm::vec3& center1, const glm::vec3& center2, float radius, float & retTime);
	static bool check_SLINE_DAABB(const LineOb& sLine, const AABBOb& dAabb, const glm::vec3 & prevDAabbPos, float& retTime);	//*
	static bool check_SAABB_SPHERE_Capsule(const LineOb& dLine, const AABBOb& dAabb, float radius, float& retTime);	//*
	static bool check_SOBB_SPHERE_CAPSULE(const LineOb& dLine, const OBBOb& sObb, float radius, float& retTime);	//*
	static bool check_ABBB_ABBB(const LineOb& line, const AABBOb& creatiaAabb, glm::vec3& axisSum, float& retTime);	//*
	static bool check_SOBB_DABBB(const OBBOb& sObb, const AABBOb& dAabb, const glm::vec3& prevDAabbPos, float& retTime);	//*
	static glm::vec3 corner(const glm::vec3& minPos, const glm::vec3& maxPos, int maxBit);
	static glm::vec3 cornerObb(const glm::vec3 & center, const std::vector<glm::vec3>& axisLenVec, int maxBit);

};