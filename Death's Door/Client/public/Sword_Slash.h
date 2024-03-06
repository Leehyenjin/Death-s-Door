#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engin)
class CShader;
class CRenderer;
class CModel;
class CBone;
class CCollider;
END

BEGIN(Client)

class CSword_Slash : public CGameObject
{
public:
	typedef struct tagSlashDesc
	{
		_float4x4		PivotMatrix;
		CBone*			pSocket;
		CTransform*		pTargetTransform;

	}SLASHDESC;

public:
	CSword_Slash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSword_Slash(const CSword_Slash& rhs);
	virtual ~CSword_Slash() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;

	SLASHDESC		m_SlashDesc;

	_float4x4		m_SocketMatrix;

public:
	CCollider*		Get_ColliderWeapon() { return m_pColliderCom; }

	_bool			m_isRender = false;

	_bool			Get_IsRender() { return m_isRender; }
	void			Set_IsRender(_bool isCheck) { m_isRender = isCheck; }

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CSword_Slash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END