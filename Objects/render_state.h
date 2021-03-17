#pragma once

struct Render_state {

	u32 height, width; // размеры окна

	u32* render_memory; // память для рендера

	BITMAPINFO bitmap_info;


	void resize(u32 h, u32 w) {

		height = h;
		width = w;

		// update render_memory
		{
			if (render_memory) {
				delete[] render_memory;
			}

			u64 size = static_cast<u64>(h) * w;

			render_memory = new u32[size];
		}

		// update bitmap_info
		{
			auto& header = bitmap_info.bmiHeader;

			header.biSize = sizeof(header);
			header.biWidth = w;
			header.biHeight = h;
			header.biPlanes = 1;
			header.biBitCount = 32;
			header.biCompression = BI_RGB;
		}
	}

};
