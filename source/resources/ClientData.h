#pragma once
#include <cstdint>

#include "../layouting/RenderArea.h"

namespace RUIN
{
	class ClientBuffer
	{
	public:
		ClientBuffer(unsigned size, const void* data);
		ClientBuffer();

		unsigned GetBufferSize() const;
		const void* GetBuffer(size_t offset = 0) const;
	private:
		const void* m_pClientData;
		unsigned m_Size;
	};

	class ClientTexture
	{
	public:
		~ClientTexture();
		ClientTexture();
		ClientTexture(void* pClientData);
		ClientTexture(ClientTexture&& other) noexcept;
		ClientTexture(const ClientTexture& other) = delete;
		ClientTexture& operator=(ClientTexture&& other) noexcept;

		void* GetClientData() const;
		void GetDimensions(uint32_t& width, uint32_t& height) const;
		void Draw(const RenderArea& ra) const;
	private:

		void* m_pClientData;
	};
}