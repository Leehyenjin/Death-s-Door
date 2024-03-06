#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CNaviCube : public CGameObject
{
public:
	CNaviCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNaviCube(const CNaviCube& rhs);
	virtual ~CNaviCube() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_bool	isPeeking(void);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Square*	m_pVIBufferCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

public:
	static CNaviCube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();

};

END