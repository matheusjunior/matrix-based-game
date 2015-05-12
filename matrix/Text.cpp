//
// Created by Matheus Pereira Junior on 3/26/15.
// Copyright (c) 2015 ___FULLUSERNAME___. All rights reserved.
//

#include "Text.h"

Text::Text()
{
    surface = NULL;
    rect = {10, 10, 150, 25};
    color = {0, 0, 0};
    font = NULL;
    texture = NULL;
    displayText = "";
}

Text* Text::loadFontAndSetPositionRef(int px, int py, int w, int h) {
	Text* text = new Text();
	text->font = TTF_OpenFont("media/emulogic.ttf", 20);

	text->rect.x = px;
	text->rect.y = py;
	text->rect.w = w;
	text->rect.h = h;
	text->displayText = "Test";
	return text;
}

Text Text::loadFontAndSetPosition(int px, int py, int w, int h) {
	Text text;
	
	return text;
}