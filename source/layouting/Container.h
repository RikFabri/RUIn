#pragma once
#include <vector>
#include <memory>
#include <functional>

#include "../resources/ClientData.h"
#include "Renderable.h"
#include "tinyxml2.h"
#include "MathLib.h"
#include "AlignHelper.h"

namespace RUIN
{
	class UIContainer : public IRenderable 
	{
	public:
		UIContainer(tinyxml2::XMLElement* element);

		void Render(const RenderArea& targetArea) override;
		RenderArea CalculateUsedContentArea(const Erm::vec2f& availableArea) override;
		void ApplyContentAwareTransormations(const Erm::vec2f& scales, const Erm::vec2f& offsets) override;

		bool HandleMouseMoved(int cursorX, int cursorY) override;
		bool HandleMouseDown(int cursorX, int cursorY) override;
		bool HandleMouseUp(int cursorX, int cursorY) override;
		bool HandleMouseScroll(float distance, int cursorX, int cursorY) override;
		
		size_t PatchAllDataFromBuffer(const void* buffer, unsigned bufferSize) override;
		void SetRowNumber(int row) override;
		int GetRowNumber() const override;

		void AddChildWidget(tinyxml2::XMLElement* element);

		size_t GetNumChildren() const;

		void ClearWidgets();

	protected:
		struct RenderContext
		{
			int childIndex;
		};


	private:
		virtual RUIN::RenderArea GetAreaForChild(const Erm::vec2f& availableArea, const RenderArea& usedArea, const RenderContext& ctx) const = 0;

		template<typename FuncType, typename... Args>
		inline bool HandleMouseEventGeneric(int cursorX, int cursorY, FuncType&& func, Args&&... args);

		static RenderArea GetCombinedBounds(RenderArea left, RenderArea right);

		void DataSourceChanged();

		size_t InstantiateItemTemplate(size_t sourceStreamOffset);

		void InitializeVerticalFillmode(const char* mode);
		void InitializeHorizontalFillmode(const char* mode);
		void InitializeVerticalOverflow(const char* mode);
		void InitializeBackgroundColour(const char* col);

		AlignHelper m_AlignHelper;
		RenderArea m_RenderArea;

		std::vector<std::unique_ptr<IRenderable>> m_Renderables;
		std::vector<RenderArea> m_RenderAreaPerRenderable;

		ClientBuffer m_DataSource;
		std::string m_ItemTemplate;

		int m_RowNumber = 0;

	protected:
		enum class Overflow { Hidden, Scroll, Visible };
		Overflow m_VerticalOverflow = Overflow::Scroll;
		float m_ScrollAmount = 0;
		bool m_Overflowing = false;
		
		RUIN_Colour m_BackgroundColour = { 0, 0, 0, 0 };
	};

	template<typename FuncType, typename... Args>
	inline bool UIContainer::HandleMouseEventGeneric(int cursorX, int cursorY, FuncType&& func, Args&& ...args)
	{
		int count = 0;
		for (auto& renderable : m_Renderables)
		{
			if (count >= m_RenderAreaPerRenderable.size())
			{
				return false;
			}

			const auto& renderArea = m_RenderAreaPerRenderable[count++];

			if (renderArea.ContainsPoint(cursorX, cursorY))
			{
				const bool handled = std::forward<FuncType>(func)(renderable.get(), cursorX, cursorY, std::forward<Args>(args)...);
				if (handled)
				{
					return true;
				}
			}

		}

		return false;
	}
}