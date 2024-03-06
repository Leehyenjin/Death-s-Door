#include "stdafx.h"
#include "..\public\Object_Builder.h"
#include "GameObject.h"
#include "Terrain.h"

CObject_Builder::CObject_Builder(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CObject_Builder::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bCreate = false;

	return S_OK;
}

void CObject_Builder::Show_Window(void)
{
	ImGui::Begin("Object_Create");
	void BeginFrame();

	ImGui::PushItemWidth(200);

	ENGINE_OPEN;

	if (ENGINE->Get_NowLevel() == LEVEL_GAMEPLAY)
	{
		m_vPickingPos = static_cast<CTerrain*>(ENGINE->Get_GameObject(
			LEVEL_GAMEPLAY,
			TEXT("Layer_BackGround"),
			TEXT("GameObject_Terrain")))
			->Get_Pick();

		ImGui::InputFloat4("MousePos", &m_vPickingPos.x);
		ImGui::InputFloat3("Scale", &m_vScale.x);
		ImGui::InputText("Name", m_szObjectName, MAX_PATH);

		static int Object = OBJECT_END;

		if (ImGui::CollapsingHeader("Monseter", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Archer", &Object, OBJECT_MONSTER);
		}

		if (ImGui::CollapsingHeader("Map", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Stage01", &Object, OBJECT_GOUND);
		}

		if (ImGui::CollapsingHeader("Ect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Test", &Object, OBJECT_DOOR);
			ImGui::RadioButton("Fance", &Object, OBJECT_FANCE);
			ImGui::RadioButton("FanceBar", &Object, OBJECT_FANCEBAR);
			ImGui::RadioButton("Pillar", &Object, OBJECT_PILLAR);
			ImGui::RadioButton("CubeBox", &Object, OBJECT_CUBEBOX);
		}

		if (ImGui::CollapsingHeader("Wall", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Stairs", &Object, OBJECT_STAIRS);
			ImGui::RadioButton("Stairs02", &Object, OBJECT_STAIRS02);
			ImGui::RadioButton("Wall03", &Object, OBJECT_WALL03);
			ImGui::RadioButton("Wall04", &Object, OBJECT_WALL04);
			ImGui::RadioButton("Floor", &Object, OBJECT_FLOOR);
		}

		if (ImGui::CollapsingHeader("Create_A", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("NO", &Object, OBJECT_END);
		}
		Create_Cube(Object);
	}
	else
	{
		ImGui::Text("Faill!!!!");
	}
	
	ImGui::Checkbox("Create", &m_bCreate);

	ENGINE_CLOSE;

	ImGui::End();
}

void CObject_Builder::Create_Cube(_uint eType)
{
	ENGINE_OPEN;

	_float fCollision = m_vPickingPos.x + m_vPickingPos.y + m_vPickingPos.z;

	if (m_bCreate && (fCollision > 3.f) && ENGINE->Mouse_Down(DIM_LB))
	{
		if (!strcmp(m_szObjectName, ""))
		{
			ImGui::Checkbox("Cube_Create", &m_bCreate);
			ENGINE_CLOSE;
			ImGui::End();
			return;
		}

		_tchar* szName = new _tchar[MAX_PATH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szObjectName, sizeof(char[MAX_PATH]), szName, sizeof(_tchar[MAX_PATH]));
		TcharList.push_back(szName);

		_float4 fPos = _float4(m_vPickingPos.x, m_vPickingPos.y /*+ (m_vScale.y * 0.5f)*/, m_vPickingPos.z, 1.f);

		switch (eType)
		{
		case OBJECT_DOOR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Door"), szName);
			break;
		}
		case OBJECT_GOUND:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_StageMap"), szName);
			break;
		}
		case OBJECT_STAIRS:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Stairs"), szName);
			break;
		}
		case OBJECT_STAIRS02:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Stairs02"), szName);
			break;
		}
		case OBJECT_FANCE:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Fance"), szName);
			break;
		}
		case OBJECT_FANCEBAR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_FanceBar"), szName);
			break;
		}
		case OBJECT_WALL03:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Wall03"), szName);
			break;
		}
		case OBJECT_WALL04:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Wall04"), szName);
			break;
		}
		case OBJECT_PILLAR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Pillar"), szName);
			break;
		}
		case OBJECT_CUBEBOX:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_CubeBox"), szName);
			break;
		}
		case OBJECT_FLOOR:
		{
			ENGINE->Clone_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), TEXT("Prototype_GameObject_Floor"), szName);
			break;
		}
		case OBJECT_END:
			break;
		}

		if (nullptr != ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), szName))
		{
			static_cast<CTransform*>(
				ENGINE->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Game_Play_Layer_Object"), szName)
				->Get_Component(CGameObject::m_pTransformComTag))
				->Set_Scaling(m_vScale).Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fPos));	
		}

		m_bCreate = false;
		strcpy_s(m_szObjectName, "");
	}

	ENGINE_CLOSE;
}

CObject_Builder * CObject_Builder::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CObject_Builder* pInstance = new CObject_Builder(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CObject_Builder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Builder::Free()
{
	__super::Free();
}
