#include "../libretro.h"
#include "../FlipRetro.h"
class Render {
	public:
		struct retro_hw_render_callback hw_render;

		bool init() {
			enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
			if(!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)) return false;
#ifdef GLES
			hw_render.context_type = RETRO_HW_CONTEXT_OPENGLES2;
#elif CORE
			hw_render.context_type = RETRO_HW_CONTEXT_OPENGL_CORE;
			hw_render.version_major = 3;
			hw_render.version_minor = 1;
#else
			hw_render.context_type = RETRO_HW_CONTEXT_OPENGL;
#endif
			hw_render.context_reset = context_reset;
			hw_render.context_destroy = context_destroy;
			hw_render.depth = true;
			hw_render.stencil = true;
			hw_render.bottom_left_origin = true;
			if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render)) return false;
			return true;
		}
		static void context_reset(void) {
			rglgen_resolve_symbols(hw_render.get_proc_address);
			compile_program();
			setup_vao();
#ifdef CORE
			context_alive = true;
			init_multisample(multisample);
#endif
		}

		static void context_destroy(void) {

#ifdef CORE
			glDeleteVertexArrays(1, &vao);
			vao = 0;
			init_multisample(0);
			context_alive = false;
#endif
			glDeleteBuffers(1, &vbo);
			vbo = 0;
			glDeleteProgram(prog);
			prog = 0;
		}

		void render() {
#ifdef CORE
			glBindVertexArray(vao);
			if (multisample_fbo)
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			else
				glBindFramebuffer(GL_FRAMEBUFFER, hw_render.get_current_framebuffer());
#else
			glBindFramebuffer(GL_FRAMEBUFFER, hw_render.get_current_framebuffer());
#endif

			glClearColor(0.3, 0.4, 0.5, 1.0);
			glViewport(0, 0, WIDTH, HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUseProgram(prog);

			glEnable(GL_DEPTH_TEST);
		}
#ifdef CORE
		static const char *vertex_shader[] = {
			"#version 140\n",
			"uniform mat4 uMVP;",
			"in vec4 aVertex;",
			"in vec4 aColor;",
			"out vec4 color;",
			"void main() {",
			"	gl_Position = uMVP * vec4(aVertex);",
			"	color = aColor;",
			"}",
		};

		static const char *fragment_shader[] = {
			"#version 140\n",
			"in vec4 color;",
			"out vec4 FragColor;"
				"void main() {",
			"	FragColor = color;",
			"}",
		};
#else
		static const char *vertex_shader[] = {
			"uniform mat4 uMVP;",
			"attribute vec4 aVertex;",
			"attribute vec4 aColor;",
			"varying vec4 color;",
			"void main() {",
			"	gl_Position = uMVP * vec4(aVertex);",
			"	color = aColor;",
			"}",
		};

		static const char *fragment_shader[] = {
#ifdef GLES
			"precision mediump float;",
#endif
			"varying vec4 color;",
			"void main() {",
			"  gl_FragColor = color;",
			"}",
		};
#endif
		static void compile_program(void) {
			prog = glCreateProgram();
			GLuint vert = glCreateShader(GL_VERTEX_SHADER);
			GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(vert, ARRAY_SIZE(vertex_shader), vertex_shader, 0);
			glShaderSource(frag, ARRAY_SIZE(fragment_shader), fragment_shader, 0);
			glCompileShader(vert);
			glCompileShader(frag);

			glAttachShader(prog, vert);
			glAttachShader(prog, frag);
			glLinkProgram(prog);
			glDeleteShader(vert);
			glDeleteShader(frag);
		}
};
