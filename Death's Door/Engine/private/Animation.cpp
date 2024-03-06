#include "..\public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
	: m_isLooping(true)
{
}

HRESULT CAnimation::Initialize(aiAnimation * pAIAnimation, CModel * pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;

	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pAINodeAnim, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_Bones(_double TimeDelta, _double AnimSpped)
{
	/*if (true == m_isFinished &&
		false == m_isLooping)
	{
		return;
	}

	m_PlayTime += m_TickPerSecond * TimeDelta;

	if (m_PlayTime >= m_Duration)
	{
		m_PlayTime = 0.0;
		m_isFinished = true;
		m_AnimFinish = true;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished)
			m_Channels[i]->Reset_KeyFrameIndex();

		m_Channels[i]->Update_TransformMatrix(m_PlayTime);
	}

	if (true == m_isFinished)
		m_isFinished = false;*/

	if (true == m_isFinished &&
		false == m_isLooping)
	{
		return false;
	}

	if (false == m_LerpFinish)
		m_PlayTime += TimeDelta * m_TickPerSecond * AnimSpped;

	if (m_PlayTime >= m_Duration)
	{
		m_PlayTime = 0.f;
		m_isFinished = true;
		return true;
	}

	if (m_isLooping && m_isFinished)
	{
		for (auto iter : m_Channels)
			iter->Reset_FrameIndex();
	}

	for (auto iter : m_Channels)
	{
		iter->Update_TransformMatrix(m_PlayTime);
	}

	if (m_isFinished = true)
		m_isFinished = false;

	return false;
}

_bool CAnimation::LerpTime(_double TimeDelta, CAnimation * NextAnim, _double LerpSpeed, _bool bFinish/*, _bool bLerpReset*/)
{
	//Lerp타임중에 애매하니깐 그냥 초기화로 만드는것인데 굳이 필요하지는않다
	/*if (bLerpReset)
	{
		for (auto iter : m_Channels)
		{
			iter->Reset_LerpTime();
		}
	}*/
	
	if (m_LerpFinish)
	{
		m_PlayTime = 0.f;

		for (auto iter : m_Channels)
		{
			// 애니메이션이 다 돌았다는 거니깐 FrameIndex는 0으로 초기화 시켜주고, LerpIndex는 -1로 초기화를 시켜준다
			iter->Reset_FrameIndex();
			iter->Reset_LerpIndex();
		}

		m_LerpFinish = false;

		return false;
	}
	else
	{
		for (auto Current : m_Channels)
		{
			for (auto Next : NextAnim->m_Channels)
			{
				// Current 현재 애니메이션, Next 다음 애니메이션 이름의 저장한다
				if (!strcmp(Current->Get_ChannelName(), Next->Get_ChannelName()))
				{
					m_LerpFinish = Current->Update_LerbTransformMatrix(m_PlayTime, Current, Next, LerpSpeed, m_isFinished);
					break;
				}
			}
		}

		return true;
	}

	return true;
}

_bool CAnimation::isFinish(void)
{
	_bool bResult = m_AnimFinish;

	m_AnimFinish = false;

	return bResult;
}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, CModel* pModel)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto iter : m_Channels)
		Safe_Release(iter);
	m_Channels.clear();
}