#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int		iCurrentIndex = -1;		

	}NAVIDESC;

public:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_bool		isMove_OnNavigation(_fvector TargetPos);
	
	HRESULT		Init_ProtoClone(const _tchar * pNavigationDataFilePath);
	HRESULT		DeleteCell(_uint iCellNum);

	_int		isPacking_NaviCell(HWND hWnd, _uint iWinsizeX, _uint iWinsizeY);

	_float		Height_OnNavigation(_fvector vPos);

#ifdef _DEBUG
public:
	HRESULT		Render();
	void		SetPickingIndex(_int iIndex);
#endif

private:
	vector<class CCell*>		m_Cells;
	NAVIDESC					m_NaviDesc;

private:
	HRESULT Ready_Neighbor();

#ifdef _DEBUG
private:
	class CShader*				m_pShader = nullptr;
	_int						m_iPickingIndex = 0;
#endif 

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFilePath);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();
};

END