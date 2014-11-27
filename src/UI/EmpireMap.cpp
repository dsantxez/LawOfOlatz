#include "Window.h"
#include "AllWindows.h"
#include "Advisors.h"
#include "PopupDialog.h"
#include "MessageDialog.h"

#include "../Animation.h"
#include "../Calc.h"
#include "../CityInfo.h"
#include "../Empire.h"
#include "../Graphics.h"
#include "../Resource.h"
#include "../Scroll.h"
#include "../SidebarMenu.h"
#include "../Util.h"
#include "../Widget.h"

#include "../Data/CityInfo.h"
#include "../Data/Constants.h"
#include "../Data/Empire.h"
#include "../Data/Graphics.h"
#include "../Data/Invasion.h"
#include "../Data/Mouse.h"
#include "../Data/Scenario.h"
#include "../Data/Screen.h"

#define MAX_WIDTH 2032
#define MAX_HEIGHT 1136

static void drawPaneling();
static void drawPanelInfo();
static void drawPanelInfoCity();
static void drawPanelInfoBattleIcon();
static void drawPanelInfoRomanArmy();
static void drawPanelInfoEnemyArmy();
static void drawPanelInfoCityName();
static void drawPanelButtons();
static void drawEmpireMap();
static int getSelectedObject();

static void buttonHelp(int param1, int param2);
static void buttonReturnToCity(int param1, int param2);
static void buttonAdvisor(int param1, int param2);
static void buttonOpenTrade(int param1, int param2);
static void buttonEmpireMap(int param1, int param2);
static void confirmOpenTrade(int accepted);

static ImageButton imageButtonHelp[] = {
	{0, 0, 27, 27, 4, 134, 0, buttonHelp, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0}
};
static ImageButton imageButtonReturnToCity[] = {
	{0, 0, 24, 24, 4, 134, 4, buttonReturnToCity, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0}
};
static ImageButton imageButtonAdvisor[] = {
	{-4, 0, 24, 24, 4, 199, 12, buttonAdvisor, Widget_Button_doNothing, 1, 0, 0, 0, 5, 0}
};
static CustomButton customButtonOpenTrade[] = {
	{50, 68, 450, 91, buttonOpenTrade, Widget_Button_doNothing, 1, 0, 0}
};

static ImageButton imageButtonsTradeOpened[] = {
	{92, 248, 28, 28, 4, 199, 12, buttonAdvisor, Widget_Button_doNothing, 1, 0, 0, 0, 5, 0},
	{522, 252, 24, 24, 4, 134, 4, buttonEmpireMap, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0},
};

//MAKE THIS DATA OBJECT
static struct {
	int selectedButton;
	int selectedCity;
	int xMin, xMax, yMin, yMax;
} data = {0, 1};

void UI_Empire_drawBackground()
{
	data.xMin = Data_Screen.width <= MAX_WIDTH ? 0 : (Data_Screen.width - MAX_WIDTH) / 2;
	data.xMax = Data_Screen.width <= MAX_WIDTH ? Data_Screen.width : data.xMin + MAX_WIDTH;
	data.yMin = Data_Screen.height <= MAX_HEIGHT ? 0 : (Data_Screen.height - MAX_HEIGHT) / 2;
	data.yMax = Data_Screen.height <= MAX_HEIGHT ? Data_Screen.height : data.yMin + MAX_HEIGHT;

	if (data.xMin || data.yMin) {
		Graphics_clearScreen();
	}
	drawPaneling();
	drawPanelInfo();
}

void UI_Empire_drawForeground()
{
	drawEmpireMap();
	drawPanelInfoCityName();
	drawPanelButtons();
}

static void drawPaneling()
{
	int graphicBase = GraphicId(ID_Graphic_EmpirePanels);
	// bottom panel background
	Graphics_setClipRectangle(data.xMin, data.yMin, data.xMax - data.xMin, data.yMax - data.yMin);
	for (int x = data.xMin; x < data.xMax; x += 70) {
		Graphics_drawImage(graphicBase + 3, x, data.yMax - 120);
		Graphics_drawImage(graphicBase + 3, x, data.yMax - 80);
		Graphics_drawImage(graphicBase + 3, x, data.yMax - 40);
	}

	// horizontal bar borders
	for (int x = data.xMin; x < data.xMax; x += 86) {
		Graphics_drawImage(graphicBase + 1, x, data.yMin);
		Graphics_drawImage(graphicBase + 1, x, data.yMax - 120);
		Graphics_drawImage(graphicBase + 1, x, data.yMax - 16);
	}

	// vertical bar borders
	for (int y = data.yMin + 16; y < data.yMax; y += 86) {
		Graphics_drawImage(graphicBase, data.xMin, y);
		Graphics_drawImage(graphicBase, data.xMax - 16, y);
	}

	// crossbars
	Graphics_drawImage(graphicBase + 2, data.xMin, data.yMin);
	Graphics_drawImage(graphicBase + 2, data.xMin, data.yMax - 120);
	Graphics_drawImage(graphicBase + 2, data.xMin, data.yMax - 16);
	Graphics_drawImage(graphicBase + 2, data.xMax - 16, data.yMin);
	Graphics_drawImage(graphicBase + 2, data.xMax - 16, data.yMax - 120);
	Graphics_drawImage(graphicBase + 2, data.xMax - 16, data.yMax - 16);

	Graphics_resetClipRectangle();
}

static void drawPanelInfo()
{
	if (Data_Empire.selectedObject) {
		switch (Data_Empire_Objects[Data_Empire.selectedObject-1].type) {
			case EmpireObject_City:
				drawPanelInfoCity();
				break;
			case EmpireObject_BattleIcon:
				drawPanelInfoBattleIcon();
				break;
			case EmpireObject_RomanArmy:
				drawPanelInfoRomanArmy();
				break;
			case EmpireObject_EnemyArmy:
				drawPanelInfoEnemyArmy();
				break;
		}
	} else {
		Widget_GameText_drawCentered(47, 8, data.xMin, data.yMax - 48, data.xMax - data.xMin, Font_NormalGreen);
	}
}

static void drawPanelInfoCity()
{
	int objectId = Data_Empire.selectedObject - 1;
	int xOffset = (data.xMin + data.xMax - 240) / 2;
	int yOffset = data.yMax - 88;

	if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_DistantRoman) {
		Widget_GameText_drawCentered(47, 12, xOffset, yOffset + 42, 240, Font_NormalGreen);
		return;
	}
	if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_VulnerableRoman) {
		if (Data_CityInfo.distantBattleCityMonthsUntilRoman <= 0) {
			Widget_GameText_drawCentered(47, 12, xOffset, yOffset + 42, 240, Font_NormalGreen);
		} else {
			Widget_GameText_drawCentered(47, 13, xOffset, yOffset + 42, 240, Font_NormalGreen);
		}
		return;
	}
	if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_FutureTrade ||
		Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_DistantForeign ||
		Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_FutureRoman) {
		Widget_GameText_drawCentered(47, 0, xOffset, yOffset + 42, 240, Font_NormalGreen);
		return;
	}
	if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_Ours) {
		Widget_GameText_drawCentered(47, 1, xOffset, yOffset + 42, 240, Font_NormalGreen);
		return;
	}
	if (Data_Empire_Cities[data.selectedCity].cityType != EmpireCity_Trade) {
		return;
	}
	// trade city
	xOffset = (data.xMin + data.xMax - 500) / 2;
	yOffset = data.yMax - 108;
	if (Data_Empire_Cities[data.selectedCity].isOpen) {
		// city sells
		Widget_GameText_draw(47, 10, xOffset + 40, yOffset + 30, Font_NormalGreen);
		int goodOffset = 0;
		for (int good = 1; good <= 15; good++) {
			if (!Empire_citySellsResource(objectId, good)) {
				continue;
			}
			Graphics_drawInsetRect(xOffset + 100 * goodOffset + 120, yOffset + 21, 26, 26);
			int graphicId = good + GraphicId(ID_Graphic_EmpireResource);
			int resourceOffset = Resource_getGraphicIdOffset(good, 3);
			Graphics_drawImage(graphicId + resourceOffset, xOffset + 100 * goodOffset + 121, yOffset + 22);
			int routeId = Data_Empire_Cities[data.selectedCity].routeId;
			switch (Data_Empire_Trade.maxPerYear[routeId][good]) {
				case 15:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount),
						xOffset + 100 * goodOffset + 141, yOffset + 20);
					break;
				case 25:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 1,
						xOffset + 100 * goodOffset + 137, yOffset + 20);
					break;
				case 40:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 2,
						xOffset + 100 * goodOffset + 133, yOffset + 20);
					break;
			}
			int tradeNow = Data_Empire_Trade.tradedThisYear[routeId][good];
			int tradeMax = Data_Empire_Trade.maxPerYear[routeId][good];
			if (tradeNow > tradeMax) {
				tradeMax = tradeNow;
			}
			int textWidth = Widget_Text_drawNumber(tradeNow, '@', "",
				xOffset + 100 * goodOffset + 150, yOffset + 30, Font_NormalGreen);
			textWidth += Widget_GameText_draw(47, 11,
				xOffset + 100 * goodOffset + 148 + textWidth, yOffset + 30, Font_NormalGreen);
			Widget_Text_drawNumber(tradeMax, '@', "",
				xOffset + 100 * goodOffset + 138 + textWidth, yOffset + 30, Font_NormalGreen);
			goodOffset++;
		}
		// city buys
		Widget_GameText_draw(47, 9, xOffset + 40, yOffset + 60, Font_NormalGreen);
		goodOffset = 0;
		for (int good = 1; good <= 15; good++) {
			if (!Empire_cityBuysResource(objectId, good)) {
				continue;
			}
			Graphics_drawInsetRect(xOffset + 100 * goodOffset + 120, yOffset + 51, 26, 26);
			int graphicId = good + GraphicId(ID_Graphic_EmpireResource);
			int resourceOffset = Resource_getGraphicIdOffset(good, 3);
			Graphics_drawImage(graphicId + resourceOffset, xOffset + 100 * goodOffset + 121, yOffset + 52);
			int routeId = Data_Empire_Cities[data.selectedCity].routeId;
			switch (Data_Empire_Trade.maxPerYear[routeId][good]) {
				case 15:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount),
						xOffset + 100 * goodOffset + 141, yOffset + 50);
					break;
				case 25:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 1,
						xOffset + 100 * goodOffset + 137, yOffset + 50);
					break;
				case 40:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 2,
						xOffset + 100 * goodOffset + 133, yOffset + 50);
					break;
			}
			int tradeNow = Data_Empire_Trade.tradedThisYear[routeId][good];
			int tradeMax = Data_Empire_Trade.maxPerYear[routeId][good];
			if (tradeNow > tradeMax) {
				tradeMax = tradeNow;
			}
			int textWidth = Widget_Text_drawNumber(tradeNow, '@', "",
				xOffset + 100 * goodOffset + 150, yOffset + 60, Font_NormalGreen);
			textWidth += Widget_GameText_draw(47, 11,
				xOffset + 100 * goodOffset + 148 + textWidth, yOffset + 60, Font_NormalGreen);
			Widget_Text_drawNumber(tradeMax, '@', "",
				xOffset + 100 * goodOffset + 138 + textWidth, yOffset + 60, Font_NormalGreen);
			goodOffset++;
		}
	} else { // trade is closed
		int goodOffset = Widget_GameText_draw(47, 5, xOffset + 50, yOffset + 42, Font_NormalGreen);
		for (int good = 1; good <= 15; good++) {
			if (!Empire_citySellsResource(objectId, good)) {
				continue;
			}
			Graphics_drawInsetRect(xOffset + goodOffset + 60, yOffset + 33, 26, 26);
			int graphicId = good + GraphicId(ID_Graphic_EmpireResource);
			int resourceOffset = Resource_getGraphicIdOffset(good, 3);
			Graphics_drawImage(graphicId + resourceOffset, xOffset + goodOffset + 61, yOffset + 34);
			int routeId = Data_Empire_Cities[data.selectedCity].routeId;
			switch (Data_Empire_Trade.maxPerYear[routeId][good]) {
				case 15:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount),
						xOffset + goodOffset + 81, yOffset + 32);
					break;
				case 25:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 1,
						xOffset + goodOffset + 77, yOffset + 32);
					break;
				case 40:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 2,
						xOffset + goodOffset + 73, yOffset + 32);
					break;
			}
			goodOffset += 32;
		}
		goodOffset += Widget_GameText_draw(47, 4, xOffset + goodOffset + 100, yOffset + 42, Font_NormalGreen);
		for (int good = 1; good <= 15; good++) {
			if (!Empire_cityBuysResource(objectId, good)) {
				continue;
			}
			Graphics_drawInsetRect(xOffset + goodOffset + 110, yOffset + 33, 26, 26);
			int graphicId = good + GraphicId(ID_Graphic_EmpireResource);
			int resourceOffset = Resource_getGraphicIdOffset(good, 3);
			Graphics_drawImage(graphicId + resourceOffset, xOffset + goodOffset + 110, yOffset + 34);
			int routeId = Data_Empire_Cities[data.selectedCity].routeId;
			switch (Data_Empire_Trade.maxPerYear[routeId][good]) {
				case 15:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount),
						xOffset + goodOffset + 130, yOffset + 32);
					break;
				case 25:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 1,
						xOffset + goodOffset + 126, yOffset + 32);
					break;
				case 40:
					Graphics_drawImage(GraphicId(ID_Graphic_TradeAmount) + 2,
						xOffset + goodOffset + 122, yOffset + 32);
					break;
			}
			goodOffset += 32;
		}
		Widget_Panel_drawButtonBorder(xOffset + 50, yOffset + 68, 400, 20, data.selectedButton);
		goodOffset = Widget_GameText_drawNumberWithDescription(8, 0,
			Data_Empire_Cities[data.selectedCity].costToOpen,
			xOffset + 60, yOffset + 73, Font_NormalGreen);
		Widget_GameText_draw(47, 6, xOffset + goodOffset + 60, yOffset + 73, Font_NormalGreen);
	}
}

static void drawPanelInfoBattleIcon()
{
	// nothing
}

static void drawPanelInfoRomanArmy()
{
	if (Data_CityInfo.distantBattleRomanMonthsToTravel > 0 ||
		Data_CityInfo.distantBattleRomanMonthsToReturn > 0) {
		if (Data_CityInfo.distantBattleRomanMonthsTraveled ==
			Data_Empire_Objects[Data_Empire.selectedObject-1].distantBattleTravelMonths) {
			int xOffset = (data.xMin + data.xMax - 240) / 2;
			int yOffset = data.yMax - 88;
			int textId;
			if (Data_CityInfo.distantBattleRomanMonthsToTravel) {
				textId = 15;
			} else {
				textId = 16;
			}
			Widget_GameText_drawMultiline(47, textId, xOffset, yOffset, 240, Font_NormalBlack);
		}
	}
}

static void drawPanelInfoEnemyArmy()
{
	if (Data_CityInfo.distantBattleMonthsToBattle > 0) {
		if (Data_CityInfo.distantBattleEnemyMonthsTraveled ==
			Data_Empire_Objects[Data_Empire.selectedObject-1].distantBattleTravelMonths) {
			Widget_GameText_drawMultiline(47, 14,
				(data.xMin + data.xMax - 240) / 2,
				data.yMax - 68,
				240, Font_NormalBlack);
		}
	}
}

static void drawPanelInfoCityName()
{
	int graphicBase = GraphicId(ID_Graphic_EmpirePanels);
	Graphics_drawImage(graphicBase + 6, data.xMin + 2, data.yMax - 199);
	Graphics_drawImage(graphicBase + 7, data.xMax - 84, data.yMax - 199);
	Graphics_drawImage(graphicBase + 8, (data.xMin + data.xMax - 332) / 2, data.yMax - 181);
	if (Data_Empire.selectedObject > 0) {
		if (Data_Empire_Objects[Data_Empire.selectedObject-1].type == EmpireObject_City) {
			Widget_GameText_drawCentered(21, Data_Empire_Cities[data.selectedCity].cityNameId,
				(data.xMin + data.xMax - 332) / 2 + 64, data.yMax - 118, 268, Font_LargeBlack);
		}
	}
}

static void drawPanelButtons()
{
	Widget_Button_drawImageButtons(data.xMin + 20, data.yMax - 44, imageButtonHelp, 1);
	Widget_Button_drawImageButtons(data.xMax - 44, data.yMax - 44, imageButtonReturnToCity, 1);
	Widget_Button_drawImageButtons(data.xMax - 44, data.yMax - 100, imageButtonAdvisor, 1);
	if (Data_Empire.selectedObject) {
		if (Data_Empire_Objects[Data_Empire.selectedObject-1].type == EmpireObject_City) {
			data.selectedCity = Empire_getCityForObject(Data_Empire.selectedObject-1);
			if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_Trade && !Data_Empire_Cities[data.selectedCity].isOpen) {
				Widget_Panel_drawButtonBorder((data.xMin + data.xMax - 500) / 2 + 50, data.yMax - 40, 400, 20, data.selectedButton);
			}
		}
	}
}

static void drawEmpireMap()
{
	Graphics_setClipRectangle(data.xMin + 16, data.yMin + 16, data.xMax - data.xMin - 32, data.yMax - data.yMin - 136);

	BOUND(Data_Empire.scrollX, 0, 2000 - (data.xMax - data.xMin - 32));
	BOUND(Data_Empire.scrollY, 0, 1000 - (data.yMax - data.yMin - 136));

	int xOffset = data.xMin + 16 - Data_Empire.scrollX;
	int yOffset = data.yMin + 16 - Data_Empire.scrollY;
	Graphics_drawImage(GraphicId(ID_Graphic_EmpireMap), xOffset, yOffset);

	for (int i = 0; i < 200 && Data_Empire_Objects[i].inUse; i++) {
		Data_Empire_Object *obj = &Data_Empire_Objects[i];
		if (obj->type == EmpireObject_LandTradeRoute || obj->type == EmpireObject_SeaTradeRoute) {
			if (!Empire_isTradeRouteOpen(obj->tradeRouteId)) {
				continue;
			}
		}
		int x, y, graphicId;
		if (Data_Scenario.empireHasExpanded) {
			x = obj->xExpanded;
			y = obj->yExpanded;
			graphicId = obj->graphicIdExpanded;
		} else {
			x = obj->x;
			y = obj->y;
			graphicId = obj->graphicId;
		}

		if (obj->type == EmpireObject_City) {
			int city = Empire_getCityForObject(i);
			if (Data_Empire_Cities[city].cityType == EmpireCity_DistantForeign ||
				Data_Empire_Cities[city].cityType == EmpireCity_FutureRoman) {
				graphicId = GraphicId(ID_Graphic_EmpireForeignCity);
			}
		}
		if (obj->type == EmpireObject_BattleIcon) {
			// handled below
			continue;
		}
		if (obj->type == EmpireObject_EnemyArmy) {
			if (Data_CityInfo.distantBattleMonthsToBattle <= 0) {
				continue;
			}
			if (Data_CityInfo.distantBattleEnemyMonthsTraveled != obj->distantBattleTravelMonths) {
				continue;
			}
		}
		if (obj->type == EmpireObject_RomanArmy) {
			if (Data_CityInfo.distantBattleRomanMonthsToTravel <= 0 &&
				Data_CityInfo.distantBattleRomanMonthsToReturn <= 0) {
				continue;
			}
			if (Data_CityInfo.distantBattleRomanMonthsTraveled != obj->distantBattleTravelMonths) {
				continue;
			}
		}
		Graphics_drawImage(graphicId, xOffset + x, yOffset + y);
		if (GraphicHasAnimationSprite(graphicId)) {
			obj->animationIndex = Animation_getIndexForEmpireMap(graphicId, obj->animationIndex);
			Graphics_drawImage(graphicId + obj->animationIndex,
				xOffset + x + GraphicSpriteOffsetX(graphicId),
				yOffset + y + GraphicSpriteOffsetY(graphicId));
		}
	}

	for (int i = 0; i < 101; i++) {
		if (Data_InvasionWarnings[i].inUse && Data_InvasionWarnings[i].handled) {
			Graphics_drawImage(Data_InvasionWarnings[i].empireGraphicId,
				xOffset + Data_InvasionWarnings[i].empireX,
				yOffset + Data_InvasionWarnings[i].empireY);
		}
	}
	Graphics_resetClipRectangle();
}

static int getSelectedObject()
{
	if (!Data_Mouse.left.wentDown) {
		return -1;
	}
	if (Data_Mouse.x < data.xMin + 16 || Data_Mouse.x >= data.xMax - 16 ||
		Data_Mouse.y < data.yMin + 16 || Data_Mouse.y >= data.yMax - 120) {
		return -1;
	}
	int xMap = Data_Mouse.x + Data_Empire.scrollX - data.xMin - 16;
	int yMap = Data_Mouse.y + Data_Empire.scrollY - data.yMin - 16;
	int minDist = 10000;
	int objId = 0;
	for (int i = 0; i < 200 && Data_Empire_Objects[i].inUse; i++) {
		Data_Empire_Object *obj = &Data_Empire_Objects[i];
		int xObj, yObj;
		if (Data_Scenario.empireHasExpanded) {
			xObj = obj->xExpanded;
			yObj = obj->yExpanded;
		} else {
			xObj = obj->x;
			yObj = obj->y;
		}
		if (xObj - 8 > xMap || xObj + obj->width + 8 <= xMap) {
			continue;
		}
		if (yObj - 8 > yMap || yObj + obj->height + 8 <= yMap) {
			continue;
		}
		int dist = Calc_distanceMaximum(xMap, yMap,
			xObj + obj->width / 2, yObj + obj->height / 2);
		if (dist < minDist) {
			minDist = dist;
			objId = i + 1;
		}
	}
	UI_Window_requestRefresh();
	return objId;
}

void UI_Empire_handleMouse()
{
	Empire_scrollMap(Scroll_getDirection());
	if (Widget_Button_handleImageButtons(data.xMin + 20, data.yMax - 44, imageButtonHelp, 1)) {
		return;
	}
	if (Widget_Button_handleImageButtons(data.xMax - 44, data.yMax - 44, imageButtonReturnToCity, 1)) {
		return;
	}
	if (Widget_Button_handleImageButtons(data.xMax - 44, data.yMax - 100, imageButtonAdvisor, 1)) {
		return;
	}
	int objectId = getSelectedObject();
	if (objectId > 0) {
		Data_Empire.selectedObject = objectId;
	} else if (objectId == 0) {
		Data_Empire.selectedObject = 0;
	}
	if (Data_Mouse.right.wentDown) {
		Data_Empire.selectedObject = 0;
		UI_Window_requestRefresh();
	}
	if (Data_Empire.selectedObject) {
		if (Data_Empire_Objects[Data_Empire.selectedObject-1].type == EmpireObject_City) {
			data.selectedCity = Empire_getCityForObject(Data_Empire.selectedObject-1);
			if (Data_Empire_Cities[data.selectedCity].cityType == EmpireCity_Trade && !Data_Empire_Cities[data.selectedCity].isOpen) {
				Widget_Button_handleCustomButtons((data.xMin + data.xMax - 500) / 2, data.yMax - 105, customButtonOpenTrade, 1, &data.selectedButton);
			}
		}
	}
}

static void buttonHelp(int param1, int param2)
{
	UI_MessageDialog_show(MessageDialog_EmpireMap, 1);
}

static void buttonReturnToCity(int param1, int param2)
{
	UI_Window_goTo(Window_City);
}

static void buttonAdvisor(int advisor, int param2)
{
	UI_Advisors_goToFromMessage(Advisor_Trade);
}

static void buttonOpenTrade(int param1, int param2)
{
	UI_PopupDialog_show(PopupDialog_OpenTrade, confirmOpenTrade, 2);
}

static void buttonEmpireMap(int param1, int param2)
{
	UI_Window_goTo(Window_Empire);
}

static void confirmOpenTrade(int accepted)
{
	if (accepted) {
		CityInfo_Finance_spendOnConstruction(Data_Empire_Cities[data.selectedCity].costToOpen);
		Data_Empire_Cities[data.selectedCity].isOpen = 1;
		SidebarMenu_enableBuildingMenuItemsAndButtons();
		UI_Window_goTo(Window_TradeOpenedDialog);
	}
}

void UI_TradeOpenedDialog_drawBackground()
{
	int xOffset = Data_Screen.offset640x480.x;
	int yOffset = Data_Screen.offset640x480.y;
	Widget_Panel_drawOuterPanel(xOffset + 80, yOffset + 64, 30, 14);
	Widget_GameText_drawCentered(142, 0, xOffset + 80, yOffset + 80, 480, Font_LargeBlack);
	if (Data_Empire_Cities[data.selectedCity].isSeaTrade) {
		Widget_GameText_drawMultiline(142, 1, xOffset + 112, yOffset + 120, 416, Font_NormalBlack);
		Widget_GameText_drawMultiline(142, 3, xOffset + 112, yOffset + 184, 416, Font_NormalBlack);
	} else {
		Widget_GameText_drawMultiline(142, 1, xOffset + 112, yOffset + 152, 416, Font_NormalBlack);
	}
	Widget_GameText_draw(142, 2, xOffset + 128, yOffset + 256, Font_NormalBlack);
}

void UI_TradeOpenedDialog_drawForeground()
{
	Widget_Button_drawImageButtons(
		Data_Screen.offset640x480.x, Data_Screen.offset640x480.y,
		imageButtonsTradeOpened, 2);
}

void UI_TradeOpenedDialog_handleMouse()
{
	Widget_Button_handleImageButtons(
		Data_Screen.offset640x480.x, Data_Screen.offset640x480.y,
		imageButtonsTradeOpened, 2);
}
