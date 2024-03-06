#pragma once

#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)

class ENGINE_DLL CUI : public CGameObject
{
public:
	typedef struct tagUIDescription : public GAMEOBJECTDESC
	{
		_float4	UI_Size, UI_Position;
		_uint UI_Number;

	}UI_DESC;

	enum UITYPE { UI_GUAGE, UI_WINDOW, UI_IMAGE, UI_BACKGROUND, UI_END };

	enum UINUMBER { UI_1, UI_2, UI_3, UI_4, UINUMBER_END };

public:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	UITYPE					m_eType;
	UINUMBER				m_eTypeNumber = UINUMBER_END;

	CUI::UI_DESC			m_UIDesc;

protected:
	_float4x4				m_matView, m_matProj;
	HRESULT					Setup_ShaderResources(void);
	
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_Rect*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg = nullptr)PURE;
	virtual void Free();
};

END