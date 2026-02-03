#pragma once
#include <cstdint>
#include <cassert>
#include <stdexcept>

namespace x3
{
	class color4 {
	public:
		float r, g, b, a;
		color4(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f)
			: r(red), g(green), b(blue), a(alpha) {
		}

		void from_RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
			r = static_cast<float>(r) / 255.0f;
			g = static_cast<float>(g) / 255.0f;
			b = static_cast<float>(b) / 255.0f;
			a = static_cast<float>(a) / 255.0f;
		}
	};

	class vec3 {
	public:
		float x, y, z;
		vec3(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f)
			: x(vx), y(vy), z(vz) {
		}
	};

	class vec4 {
	public:
		union {
			struct { float x, y, z, w; };
			float v[4];
		};
		vec4(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f, float vw=0.f)
			: x(vx), y(vy), z(vz), w(vw) {
		}
	};

	class vert {
	public:
		vec3   position;
		color4 color;
		vert(const vec3& pos = vec3(0.f, 0.f, 0.f), const color4& col = color4(1.f, 1.f, 1.f, 1.f))
			: position(pos), color(col) {
		}
	};

	class mat4 {
		vec4 rows[4];
	public:
		mat4() {
			make_identity();
		}
		inline vec4& operator[](int index) {
			assert(index >= 0 && index < 4 && "mat4 row index out of bounds");
			return rows[index];
		}

		void make_identity() {
			rows[0] = vec4(1.f, 0.f, 0.f, 0.f);
			rows[1] = vec4(0.f, 1.f, 0.f, 0.f);
			rows[2] = vec4(0.f, 0.f, 1.f, 0.f);
			rows[3] = vec4(0.f, 0.f, 0.f, 1.f);
		}
	};

	/* widget */
	class iwidget {
	public:
		virtual ~iwidget() = default;
		virtual void show(bool bshow = true) = 0;
		virtual void resize(int width, int height) = 0;
		virtual void move(int x, int y) = 0;
		virtual void set_text(const char* text) = 0;
		virtual void set_parent(iwidget* parent) = 0;
		virtual void* get_handle() const = 0;
		virtual iwidget* get_parent() const = 0;
	};

	/* window */
	class iwindow : public iwidget {
	public:
		virtual ~iwindow() = default;
		virtual void poll_events() = 0;
		virtual bool should_close() const = 0;
	};

	class ibuffers {
	public:
		virtual ~ibuffers() = default;
		virtual int  get_vertex_count() const = 0;
		virtual int  get_index_count() const = 0;
		virtual bool lock_buffers(vert** pverts, int** ptris) = 0;
		virtual void unlock_buffers() = 0;
	};

	class buffers_autolock {
		bool m_locked;
		ibuffers*& m_buffersref;
	public:
		buffers_autolock(ibuffers*& pbuffers, vert** pverts, int** ptris) :
			m_buffersref(pbuffers), m_locked(false) {
			assert(m_buffersref != nullptr && "m_buffersref was nullptr");
			m_locked = m_buffersref->lock_buffers(pverts, ptris);
		}
		~buffers_autolock() {
			m_buffersref->unlock_buffers();
		}
		inline bool is_locked() const {
			return m_locked;
		}
	};

	class ifont {
	public:
		virtual ~ifont() = default;
		virtual int get_size() const = 0;
		virtual bool get_text_metrics(const char* text) const = 0;
		virtual const char *get_name() = 0;
	};

	/* graphics context */
	class igc {
	public:
		virtual ~igc() = default;
		virtual void set_clear_color(float r, float g, float b, float a) = 0;
		virtual void clear() = 0;
		virtual void present() = 0;

		/**
		* @brief Saves the current graphics state onto a stack.
		* @details This function saves the current graphics state, including settings such as colors, line widths, and transformations, onto a stack. This allows for temporary changes to the graphics state that can be easily reverted.
		*/
		virtual void push_state() = 0;

		/**
		* @brief Restores the most recently saved graphics state from the stack.
		* @details This function restores the graphics state to the most recently saved state from the stack. It is used in conjunction with push_state() to revert any temporary changes made to the graphics state.
		*/
		virtual void pop_state() = 0;

		virtual void set_fill_color(color4 color) = 0;
		virtual void set_line_color(color4 color) = 0;
		virtual void set_line_width(float width) = 0;
		virtual void set_stroke_color(color4 color) = 0;
		virtual void set_projection(const mat4& projection) = 0;
		virtual void set_model(const mat4& transform) = 0;

		virtual color4 get_fill_color() const = 0;
		virtual color4 get_line_color() const = 0;
		virtual float  get_line_width() const = 0;
		virtual color4 get_stroke_color() const = 0;
		virtual mat4  get_projection() const = 0;
		virtual mat4  get_model() const = 0;

		virtual void draw_line(vec3 p1, vec3 p2) = 0;
		virtual void draw_filled_rect(float x, float y, float width, float height) = 0;
		//virtual void draw(vec3 *ptris, int count, int start = 0) = 0;
		virtual ibuffers* create_buffers(int vcount, int icount, vert* pverts = nullptr, int* pindices = nullptr) = 0;
		virtual void remove_buffers(ibuffers* pbuffers) = 0;
		virtual void draw(vert* pverts, int count, int start = 0) = 0;
		virtual void draw(vert* pverts, int* ptris, int count, int start = 0) = 0;
		virtual void draw(ibuffers* pbuffers, int start = 0) = 0;

		/* font drawing */
		virtual ifont* create_font(const char* filename, int size) = 0;
		virtual void remove_font(ifont* pfont) = 0;
		virtual void set_font(ifont* pfont) = 0;
		virtual ifont* get_font() = 0;
		virtual ifont* get_default_font() = 0;
		virtual ifont* find_font(const char* filename) = 0;
		virtual void draw_text(float x, float y, const char* text) = 0;
		virtual void draw_textf(float x, float y, const char* pformat, ...) = 0;
	};

	/* viewport */
	class iviewport : public iwidget {
	public:
		virtual ~iviewport() = default;
		virtual void set_context(igc* pgc) = 0;
		virtual igc* get_context() = 0;
	};

	enum backend {
		OpenGL_core = 0,
		OpenGL_legacy_compat
	};

	class ix3app {
	public:
		virtual ~ix3app() = default;
		virtual iwindow* create_window(const char* title, int width, int height) = 0;
		virtual iviewport* create_viewport(iwidget* parent, int x, int y, int width, int height, backend backend_api = backend::OpenGL_core) = 0;
		virtual void destroy_window(iwindow* pwindow) = 0;
		virtual void destroy_viewport(iviewport* pviewport) = 0;
		virtual void log_message(const char* pformat, ...) = 0;
	};

	extern int x3_run_app(int argc, char** argv, ix3app *pvis);
};

