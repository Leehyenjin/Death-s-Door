#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	_bool Update_Bones(_double TimeDelta, _double AnimSpeed);

public:
	_bool	LerpTime(_double TimeDelta, CAnimation* NextAnim, _double LerpSpeed, _bool bFinish = false/*, _bool bLerpReset = false*/);
	_bool	isFinish(void);

	_double Get_PlayRate() { return m_PlayTime / m_Duration; }

private:
	char						m_szName[MAX_PATH];					// ���� �ִϸ��̼��� ������ �̸� ex) Attack, Walk���� �ִϸ��̼� ������ �̸�
	_double						m_Duration = 0.0;					// �ִϸ��̼� �� ��� �ð�(�ִϸ��̼��� ����ϴµ� �ɸ��� ��ü �ð�)
	_double						m_TickPerSecond = 0.0;				// TimeDelta�� ���ؼ� Duration���� Ŀ���� ���� ���� ��ǰ��� �ð� ������ �ϴ� ���(�ʴ� ����ؾ��� �ִϸ��̼��� �ӵ��̴�)
	_double						m_PlayTime = 0.0;

	_bool						m_isFinished = false;				// �ִϸ��̼��� �������� �Ǵ� ����
	_bool						m_isLooping = true;					// �������� ���� �ȵ����� �Ǵ� ����

	_uint						m_iNumChannels = 0;					// �� �ִϸ��̼��� ����ϱ����� �����ؾ��ϴ� ����
	vector<class CChannel*>		m_Channels;

	_bool						m_AnimFinish = false;				
	_bool						m_LerpFinish = false;				// LerpEnd

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void Free() override;
};

END