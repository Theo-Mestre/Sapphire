#include "sphpch.h"
#include "Profiler.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Core/Core.h"

namespace sph
{
	namespace Profiling
	{
		Profiler::Profiler()
		{
			m_file.open(std::string("Profiling/ProfilingResults" + std::to_string((float)std::chrono::system_clock::now().time_since_epoch().count()) + ".json"));

			if (!m_file.is_open())
			{
				LogError("Failed to open file");
			}

			m_file << "{\n";
		}

		Profiler::~Profiler()
		{
			m_file << "}\n";
			m_file.close();
		}

		void Profiler::AddResult(FrameData&& _result)
		{
			m_frameResults.push_back(std::move(_result));
		}

		void Profiler::StartFrame()
		{
			m_frameNumber++;
			m_frameResults.clear();
		}

		void Profiler::EndFrame()
		{
			SaveData();
		}

		void Profiler::SaveData()
		{
			if (!m_file.is_open()) { return; }

			m_file << "\t\"frames" << m_frameNumber << "\" : \n\t[\n";

			for (uint32_t i = 0; i < m_frameResults.size(); i++)
			{
				auto& frame = m_frameResults[i];
				m_file << "\t{\n";
				m_file << "\t\t\"name\" : \"" << frame.Name << "\",\n";
				m_file << "\t\t\"start\" : " << frame.Start << ",\n";
				m_file << "\t\t\"end\" : " << frame.End << "\n";
				if (i == m_frameResults.size() - 1) m_file << "\t}\n";
				else m_file << "\t},\n";
			}
			m_file << "\t],\n";
		}
	}
}