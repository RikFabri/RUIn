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

		bool PropagateEvent(const Erm::vec2f& position, int cursorX, int cursorY, const Event& event) override;
		
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

		bool HandleMouseScroll(const Event& event);
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
		
		bool m_ScrolledToEnd = true;
		float m_ScrollAmount = 0;

	protected:
		enum class Overflow { Hidden, Scroll, Visible };
		Overflow m_VerticalOverflow = Overflow::Hidden;
		bool m_Overflowing = false;
		
		RUIN_Colour m_BackgroundColour = { 0, 0, 0, 0 };
	};
}