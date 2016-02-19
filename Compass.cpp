#include "main.h"


using namespace GW2LIB;


Compass::Compass()
{
	m_ptr = nullptr;
}

Compass::Compass(const Compass &comp)
{
	m_ptr = comp.m_ptr;
}

Compass &Compass::operator= (const Compass &comp)
{
	if (this != &comp) {
		m_ptr = comp.m_ptr;
	}
	return *this;
}

float Compass::GetMaxWidth() const {
	if (m_ptr)
		return m_ptr->maxWidth;
	return false;
}

float Compass::GetMaxHeight() const {
	if (m_ptr)
		return m_ptr->maxHeight;
	return false;
}

float Compass::GetWidth() const
{
	if (m_ptr)
		return m_ptr->width;
	return false;
}


float Compass::GetHeight() const
{
	if (m_ptr)
		return m_ptr->height;
	return false;
}


int Compass::GetZoom() const
{
	if (m_ptr)
		return m_ptr->zoom;
	return false;
}

bool Compass::GetRotation() const {
	if (m_ptr)
		return m_ptr->flags.rotation;
	return false;
}

bool Compass::GetPosition() const {
	if (m_ptr)
		return m_ptr->flags.position;
	return false;
}

