#pragma once

struct Button_state {

	bool is_down;
	bool changed;
};

enum button_t {

	BUTTON_W,
	BUTTON_A,
	BUTTON_S,
	BUTTON_D,



	BUTTON_COUNT,
};

struct Input {
	Button_state buttons[BUTTON_COUNT];
};

