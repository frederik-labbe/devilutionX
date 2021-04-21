/**
* @file xpbar.cpp
*
* Adds XP bar QoL feature
*/

#include "DiabloUI/art_draw.h"
#include "common.h"
#include "control.h"
#include "options.h"

#include <array>

namespace devilution {

namespace {

constexpr int BAR_WIDTH = 307;

using ColorGradient = std::array<Uint8, 12>;
constexpr ColorGradient GOLD_GRADIENT = { 0xCF, 0xCE, 0xCD, 0xCC, 0xCB, 0xCA, 0xC9, 0xC8, 0xC7, 0xC6, 0xC5, 0xC4 };
constexpr ColorGradient SILVER_GRADIENT = { 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3 };

constexpr int BACK_WIDTH = 313;
constexpr int BACK_HEIGHT = 9;

Art xpbarArt;

void DrawBar(const CelOutputBuffer &out, int x, int y, int width, const ColorGradient &gradient)
{
	FastDrawHorizLine(out, x, y + 1, width, gradient[gradient.size() * 3 / 4 - 1]);
	FastDrawHorizLine(out, x, y + 2, width, gradient[gradient.size() - 1]);
	FastDrawHorizLine(out, x, y + 3, width, gradient[gradient.size() / 2 - 1]);
}

void DrawEndCap(const CelOutputBuffer &out, int x, int y, int idx, const ColorGradient &gradient)
{
	SetPixel(out, x, y + 1, gradient[idx * 3 / 4]);
	SetPixel(out, x, y + 2, gradient[idx]);
	SetPixel(out, x, y + 3, gradient[idx / 2]);
}

} // namespace

void InitXPBar()
{
	if (sgOptions.Gameplay.bExperienceBar) {
		LoadMaskedArt("data\\xpbar.pcx", &xpbarArt, 1, 1);

		if (xpbarArt.surface == nullptr) {
			app_fatal("Failed to load UI resources. Is devilutionx.mpq accessible and up to date?");
		}
	}
}

void FreeXPBar()
{
	xpbarArt.Unload();
}

void DrawXPBar(const CelOutputBuffer &out)
{
	if (!sgOptions.Gameplay.bExperienceBar)
		return;

	const PlayerStruct &player = plr[myplr];

	const int backX = PANEL_LEFT + PANEL_WIDTH / 2 - 155;
	const int backY = PANEL_TOP + PANEL_HEIGHT - 11;

	const int xPos = backX + 3;
	const int yPos = backY + 2;

	DrawArt(out, backX, backY, &xpbarArt);

	const int charLevel = player._pLevel;

	const ColorGradient& gradient = (charLevel == MAXCHARLEVEL - 1) ? GOLD_GRADIENT : SILVER_GRADIENT;

	const int prevXp = ExpLvlsTbl[charLevel - 1];
	if (player._pExperience < prevXp)
		return;

	Uint64 prevXpDelta_1 = player._pExperience - prevXp;
	Uint64 prevXpDelta = ExpLvlsTbl[charLevel] - prevXp;
	Uint64 fullBar = BAR_WIDTH * prevXpDelta_1 / prevXpDelta;

	// Figure out how much to fill the last pixel of the XP bar, to make it gradually appear with gained XP
	Uint64 onePx = prevXpDelta / BAR_WIDTH;
	Uint64 lastFullPx = fullBar * prevXpDelta / BAR_WIDTH;

	const Uint64 fade = (prevXpDelta_1 - lastFullPx) * (gradient.size() - 1) / onePx;

	// Draw beginning of bar full brightness
	DrawBar(out, xPos, yPos, fullBar, gradient);

	// End pixels appear gradually
	DrawEndCap(out, xPos + fullBar, yPos, fade, gradient);
}

bool CheckXPBarInfo()
{
	if (!sgOptions.Gameplay.bExperienceBar)
		return false;

	const int backX = PANEL_LEFT + PANEL_WIDTH / 2 - 155;
	const int backY = PANEL_TOP + PANEL_HEIGHT - 11;

	if (MouseX < backX || MouseX >= backX + BACK_WIDTH || MouseY < backY || MouseY >= backY + BACK_HEIGHT)
		return false;

	const PlayerStruct &player = plr[myplr];

	const int charLevel = player._pLevel;

	infoclr = COL_WHITE;

	sprintf(tempstr, "Level %d", charLevel);
	AddPanelString(tempstr, true);
	sprintf(tempstr, "Experience: ");
	PrintWithSeparator(tempstr + SDL_arraysize("Experience: ") - 1, player._pExperience);
	AddPanelString(tempstr, true);

	if (charLevel != (MAXCHARLEVEL - 1)) {
		auto difference = ExpLvlsTbl[charLevel] - player._pExperience;
		sprintf(tempstr, "Next level: ");
		PrintWithSeparator(tempstr + SDL_arraysize("Next level: ") - 1, ExpLvlsTbl[charLevel]);
		AddPanelString(tempstr, true);
		sprintf(PrintWithSeparator(tempstr, difference), " to level %d", charLevel + 1);
		AddPanelString(tempstr, true);
	}

	return true;
}

} // namespace devilution
