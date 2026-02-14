#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <ripgrep.hpp>
#include <string>
#include <vector>

using namespace ftxui;

int setupTUI() {

  auto screen = ScreenInteractive::Fullscreen();

  std::vector<std::string> entries = {};
  std::string pattern_input;
  InputOption input_option = InputOption();
  std::string hint = "";
  std::vector<RipGrepMatch> current_matches = {};

  input_option.on_change = [&entries, &screen, &pattern_input,
                            &current_matches] {
    screen.Post([&] {
      // get entries and push them all in preferably in a stack
      auto results = jsonToStruct(run_ripgrep(pattern_input));
      entries = {};
      for (auto result : results) {
        entries.push_back(result.path);
      }
      current_matches = results;
    });
  };

  input_option.transform = [](InputState state) {
    state.element |= borderEmpty;

    if (state.is_placeholder) {
      state.element |= dim;
    }

    if (state.hovered) {
      state.element |= borderRounded;
      state.element |= color(Color::White);
    } else {

      state.element |= color(Color::White);
    }

    return state.element;
  };

  Component input_path = Input(&pattern_input, &hint, input_option);

  int selected = 0;

  MenuOption option;

  std::vector<std::string> lines = {};
  option.on_enter = [&lines, &current_matches, &selected] {
    lines = {};

    std::fstream file(current_matches[selected].path);
    if (!file) {
      return;
    }
    std::string string = "";
    while (std::getline(file, string)) {
      lines.push_back(string);
    }
  };

  auto menu = Menu(&entries, &selected, option);

  auto left = Container::Vertical(
      {menu | frame | border | flex,
       input_path | frame |
           size(ftxui::WIDTH, ftxui::EQUAL, (Terminal::Size().dimx / 2)) |
           size(ftxui::HEIGHT, EQUAL, 1) | border});

  int scroll_position = 0;

  auto text_preview = ftxui::Renderer([&] {
    std::vector<ftxui::Element> elements;
    for (const auto &line : lines) {
      elements.push_back(text(line));
    }
    return vbox(elements) | vscroll_indicator | frame | flex;
  });

  auto window_1 = Renderer(text_preview, [&] {
    return vbox({
               text("Preview Window") | bold | center,
               separator(),
               text_preview->Render() | flex,
           }) |
           border;
  });

  auto window_container = Container::Horizontal({left | flex, window_1 | flex});

  screen.Loop(window_container);
  return 1;
}
