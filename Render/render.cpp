

void draw_pixels(u32 x0, u32 y0, u32 x1, u32 y1, u32 color) {

	for (u32 y = y0; y < y1; y++) {
		for (u32 x = x0; x < x1; x++) {

			render_state.render_memory[y * render_state.width + x] = color;
		}
	}
}
