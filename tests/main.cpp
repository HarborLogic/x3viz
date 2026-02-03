#include "x3viz.h"

namespace x3
{
	int x3_run_app(int argc, char** argv, ix3app* papp) {
		x3::iwindow* pwindow = papp->create_window("Test Window", 800, 600);
		x3::iviewport* pviewport = papp->create_viewport(pwindow, 0, 0, 800, 600);
		x3::igc* pgc = pviewport->get_context();
		if (!pgc) {
			papp->log_message("Failed to get graphics context from viewport\n");
			return -1;
		}

		x3::ifont* pfont = pgc->create_font("arial.ttf", 16);
		if (!pfont) {
			papp->log_message("Failed to load font 'arial.ttf', using default font\n");
			pfont = pgc->get_default_font();
		}

		/* prepare graphics */
		pgc->push_state();
		pgc->set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
		pgc->set_fill_color(x3::color4(0.0f, 0.5f, 1.0f, 1.0f));
		pgc->set_font(pfont);

		while (!pwindow->should_close()) {
			pgc->clear();



			pwindow->poll_events();
		}
		pgc->pop_state();
		return 0;
	}
};