#include "sphpch.h"

#include "FrameTimeData.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
    namespace Profiling
    {
        FrameTimeData::FrameTimeData()
			: m_currentIndex(0)
		{
        }

        FrameTimeData::~FrameTimeData()
        {
        }

        const float* FrameTimeData::GetFrameTimeData() const
        {
            return m_frameTimeData;
        }

        const float* FrameTimeData::GetGameTimeData() const
        {
            return m_gameTimeData;
        }

        const size_t FrameTimeData::GetFrameTimeDataSize() const
        {
            return m_currentIndex;
        }

        void FrameTimeData::Update(float _frameTime, float _gameTime)
        {
            m_frameTimeData[m_currentIndex] = _frameTime;
            m_gameTimeData[m_currentIndex] = _gameTime;
            m_currentIndex++;

            if (m_currentIndex >= m_maxSize)
            {
                // Move all elements one step to the left
                std::copy(m_frameTimeData + 1, m_frameTimeData + m_maxSize, m_frameTimeData);
                std::copy(m_gameTimeData + 1, m_gameTimeData + m_maxSize, m_gameTimeData);
                m_currentIndex = m_maxSize - 1;
            }
        }
    }
}
