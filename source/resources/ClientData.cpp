#include "ClientData.h"
#include <algorithm>
#include "../UIManager.h"

RUIN::ClientTexture::~ClientTexture()
{
	if (!m_pClientData)
		return;

	UIManager::GetInstance().FreeTexture(m_pClientData);
}

RUIN::ClientTexture::ClientTexture()
	: m_pClientData(nullptr)
{
}

RUIN::ClientTexture::ClientTexture(void* pClientData)
	: m_pClientData(pClientData)
{
}

RUIN::ClientTexture::ClientTexture(ClientTexture&& other) noexcept
{
	m_pClientData = other.m_pClientData;
	other.m_pClientData = nullptr;
}

RUIN::ClientTexture& RUIN::ClientTexture::operator=(ClientTexture&& other) noexcept
{
	if (this == &other)
		return *this;

	std::swap(m_pClientData, other.m_pClientData);

	return *this;
}

void* RUIN::ClientTexture::GetClientData() const
{
	return m_pClientData;
}

void RUIN::ClientTexture::GetDimensions(uint32_t& width, uint32_t& height) const
{
	UIManager::GetInstance().QueryTextureDimensions(*this, width, height);
}

void RUIN::ClientTexture::Draw(const RenderArea& ra) const
{
	UIManager::GetInstance().DrawTexture(*this, ra);
}

RUIN::ClientBuffer::ClientBuffer(unsigned size, const void* data)
	: m_Size(size)
	, m_pClientData(data)
{
}

RUIN::ClientBuffer::ClientBuffer()
	: m_Size(0)
	, m_pClientData(nullptr)
{
}

unsigned RUIN::ClientBuffer::GetBufferSize() const
{
	return m_Size;
}

const void* RUIN::ClientBuffer::GetBuffer(size_t offset) const
{
	RASSERT(offset < m_Size, "Accessing buffer with offset bigger than size!");

	return (char*)m_pClientData + offset;
}
