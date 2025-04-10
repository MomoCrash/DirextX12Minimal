#include "Transform.h"

void TRANSFORM::UpdateMatrix()
{
	XMStoreFloat4x4(
		&mmMatrix,
		XMMatrixAffineTransformation(
			GetScaling(),
			XMLoadFloat3(&mvOrigin),
			GetRotation(),
			GetPosition()
		)
	);
	mIsUpdated = true;
}

TRANSFORM::TRANSFORM()
{
	Reset();
}

TRANSFORM::TRANSFORM(const XMFLOAT3& _position, const XMFLOAT3& _rotationYawPitchRoll, const XMFLOAT3& _scaling)
{

}


const XMFLOAT3& TRANSFORM::GetPositionFLOAT() const
{
    return mvPosition;
}

XMVECTOR TRANSFORM::GetPosition()
{
    return XMLoadFloat3(&mvPosition);
}

void TRANSFORM::SetPosition(const XMFLOAT3& _vec)
{
	mvPosition = _vec;
	mIsUpdated = false;
}

void XM_CALLCONV TRANSFORM::SetPosition(FXMVECTOR _vec)
{
	XMStoreFloat3(
		&mvPosition,
		_vec
	);
	mIsUpdated = false;
}

void TRANSFORM::OffsetPosition(const XMFLOAT3& _vec)
{
	XMFLOAT3 pos = GetPositionFLOAT();
	pos.x += _vec.x;
	pos.y += _vec.y;
	pos.z += _vec.z;
	SetPosition(pos);
}

void XM_CALLCONV TRANSFORM::OffsetPosition(FXMVECTOR _vec)
{
	SetPosition(GetPosition() + _vec);
}

const XMFLOAT4& TRANSFORM::GetRotationFLOAT() const
{
	return mqRotation;
}

XMVECTOR TRANSFORM::GetRotation()
{
	return XMLoadFloat4(&mqRotation);
}

void TRANSFORM::SetRotationQuaternion(const XMFLOAT4& _vec)
{
	mqRotation = _vec;
	XMVECTOR qRot = XMLoadFloat4(
		&mqRotation
	);
	XMStoreFloat4x4(
		&mmRotation,
		XMMatrixRotationQuaternion(
			qRot
		)
	);

	mvRight		= { mmRotation._11, mmRotation._12, mmRotation._13 };
	mvUp		= { mmRotation._21, mmRotation._22, mmRotation._23 };
	mvForward	= { mmRotation._31, mmRotation._32, mmRotation._33 };
	mIsUpdated = false;
}

void XM_CALLCONV TRANSFORM::SetRotationQuaternion(FXMVECTOR _vec)
{
	XMStoreFloat4(
		&mqRotation,
		_vec
	);
	XMStoreFloat4x4(
		&mmRotation,
		XMMatrixRotationQuaternion(_vec)
	);
	mvRight = { mmRotation._11, mmRotation._12, mmRotation._13 };
	mvUp = { mmRotation._21, mmRotation._22, mmRotation._23 };
	mvForward = { mmRotation._31, mmRotation._32, mmRotation._33 };
	mIsUpdated = false;
}

void TRANSFORM::SetRotationYPR(const XMFLOAT3& _ypr)
{
	XMVECTOR RIGHT		= { 1, 0, 0, 0 };
	XMVECTOR UP			= { 0, 1, 0, 0 };
	XMVECTOR FORWARD	= { 0, 0, 1, 0 };

	XMVECTOR qRot = XMQuaternionRotationAxis(UP, _ypr.x);
	XMVECTOR qRotTemp;

	RIGHT = XMVector3Rotate(RIGHT, qRot);
	qRotTemp = XMQuaternionRotationAxis(RIGHT, _ypr.y);
	qRot = XMQuaternionMultiply(qRot, qRotTemp);

	FORWARD = XMVector3Rotate(FORWARD, qRot);
	qRotTemp = XMQuaternionRotationAxis(FORWARD, _ypr.z);
	qRot = XMQuaternionMultiply(qRot, qRotTemp);

	SetRotationQuaternion(qRot);
}

void XM_CALLCONV TRANSFORM::SetRotationYPR(FXMVECTOR _ypr)
{
	XMFLOAT3 YPR;
	XMStoreFloat3(
		&YPR,
		_ypr
	);
	SetRotationYPR(YPR);
}

const XMFLOAT3& TRANSFORM::GetScalingFLOAT() const
{
	return mvScaling;
}

XMVECTOR TRANSFORM::GetScaling()
{
	return XMLoadFloat3(&mvScaling);
}

void TRANSFORM::SetScaling(const XMFLOAT3& _vec)
{
	mvScaling = _vec;
	mIsUpdated = false;
}

void XM_CALLCONV TRANSFORM::SetScaling(FXMVECTOR _vec)
{
	XMStoreFloat3(
		&mvScaling,
		_vec
	);
	mIsUpdated = false;
}

const XMFLOAT4X4& TRANSFORM::GetMatrixFLOAT()
{
	if (mIsUpdated == false)
		UpdateMatrix();
	return mmMatrix;
}

XMMATRIX TRANSFORM::GetMatrix()
{
	if (mIsUpdated == false)
		UpdateMatrix();
	return XMLoadFloat4x4(&mmMatrix);
}

XMFLOAT3 TRANSFORM::Forward()
{
	return mvForward;
}

XMFLOAT3 TRANSFORM::Right()
{
	return mvRight;
}

XMFLOAT3 TRANSFORM::Up()
{
	return mvUp;
}

XMMATRIX TRANSFORM::operator*(TRANSFORM& _other)
{
	XMMATRIX mat = GetMatrix();
	mat *= _other.GetMatrix();
	return mat;
}

XMMATRIX XM_CALLCONV TRANSFORM::operator*(FXMMATRIX _other)
{
	XMMATRIX mat = GetMatrix();
	mat *= _other;
	return mat;
}

XMVECTOR TRANSFORM::operator*(const XMFLOAT3& _other)
{
	return XMVector3Transform(XMLoadFloat3(&_other), GetMatrix());
}

void TRANSFORM::Reset()
{
	mvPosition = { 0, 0, 0 };
	mvScaling = { 1, 1, 1 };

	mvRight = { 1, 0, 0 };
	mvUp = { 0, 1, 0 };
	mvForward = { 0, 0, 1 };
	mvOrigin = { 0, 0, 0 };

	XMStoreFloat4(
		&mqRotation,
		XMQuaternionIdentity()
	);

	XMStoreFloat4x4(
		&mmRotation,
		XMMatrixIdentity()
	);

	XMStoreFloat4x4(
		&mmMatrix,
		XMMatrixIdentity()
	);
	mIsUpdated = true;
}
