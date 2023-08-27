#include "stdafx.h"
#include "ToolTip.h"
#include "Button.h"
#include "Input.h"
#include "IUIObject.h"//TODO

static std::set<ToolTip*> ToolTipSet;

static const unsigned int TOOLTIP_MAX_SIZE = 0x800;
static char ToolTipStrBuffer[TOOLTIP_MAX_SIZE];

ToolTip::ToolTip() : _frame(), _text() {}

static const float TEXT_MARGIN_X = 0.001f;;
static const float TEXT_MARGIN_Y = 0.001f;

ToolTip* ToolTip::Create(
	UISimpleFrame* parent,
	float width,
	float height,
	const char* formattedContent,
	bool dontStore
)
{
	ToolTip* tooltip = new ToolTip();

	UISimpleFrame* sf = UISimpleFrame::Create(parent);
	sf->setWidth(width); sf->setHeight(height);
	sf->setBackground(Skin::getPathByName("ToolTipBackground"));
	sf->setBorder(Skin::getPathByName("ToolTipBorder"));
	sf->setBorderWidth(0.01f);//size ?
	sf->setPadding(0.002f);
	sf->applyTextureSettings();
	//sf->setAbsolutePosition( UILayoutFrame::POSITION_BOTTOM_LEFT, 0.3f, 0.3f );
	sf->setRelativePosition(UISimpleFrame::POSITION_TOP_LEFT, parent, UISimpleFrame::POSITION_TOP_LEFT, width + 0.02f, 0.0f);
	/*sf->setRelativePosition(
		UISimpleFrame::POSITION_LEFT,
		parent,
		UISimpleFrame::POSITION_TOP_RIGHT,
		0, 0
	);*/
	sf->show();


	tooltip->_frame = sf;

	UISimpleFontString* sfs = UISimpleFontString::Create(tooltip->_frame);
	sprintf_s(ToolTipStrBuffer, TOOLTIP_MAX_SIZE, "%s", formattedContent);//TODO½âÎö?
	sfs->initFont(UIObject::GetPathByName("InfoPanelTextFont"), 0.013f, 0);//?
	sfs->setRelativePosition(UISimpleFrame::POSITION_TOP_LEFT, tooltip->_frame, UISimpleFrame::POSITION_TOP_LEFT, 0.001f, 0.001f);
	sfs->setText(ToolTipStrBuffer);
	
	sf->setWidth(sfs->getTextWidth() + (2 * TEXT_MARGIN_X));
	sf->setHeight(sfs->getTextHeight() + (2 * TEXT_MARGIN_Y));

	//	sf->setWidth( sfs->getTextWidth( ) );


	tooltip->_text = sfs;

	if (!dontStore) ToolTipSet.insert(tooltip);

	tooltip->applyPosition();

	return tooltip;
}

void ToolTip::Destroy(ToolTip* tooltip) {
	if (!tooltip) return;
	if (tooltip->_text) UISimpleFontString::Destroy(tooltip->_text);
	if (tooltip->_frame) UISimpleFrame::Destroy(tooltip->_frame);
	delete tooltip;
}

void ToolTip::bindButton(Button* button) {

	float centerx = button->getFrame()->getCenterX();
	float centery = button->getFrame()->getCenterY();

	float widthoffset = button->getFrame()->width() * 0.5f;
	float heightoffset = button->getFrame()->height() * 0.5f;

	float tooltipwidth = _frame->width();
	float tooltipheight = _frame->height();

	float offsetx = 0.0f;
	float offsety = 0.0f;

	if (centerx + widthoffset + tooltipwidth + 0.01f > wc3_max_x)
	{
		offsetx = -tooltipwidth;
	}
	else
	{
		offsetx = widthoffset * 2.0f;
	}

	if ((centery - heightoffset * 2.0f) - tooltipheight < 0.01f)
	{
		offsety = tooltipheight;
	}
	else
	{
		offsety = 0.0f;
	}

	bindButton(button, offsetx, offsety);
}
void ToolTip::bindButton(Button* button, float offsetx, float offsety) {
	_frame->setRelativePosition(
		UISimpleFrame::POSITION_TOP_LEFT,
		button->getFrame(),
		UISimpleFrame::POSITION_TOP_LEFT,
		offsetx, offsety
	);
	applyPosition();
}

void ToolTip::applyPosition() {
	_frame->setWidth(_text->getTextWidth() * War3WindowRatioX() + 0.02f);
	_frame->setHeight(_text->getTextHeight() * War3WindowRatioY() + 0.01f);

	_frame->applyPosition();
	_text->applyPosition();
}

void ToolTip::show() {
	_frame->show();
}

void ToolTip::hide() {
	_frame->hide();
}

void ToolTip_Init() {
	ToolTipSet.clear();
}

void ToolTip_Cleanup() {
	for (std::set<ToolTip*>::iterator iter = ToolTipSet.begin();
		iter != ToolTipSet.end(); ++iter) {
		ToolTip::Destroy(*iter);
	}
	ToolTipSet.clear();
}