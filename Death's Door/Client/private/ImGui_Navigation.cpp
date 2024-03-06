#include "stdafx.h"
#include "..\public\ImGui_Navigation.h"
#include "Terrain.h"
#include "Cell.h"
#include "Navigation.h"
#include "VIBuffer_Cell.h"
#include "GameObject.h"
#include "NaviCube.h"

CImGui_Navigation::CImGui_Navigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CImGui_Navigation::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bCreateCell = false;
	m_bCreateCube = false;
	m_bCheckDelete = false;
	
	return S_OK;
}

void CImGui_Navigation::Show_Window(void)
{
	ImGui::Begin("Navigation");
	void BeginFrame();

	ImGui::PushItemWidth(150);

	ENGINE_OPEN;

	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY && m_NaviCom == nullptr)
	{
		m_NaviCom = static_cast<CTerrain*>(ENGINE->Get_GameObject(
			LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"),
			TEXT("GameObject_Terrain")))
			->Get_NaviCom();
	}

	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY)
	{
		m_vPickingCell = static_cast<CTerrain*>(ENGINE->Get_GameObject(
			LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"),
			TEXT("GameObject_Terrain")))
			->Get_Pick();
	}

	ImGui::Checkbox("CreateCube", &m_bCreateCube);
	ImGui::SameLine();
	ImGui::Checkbox("CreateCell", &m_bCreateCell);
		
	ImGui::InputText("CellName", m_szCellName, MAX_PATH);

	if (ImGui::Button("SaveCell"))
	{
		_tchar szName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCellName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFile(szName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD	dwByte = 0;

		for (auto& iter : m_vPoint)
			m_PointList.push_back(iter);

		for (auto& iter : m_PointList)
		{
			WriteFile(hFile, &iter, sizeof(_float3), &dwByte, nullptr);
		}

		CloseHandle(hFile);
		MSG_BOX("SaveCell_Complete");
	}

	ImGui::SameLine();
	
	if (ImGui::Button("LoadCell"))
	{
		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCellName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		_ulong		dwByte = 0;
		HANDLE		hFile = CreateFileW(szName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& iter : m_PointList)
			m_PointList.clear();

		_float3		vPoints;

		while (true)
		{
			ReadFile(hFile, &vPoints, sizeof(_float3), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			m_PointList.push_back(vPoints);
		}
		CloseHandle(hFile);
		MSG_BOX("LoadCell_Complete");
	}

	ImGui::InputText("CubeName", m_szCubeName, MAX_PATH);

	if (ImGui::Button("SaveCube"))
	{
		_tchar szName[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCubeName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		DWORD		dwByte = 0;
		HANDLE		hFile = CreateFileW(szName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& iter : m_CellCube)
		{
			_vector vAsve = static_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_TRANSLATION);
			WriteFile(hFile, &vAsve, sizeof(_vector), &dwByte, nullptr);
		}

		CloseHandle(hFile);
		MSG_BOX("SaveCube_Complete");
	}

	ImGui::SameLine();

	if (ImGui::Button("LoadCube"))
	{
		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCubeName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_tchar szPath[MAX_PATH] = TEXT("../Bin/Data/");

		lstrcat(szPath, szName);
		lstrcpy(szName, szPath);
		lstrcat(szName, TEXT(".dat"));

		HANDLE		hFile = CreateFile(szName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& iter : m_CellCube)
			Safe_Release(iter);
		m_CellCube.clear();

		DWORD	dwByte = 0;

		while (true)
		{
			_vector vPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
			ReadFile(hFile, &vPosition, sizeof(_vector), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject*		pCellCube = nullptr;

			pCellCube = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_NaviCube"));
			if (nullptr == pCellCube)
				return;

			static_cast<CTransform*>(pCellCube->Get_Component(TEXT("Com_Transform")))->
				Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPosition), XMVectorGetZ(vPosition), 1.f));

			m_CellCube.push_back(pCellCube);
		}

		CloseHandle(hFile);
		MSG_BOX("LoadCube_Complete");
	}

	for (auto& iter : m_CellCube)
	{
		iter->Tick(0.f);
		iter->Late_Tick(0.f);
		//iter->Render();
	}

	ImGui::Checkbox("DeleteCell", &m_bCheckDelete);

	_int iPickingIndex = -1;
	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY && m_NaviCom != nullptr)
	{
		_uint iNum = 1;
		D3D11_VIEWPORT ViewPort;
		m_pContext->RSGetViewports(&iNum, &ViewPort);

		iPickingIndex = m_NaviCom->isPacking_NaviCell(ENGINE->GetHWND(), ViewPort.Width, ViewPort.Height);
		ImGui::Text("iPickingIndex : %d", iPickingIndex);

		if (ENGINE->Key_Down(DIK_P))
		{
			m_iDeleteCellIndex = iPickingIndex;
		}
#ifdef _DEBUG
		m_NaviCom->SetPickingIndex(iPickingIndex);
#endif // _DEBUG
	}

	ImGui::Text("DeleteCellIndex : %d", m_iDeleteCellIndex);
	if (m_bCheckDelete && ENGINE->Mouse_Down(DIM_LB))
	{
		if (-1 != m_iDeleteCellIndex)
		{
			m_NaviCom->DeleteCell(m_iDeleteCellIndex);
		}
	}

	if (ImGui::Button("Undo"))
	{
		if (!m_PointList.empty())
		{
			_uint iDeleteCellIndex = (m_PointList.size() / 3) - 1;

			m_NaviCom->DeleteCell(iDeleteCellIndex);
			m_PointList.pop_back();
			m_PointList.pop_back();
			m_PointList.pop_back();

			_ulong		dwByte = 0;
			HANDLE		hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			for (auto& iter : m_PointList)
			{
				WriteFile(hFile, &iter, sizeof(_float3), &dwByte, nullptr);
			}

			CloseHandle(hFile);

			m_NaviCom->Init_ProtoClone(TEXT("../Bin/Data/Navigation.dat"));

			m_iCurIndex = 0;
		}
	}

	Peeking();
	Connect_Cells();
	Create_Izmo();

	if (m_bCreateCube || m_bCreateCell)
		m_iCurrentIndex = -1;

	if (-1 != m_iCurrentIndex)
		ImGuizmo_Setting();

	ENGINE_CLOSE;

	ImGui::End();
}

void CImGui_Navigation::Peeking()
{
	ENGINE_OPEN;

	if (m_bCreateCube && ENGINE->Key_Pressing(DIK_V) && ENGINE->Mouse_Down(DIM_LB))
	{
		Create_NaviCube(TEXT("NaviCube"));
	}
	
	ENGINE_CLOSE;
}

void CImGui_Navigation::Create_NaviCube(const _tchar * LayerTag)
{
	ENGINE_OPEN;

	CGameObject*		pCellCube = nullptr;

	pCellCube = ENGINE->Clone_NoGameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_NaviCube"));
	if (nullptr == pCellCube)
		return;

	static_cast<CTransform*>(pCellCube->Get_Component(TEXT("Com_Transform")))->
		Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_vPickingCell.x, m_vPickingCell.y, m_vPickingCell.z, 1.f));

	m_CellCube.push_back(pCellCube);

	ENGINE_CLOSE;

	return;
}

void CImGui_Navigation::Connect_Cells(void)
{
	ENGINE_OPEN;

	if (m_bCreateCell && ENGINE->Mouse_Down(DIM_LB))
	{
		for (auto& iter : m_CellCube)
		{
			if (static_cast<CNaviCube*>(iter)->isPeeking())
			{
				_vector vPos =
				static_cast<CTransform*>
					(iter->Get_Component(TEXT("Com_Transform")))
					->Get_State(CTransform::STATE_TRANSLATION);

				m_vPoint[m_iCurIndex++] =
					_float3(XMVectorGetX(vPos),
						XMVectorGetY(vPos),
						XMVectorGetZ(vPos));
			}
		}

		if (m_iCurIndex >= 3)
		{
			_ulong		dwByte = 0;
			HANDLE		hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			//Sorting
			const _vector vPointA = XMLoadFloat3(&m_vPoint[0]);
			const _vector vPointB = XMLoadFloat3(&m_vPoint[1]);
			const _vector vPointC = XMLoadFloat3(&m_vPoint[2]);

			const _vector vAB = vPointB - vPointA;
			const _vector vBC = vPointC - vPointB;

			_float3 vCross;
			XMStoreFloat3(&vCross, XMVector3Cross(vAB, vBC));

			if (vCross.y < 0)
			{
				_float3 temp = m_vPoint[1];
				m_vPoint[1] = m_vPoint[2];
				m_vPoint[2] = temp;
			}

			for (auto& iter : m_vPoint)
				m_PointList.push_back(iter);

			for (auto& iter : m_PointList)
			{
				WriteFile(hFile, &iter, sizeof(_float3), &dwByte, nullptr);
			}

			CloseHandle(hFile);

			m_NaviCom->Init_ProtoClone(TEXT("../Bin/Data/Navigation.dat"));
			
			m_iCurIndex = 0;
		}
	}

	ENGINE_CLOSE;
}

void CImGui_Navigation::Create_Izmo()
{
	ENGINE_OPEN;

	if (!m_bCreateCell && !m_bCreateCube && ENGINE->Mouse_Down(DIM_LB))
	{
		_uint iCount = 0;

		for (auto& iter : m_CellCube)
		{
			if (static_cast<CNaviCube*>(iter)->isPeeking())
			{
				m_iCurrentIndex = iCount;
				break;
			}
			else
				iCount++;
		}
	}

	ENGINE_CLOSE;
}

void CImGui_Navigation::ImGuizmo_Setting()
{
	ENGINE_OPEN; 

	_float4x4	WorldMatrix = static_cast<CTransform*>(m_CellCube[m_iCurrentIndex]->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix();

	ImGuizmo::BeginFrame();
	static float snap[3] = { 1.f, 1.f, 1.f };
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(reinterpret_cast<float*>(&WorldMatrix), matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Translate", matrixTranslation);
	ImGui::InputFloat3("Rotate", matrixRotation);
	ImGui::InputFloat3("Scale", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, reinterpret_cast<float*>(&WorldMatrix));

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("##something", &useSnap);
	ImGui::SameLine();
	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	ImGuiIO& io = ImGui::GetIO();
	RECT rt;
	GetClientRect(ENGINE->GetHWND(), &rt);
	POINT lt{ rt.left, rt.top };
	ClientToScreen(ENGINE->GetHWND(), &lt);
	ImGuizmo::SetRect((_float)lt.x, (_float)lt.y, io.DisplaySize.x, io.DisplaySize.y);

	_float4x4 matView, matProj;
	XMStoreFloat4x4(&matView, ENGINE->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	XMStoreFloat4x4(&matProj, ENGINE->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	ImGuizmo::Manipulate(
		reinterpret_cast<float*>(&matView),
		reinterpret_cast<float*>(&matProj),
		mCurrentGizmoOperation,
		mCurrentGizmoMode,
		reinterpret_cast<float*>(&WorldMatrix),
		nullptr, useSnap ? &snap[0] : nullptr);

	static_cast<CTransform*>(m_CellCube[m_iCurrentIndex]->Get_Component(TEXT("Com_Transform")))->Set_WorldMatrix(WorldMatrix);

	ENGINE_CLOSE;
}

CImGui_Navigation * CImGui_Navigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CImGui_Navigation* pInstance = new CImGui_Navigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failled to Create : CImGui_Navigation");
		Safe_Release(pInstance);
	}
 
	return pInstance;
}

void CImGui_Navigation::Free()
{
	__super::Free();

	for (auto& CellCube : m_CellCube)
		Safe_Release(CellCube);

	m_CellCube.clear();

	/*for (auto& Points : m_PointList)
		Safe_Release(Points);

	m_PointList.clear();*/

	Safe_Release(m_Cell);
	Safe_Release(m_NaviCom);
}
