#pragma once

#include "Client_Defines.h"
#include "ImGui_Console.h"
#include <sstream>

BEGIN(Client)

class CObject_Builder : public CImGui_Console
{
public:
	enum OBJECTTYPE { OBJECT_MONSTER, OBJECT_DOOR, OBJECT_GOUND, OBJECT_STAIRS, OBJECT_STAIRS02, OBJECT_FANCE, OBJECT_FANCEBAR,
					  OBJECT_WALL03, OBJECT_WALL04, OBJECT_PILLAR, OBJECT_CUBEBOX, OBJECT_CUBE, OBJECT_FLOOR, OBJECT_END
	};

public:
	CObject_Builder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CObject_Builder() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

private:
	void			Create_Cube(_uint eType);

private:
	_float4			m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);
	_float3			m_vScale = _float3(0.f, 0.f, 0.f);
	_bool			m_bCreate;
	char			m_szObjectName[MAX_PATH] = "";

public:
	static CObject_Builder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END