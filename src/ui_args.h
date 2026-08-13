//------------------------------------------------------------------------
//  Linedef and thing argument widget
//------------------------------------------------------------------------
//
//  Eureka DOOM Editor
//
//  Copyright (C) 2026 Ioan Chera
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

#ifndef __EUREKA_UI_ARGS_H__
#define __EUREKA_UI_ARGS_H__

#include "m_game.h"
#include "m_strings.h"
#include "ui_misc.h"

#include "FL/Fl_Flex.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Menu_Button.H"

#include <functional>
#include <vector>

class ArgMenuButton;
class Fl_Light_Button;
class LineDef;
class PanelFieldFixUp;
struct ConfigData;
struct Document;
struct Thing;

class UI_ArgField : public Fl_Group
{
public:
	UI_ArgField(int X, int Y, int W, int H, PanelFieldFixUp &fixUp);
	void resize(int X, int Y, int W, int H) override;

	void loadToFixUp();
	void textcolor(Fl_Color n);
	void setInputValue(const char *value);

	void setAsTag(const Document &doc);
	void setAsLineID(const Document &doc, const ConfigData &config);
	void setAsPolyobject(const Document &doc, const ConfigData &config);
	void setAsBoolean();
	void setAsGeneric();
	void setAsCustom(const ArgType &type);

	int value() const;
	void updateFlags();

private:
	class Input : public UI_DynIntInput
	{
	public:
		Input(int X, int Y, int W, int H) : UI_DynIntInput(X, Y, W, H)
		{
		}

		int handle(int event) override;

		void setButton(ArgMenuButton *button)
		{
			this->button = button;
		}

	private:
		ArgMenuButton *button;
	};

	static void optionCallback(Fl_Widget *widget, void *context);
	void updateOptions();
	void makeTagList(const std::set<int> &tags);

	PanelFieldFixUp &fixUp;

	Input *input;
	ArgMenuButton *button;
	std::vector<Fl_Menu_Item> buttonItems;
	Fl_Light_Button *toggleButton;

	ArgType argType;
};

class UI_ArgsBox : public Fl_Flex
{
public:
	using Callback = std::function<void(int index, int value)>;

	UI_ArgsBox(int X, int Y, PanelFieldFixUp &fixUp, const Document &doc);
	~UI_ArgsBox();

	void loadToFixUp() const;

	void trackLabels();
	void clear(PanelFieldFixUp &fixUp);
	void populate(PanelFieldFixUp &fixUp, const ConfigData &config, const LineDef &linedef);
	void populate(PanelFieldFixUp &fixUp, const ConfigData &config, const Thing &thing);
	bool labelsChanged() const;

	void setCallbackFunction(Callback callback)
	{
		callbackFunction = std::move(callback);
	}

private:
	static void argsCallback(Fl_Widget *widget, void *context);
	void setLabel(int index, const SString &text, SpecialArgType type, const SString &customName,
		const ConfigData &config);

	UI_ArgField *args[5];

	SString argLabels[5];

	Callback callbackFunction;

	const Document &doc;
};

#endif
