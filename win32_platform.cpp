#include <windows.h> // работа с окном

#include "utils.h"

#include "Objects/render_state.h"
#include "Objects/platform_common.h"



Render_state render_state;

bool running = true;



#include "Render/render.cpp"

#include "Game/game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {

		running = false;
	}break;


	case WM_SIZE: {

		// update window size
		{
			RECT rect;

			GetClientRect(hwnd, &rect);

			render_state.resize(rect.bottom - rect.top, rect.right - rect.left);
		}


		

	}break;

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}

	return result;
}

void update_controls(HWND& window, Input &input) {

	for (s32 i = 0; i < BUTTON_COUNT; i++) {
		input.buttons[i].changed = false;
	}

	auto set_button = [&input](button_t b, bool is_down, bool changed) -> void {

		input.buttons[b].is_down = is_down;
		input.buttons[b].changed = changed;
	};

	MSG message;
	while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

		

		switch (message.message) {

		case WM_KEYUP:
		case WM_KEYDOWN: {

			u64 vk_code = message.wParam;
			bool is_down = (message.lParam & (static_cast<u64>(1) << 31)) == 0;


#define update_button(b, vk)\
case vk:{\
	set_button(b, is_down, (is_down != input.buttons[b].is_down));\
}

			switch (vk_code) {
				update_button(BUTTON_W, 'W');
				update_button(BUTTON_A, 'A');
				update_button(BUTTON_S, 'S');
				update_button(BUTTON_D, 'D');
			}

		}break;

		default: {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		}
	}
}



int main() {

	// Create Window class
	WNDCLASS window_class = {};
	{
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpszClassName = L"Game Window Class";
		window_class.lpfnWndProc = window_callback;
		//window_class.hIcon = static_cast<HICON>(LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	}

	// Register class
	RegisterClass(&window_class);

	HINSTANCE hInstance = GetModuleHandle(NULL); // дескриптор указанного модуля

	// Create window
	HWND window = CreateWindow(window_class.lpszClassName, L"C++ Game Engine", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0, 0, hInstance, 0);
	// Fullscreen
	/*{
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}*/

	// дескриптор устройства (DC) для клиентской области указанного окна или для всего экрана
	// Используется в последующих функциях GDI для рисования в DС
	HDC hdc = GetDC(window);

	Input input = {};

	while (running) {

		update_controls(window);

		simulate_game(input);

		
		// render screen
		{
			StretchDIBits(hdc,
				0, 0, render_state.width, render_state.height,
				0, 0, render_state.width, render_state.height,
				reinterpret_cast<void*>(render_state.render_memory), &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
		}


	}

    return 0;
}