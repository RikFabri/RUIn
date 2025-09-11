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
		RenderArea CalculateUsedContentArea(const RenderArea& availableArea) override;


		bool HandleMouseMoved(int cursorX, int cursorY) override;
		bool HandleMouseDown(int cursorX, int cursorY) override;
		bool HandleMouseUp(int cursorX, int cursorY) override;
		
		size_t PatchAllDataFromBuffer(void* buffer, unsigned bufferSize) override;
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
		virtual RenderArea GetAreaForChild(const RenderArea& availableArea, RenderArea& usedArea, RenderContext& ctx) const = 0;

		bool HandleMouseEventGeneric(int cursorX, int cursorY, std::function<bool(IRenderable*, int, int)> func);
		void DataSourceChanged();

		size_t InstantiateItemTemplate(size_t sourceStreamOffset);

		void InitializeVerticalFillmode(const char* mode);
		void InitializeHorizontalFillmode(const char* mode);

		AlignHelper m_AlignHelper;

		std::vector<std::unique_ptr<IRenderable>> m_Renderables;
		std::vector<RenderArea> m_RenderAreaPerRenderable;

		ClientBuffer m_DataSource;
		std::string m_ItemTemplate;

		int m_RowNumber = 0;
	};

}