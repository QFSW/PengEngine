#pragma once

#include <string>

#include <core/subsystem.h>
#include <math/vector2.h>
#include <utils/timing.h>

struct GLFWwindow;

namespace rendering
{
	// TODO: make _active checking part of base Subsystem
    class WindowSubsystem final : public Subsystem
    {
        DECLARE_SUBSYSTEM(WindowSubsystem)

    public:
        WindowSubsystem();
        ~WindowSubsystem() override;

        void start() override;
        void shutdown() override;
        void tick(float delta_time) override;

		void finalize_frame(float target_frametime);

		void set_resolution(const math::Vector2i& resolution) noexcept;
		void set_resolution(const math::Vector2i& resolution, bool fullscreen) noexcept;
		void set_cursor_locked(bool cursor_locked);
		void set_vsync(bool vsync);
		void set_msaa(uint32_t msaa_samples);
		void set_window_name(const std::string& name);

		void enter_fullscreen();
		void exit_fullscreen();
		void toggle_fullscreen();

		void maximize_window() const;

		[[nodiscard]] bool should_close() const noexcept;
		[[nodiscard]] const math::Vector2i& resolution() const noexcept;
		[[nodiscard]] float aspect_ratio() const noexcept;
		[[nodiscard]] bool fullscreen() const noexcept;
		[[nodiscard]] GLFWwindow* window_handle() const noexcept;

	private:
		math::Vector2i _resolution;
		math::Vector2i _windowed_resolution;
		math::Vector2i _windowed_position;
		bool _fullscreen;
		bool _cursor_locked;
		bool _vsync;
		uint32_t _msaa_samples;
		std::string _window_name;
		GLFWwindow* _window;

		bool _active;
		timing::clock::time_point _last_draw_time;
    };
}