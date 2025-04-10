#pragma once
#include <DirectXMath.h>
#include <vector>

const float Pi = 3.1415926535f;

using namespace DirectX;

struct TRANSFORM
{
	XMFLOAT3 mvPosition;
	XMFLOAT3 mvScaling;

	XMFLOAT3 mvRight;
	XMFLOAT3 mvUp;
	XMFLOAT3 mvForward;
	XMFLOAT3 mvOrigin;

	XMFLOAT4 mqRotation;
	XMFLOAT4X4 mmRotation;

	XMFLOAT4X4 mmMatrix;
	bool mIsUpdated = true;

	void UpdateMatrix();
public:
	TRANSFORM();
	TRANSFORM(
		const XMFLOAT3& _position,
		const XMFLOAT3& _rotationYawPitchRoll,
		const XMFLOAT3& _scaling
	);

	const XMFLOAT3& GetPositionFLOAT() const;
	XMVECTOR GetPosition();
	void SetPosition(const XMFLOAT3& _vec);
	void XM_CALLCONV SetPosition(FXMVECTOR _vec);
	void OffsetPosition(const XMFLOAT3& _vec);
	void XM_CALLCONV OffsetPosition(FXMVECTOR _vec);

	const XMFLOAT4& GetRotationFLOAT() const;
	XMVECTOR GetRotation();
	void SetRotationQuaternion(const XMFLOAT4& _vec);
	void XM_CALLCONV SetRotationQuaternion(FXMVECTOR _vec);
	void SetRotationYPR(const XMFLOAT3& _ypr);
	void XM_CALLCONV SetRotationYPR(FXMVECTOR _ypr);

	const XMFLOAT3& GetScalingFLOAT() const;
	XMVECTOR GetScaling();
	void SetScaling(const XMFLOAT3& _vec);
	void XM_CALLCONV SetScaling(FXMVECTOR _vec);

	const XMFLOAT4X4& GetMatrixFLOAT();
	XMMATRIX GetMatrix();

	XMFLOAT3 Forward();
	XMFLOAT3 Right();
	XMFLOAT3 Up();

	XMMATRIX operator * (TRANSFORM& _other);
	XMMATRIX XM_CALLCONV operator * (FXMMATRIX _other);
	XMVECTOR operator * (const XMFLOAT3& _other);

	void Reset();
};