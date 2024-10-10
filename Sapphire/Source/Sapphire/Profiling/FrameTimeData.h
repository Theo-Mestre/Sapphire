#ifndef SPH_FRAME_TIME_DATA_H
#define SPH_FRAME_TIME_DATA_H

namespace sph
{
	namespace Profiling
	{
		class FrameTimeData
		{
		public:
			FrameTimeData();
			~FrameTimeData();

			const float* GetFrameTimeData() const;
			const float* GetGameTimeData() const;
			const size_t GetFrameTimeDataSize() const;

			void Update(float _frameTime, float _gameTime);
		private:
			static constexpr uint32_t m_maxSize = 2000;
			float m_frameTimeData[m_maxSize + 1] = { 0 };
			float m_gameTimeData[m_maxSize + 1] = { 0 };
			uint32_t m_currentIndex = 0;
		};
	}
}
#endif