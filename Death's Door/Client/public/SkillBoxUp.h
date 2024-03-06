#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSkillBoxUp : public CUI
{
public:
	enum BOXTYPE { TYPE_BOX0, TYPE_BOX1, TYPE_BOX2, TYPE_BOX3, TYPE_END };

public:
	CSkillBoxUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillBoxUp(const CSkillBoxUp& rhs);
	virtual ~CSkillBoxUp() = default;

public:
	virtual HRESULT Initialize_Prototype(OBJECT_ID eID, _uint eUIType);
	virtual HRESULT Initialize(void* pArg);
	virtual _uint	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	SetUp_Component();

private:
	CUI::UI_DESC			m_UIDesc;				

	UINUMBER				m_UINumber;

	BOXTYPE					m_eType = TYPE_END;

public:
	static CSkillBoxUp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJECT_ID eID, _uint eUIType);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free();
};

END