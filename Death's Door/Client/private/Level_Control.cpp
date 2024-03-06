#include "stdafx.h"
#include "..\public\Level_Control.h"

#include "Level_Loading.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"

CLevel_Control::CLevel_Control(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CImGui_Console(pDevice, pContext)
{
}

HRESULT CLevel_Control::Initialize(void)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Control::Show_Window(void)
{
	

	ImGui::Begin("Level_Control");
	void BeginFrame();

	ImGui::PushItemWidth(200);
	//ImGui::SetWindowPos(ImVec2(100.f, 100.f));
	ENGINE_OPEN;

	const char* items[4];
	items[0] = "Logo";
	items[1] = "Game_Play";
	items[2] = "Game_Play1";
	items[3] = "Game_Play2";

	static int item_current = 0;
	ImGui::ListBox("Sence", &item_current, items, IM_ARRAYSIZE(items), 4);

	switch (ENGINE->Get_NowLevel())
	{
	case LEVEL_LOADING:
		ImGui::Text("Sence : LOADING");
		break;

	case LEVEL_LOGO:
		ImGui::Text("Sence : LOGO");
		break;

	case LEVEL_GAMEPLAY:
		ImGui::Text("Sence : GAMEPLAY");
		break;

	case LEVEL_GAMEPLAY1:
		ImGui::Text("Sence : GAMEPLAY1");
		break;

	case LEVEL_GAMEPLAY2:
		ImGui::Text("Sence : GAMEPLAY2");
		break;
	}

	ImGui::Text("Selected : %s", items[item_current]);

	if (ImGui::Button("Go"))
	{
		if (0 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_LOGO));
		if (1 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_GAMEPLAY));
		if (2 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_GAMEPLAY1));
		if (3 == item_current)
			ENGINE->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING, LEVEL_GAMEPLAY2));
	}

	ENGINE_CLOSE;

	ImGui::End();

	__super::Show_Window();
}

CLevel_Control * CLevel_Control::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Control* pInstance = new CLevel_Control(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CLevel_Control");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Control::Free()
{
	__super::Free();

	/*for_each(m_ListBox.begin(), m_ListBox.end(), Safe_Delete_Array<char*>);
	m_ListBox.clear();*/
}
