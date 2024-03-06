#pragma once

#include "Component.h"

BEGIN(Engine)

// Model�� �ε� �� �� �ؽ��ĵ� ���� �ε��ҰŴ� Model Component�� Texture Component�� ���� ���·� ���� �� �̴�
class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_matrix Get_PivotMatrix() const {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}

	_float4x4 Get_PivotFloat4x4() const {
		return m_PivotMatrix;
	}

	// Ư�� ���� �˻��ؼ� ã�� �Լ�
	class CBone* Get_BonePtr(const char* pBoneName);
	class CAnimation* Get_AnimIndex(_uint AnimIndex);

public:
	void Set_AnimIndex(_uint iAnimIndex) {
		m_iCurrentAnimIndex = iAnimIndex;
	}
	_bool	isFinish(void);

	_bool	Get_isFinish() { return m_bAnimFinish; }
	void	Reset_isFinish() { m_bAnimFinish = false; }

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	void Play_Animation(_double TimeDelta, _double LerpSpeed = 0.3f, _double AnimSpeed = 1.f, _bool bFinish = false);		
	void Set_Animation(_uint iAnimation);																					

	HRESULT Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const char* pConstantName);
	HRESULT Render(CShader* pShader, _uint iMeshIndex, const char* pBoneConstantName = nullptr, _uint iPassIndex = 0);

public:
	const aiScene*				m_pAIScene = nullptr;

	Assimp::Importer			m_Importer;
	TYPE						m_eType = TYPE_END;

	// �ϳ��� ���� ��ü�� ������ �������� �޽÷� �����Ǿ��ִ�
	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_Materials;

	// ��ü ���� ����
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_Bones;			// �� ���� ��ӱ����� �޴´�

	_uint						m_iCurrentAnimIndex = 0;		// Ŭ���̾�Ʈ�� ����� �� �̴�
	_uint						m_iNumAnimations = 0;
	vector<class CAnimation*>	m_Animations;

	_float4x4					m_PivotMatrix;

	_bool						m_bLerpReset = false;
	_bool						m_bChanged = false;
	_bool						m_bAnimFinish = false;
	_int						m_iNextAnimation = -1;

public:
	HRESULT Ready_Bones(aiNode* pNode, class CBone* pParent);
	HRESULT Ready_MeshContainers();
	HRESULT	Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animation();			

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END