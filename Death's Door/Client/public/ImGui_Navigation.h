#pragma once

#include "Client_Defines.h"
#include "ImGui_Console.h"

BEGIN(Client)

class CImGui_Navigation : public CImGui_Console
{
public:
	CImGui_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Navigation() = default;

public:
	virtual HRESULT Initialize(void);
	virtual void	Show_Window(void);

private:
	void	Peeking();
	void	Create_NaviCube(const _tchar* LayerTag);
	void	Connect_Cells(void);
	void	Create_Izmo();
	void	ImGuizmo_Setting();

private:
	vector<class CGameObject*>				m_CellCube;
	
	class CNavigation*						m_NaviCom;
	class CCell*							m_Cell = nullptr;

private:
	_float4				m_vPickingCell = _float4(0.f, 0.f, 0.f, 1.f);
	
	list<_float3>		m_PointList;
	_float3				m_vPoint[3];

	_uint				m_iCurIndex = 0;
	_int				m_iCurrentIndex = -1;

	_bool				m_bCreateCell;
	_bool				m_bCreateCube;
	_bool				m_bCheckDelete;

	char				m_szCellName[MAX_PATH] = "FinalNavigation";
	char				m_szCubeName[MAX_PATH] = "TestCube";

	_int				m_iDeleteCellIndex = -1;

public:
	static CImGui_Navigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END