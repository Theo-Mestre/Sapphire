#include "sphpch.h"
#include "Profiler.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Core/Core.h"

namespace sph
{
	namespace Profiling
	{
		Profiler::Profiler()
			: m_recording(false)
			, m_frameCount(0)
			, m_currentFrame(nullptr)
			, m_framesData()
			, m_currentSession(nullptr)
			, m_sessions()
		{
#ifdef SPH_PROFILING_ENABLED
			StartRecording();
#endif
		}

		Profiler::~Profiler()
		{
			SaveData();

			m_framesData.clear();
			m_sessions.clear();
		}

		void Profiler::BeguinFrame()
		{
			m_frameCount++;

			if (IsRecording() == false) return;

			FrameData frameData;
			frameData.FrameNumber = m_frameCount;
			frameData.StartTime = static_cast<float>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
			frameData.EndTime = 0.0f;
			frameData.Results.clear();

			if (m_framesData.empty() == false)
			{
				frameData.Results.reserve(m_framesData.back().Results.size());
			}

			m_currentFrame = &frameData;
			m_framesData.push_back(frameData);
		}

		void Profiler::EndFrame()
		{
			if (IsRecording() == false || m_currentFrame == nullptr) return;

			m_currentFrame->EndTime = static_cast<float>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
			m_currentFrame = nullptr;
		}

		void Profiler::BeginSession(const char* _name)
		{
			if (IsRecording() == false) return;

			SessionData sessionData;
			sessionData.Name = _name;
			sessionData.StartTime = static_cast<float>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
			sessionData.EndTime = 0.0f;

			m_currentSession = &sessionData;
			m_sessions.push_back(sessionData);
		}

		void Profiler::EndSession()
		{
			//if (IsRecording() == false || m_currentSession == nullptr) return;

			m_currentSession->EndTime = static_cast<float>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
			m_currentSession = nullptr;
		}

		void Profiler::AddResult(ProfileResult&& _result)
		{
			if (IsRecording() == false || m_currentFrame == nullptr) return;

			m_currentFrame->Results.emplace_back(_result);
		}

		void Profiler::SaveData()
		{
			std::string filePath = "sph_profilling_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".json";
			filePath = "sph_profilling.json";

			std::ofstream file(filePath);

			if (file.is_open() == false)
			{
				ASSERT(false, "Failed to open file: " + filePath);
				return;
			}

			file << "{\n";
			for (auto& session : m_sessions)
			{
				file << "\t\"" << session.Name << "\":\n";
				file << "\t{\n";
				file << "\t\t\"Start\": " << session.StartTime << ",\n";
				file << "\t\t\"End\": " << session.EndTime << ",\n";
				for (auto& frame : session.Frames)
				{
					file << "\t\t\"" << frame.FrameNumber << "\":\n";
					file << "\t\t{\n";
					file << "\t\t\t\"Start\": " << frame.StartTime << ",\n";
					file << "\t\t\t\"End\": " << frame.EndTime << ",\n";
					file << "\t\t\t\"Results\":\n";
					file << "\t\t\t[\n";
					for (size_t i = 0; i < frame.Results.size(); i++)
					{
						auto& result = frame.Results[i];
						file << "\t\t\t\t{\n";
						file << "\t\t\t\t\t\"Name\": \"" << result.Name << "\",\n";
						file << "\t\t\t\t\t\"Time\": " << result.Start - result.End << "\n";
						file << "\t\t\t\t\t\"Start\": " << result.Start << ",\n";
						file << "\t\t\t\t\t\"End\": " << result.End << "\n";
						file << "\t\t\t\t}";
						if (i < frame.Results.size() - 1)
						{
							file << ",";
						}
						file << "\n";
					}
					file << "\t\t\t]\n";
					file << "\t\t}";
					if (&frame != &session.Frames.back())
					{
						file << ",";
					}
					file << "\n";
				}

			}
		
		}
	}
}