#include "sphpch.h"
#include "Layer.h"

sph::Layer::Layer(const std::string& name)
	: m_name(name)
	, m_application(nullptr)
{	}

sph::Layer::~Layer()
{	}
