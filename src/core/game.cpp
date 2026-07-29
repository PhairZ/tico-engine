#include "game.h"

#include <chrono>

void Game::run() {
  _init();

  while (m_running) {
    using namespace std::chrono;
    auto pre_update = high_resolution_clock::now();

    _update(m_delta_time);
    _draw();

    m_renderer.render_screen();

    auto post_update = high_resolution_clock::now();
    m_delta_time = duration<double>(post_update - pre_update).count();

    // Limit frames per second.
    constexpr double FPS_CAP = 30.0;
    constexpr double TARGET_FRAME_TIME = 1.0 / FPS_CAP;
    constexpr double BUSY_WAIT_TIME = 0.01;
    const double time_to_wait = TARGET_FRAME_TIME - m_delta_time;

    // Sleep most of the remained time.
    if (time_to_wait > 0.001) {
      std::this_thread::sleep_for(
          duration<double>(time_to_wait - BUSY_WAIT_TIME));
    }

    // Busy wait until the next frame for the last couple of clocks.
    // This method achieves more precision.
    while (true) {
      double d =
          duration<double>(high_resolution_clock::now() - pre_update).count();
      if (d > TARGET_FRAME_TIME) {
        break;
      } else {
        m_delta_time = d;
      }
    }
  }
}
