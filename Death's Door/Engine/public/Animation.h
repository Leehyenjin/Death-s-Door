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
	char						m_szName[MAX_PATH];					// 현재 애니메이션을 상태의 이름 ex) Attack, Walk같은 애니메이션 상태의 이름
	_double						m_Duration = 0.0;					// 애니메이션 총 재생 시간(애니메이션을 재생하는데 걸리는 전체 시간)
	_double						m_TickPerSecond = 0.0;				// TimeDelta를 곱해서 Duration보다 커지면 종료 공격 모션같은 시간 조절을 하는 기능(초당 재생해야할 애니메이션의 속도이다)
	_double						m_PlayTime = 0.0;

	_bool						m_isFinished = false;				// 애니메이션이 끝났나의 판단 여부
	_bool						m_isLooping = true;					// 무한으로 돈단 안돈다의 판단 여부

	_uint						m_iNumChannels = 0;					// 이 애니메이션을 재생하기위해 갱신해야하는 뼈들
	vector<class CChannel*>		m_Channels;

	_bool						m_AnimFinish = false;				
	_bool						m_LerpFinish = false;				// LerpEnd

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void Free() override;
};

END