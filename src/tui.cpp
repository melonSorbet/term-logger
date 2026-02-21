#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <print>
#include <ripgrep.hpp>
#include <string>
#include <vector>

void load_file(std::vector<std::string> &lines,
               std::vector<RipGrepMatch> current_matches, int selected) {
  lines = {};

  std::fstream file(current_matches[selected].path);
  if (!file) {
    return;
  }
  std::string string = "";
  while (std::getline(file, string)) {
    lines.push_back(string);
  }
}

using namespace ftxui;
int setupTUI() {

  auto screen = ScreenInteractive::Fullscreen();

  std::vector<std::string> entries = {};
  std::string pattern_input;
  std::string hint = "";
  std::vector<RipGrepMatch> current_matches = {};

  std::vector<std::string> lines = {};

  MenuOption option;
  InputOption input_option = InputOption();
  int selected = 0;

  input_option.on_change = [&entries, &screen, &pattern_input,
                            &current_matches] {
    screen.Post([&] {
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

  option.on_change = [&lines, &current_matches, &selected] {};

  // add event to listen for up arrow and down error to only controll movement
  // add event to listen on mouse button click and go into used editor on teh
  // file add event to listen to mouse wheel and up and down for scrolling
  // add event for listening to n and N to jump to the next and last marked word
  // add event for ctrl c and and y to copy
  // add event for h for help

  auto menu = Menu(&entries, &selected, option);
  float scroll_y = 0.1f;
  SliderOption<float> option_y;
  option_y.value = &scroll_y;
  option_y.min = 0.f;
  option_y.max = 1.f;
  option_y.increment = 0.1f;
  option_y.direction = Direction::Down;
  option_y.color_active = Color::Yellow;
  option_y.color_inactive = Color::YellowLight;
  auto scrollbar_y = Slider(option_y);

  // mark matches

  // Open file with Enter

  auto content = ftxui::Renderer([&] {
    std::vector<ftxui::Element> elements;
    int count = 0;
    for (const auto &line : lines) {
      if (count == current_matches[selected].line_number - 1) {
        int start = std::min((int)line.size(), current_matches[selected].start);
        int end = std::min((int)line.size(), current_matches[selected].end);

        std::string first_half = line.substr(0, start);
        std::string keyword = line.substr(start, end - start);
        std::string second_half = line.substr(end);
        std::println("{}", keyword);
        elements.push_back(
            ftxui::hbox({ftxui::text(first_half),
                         ftxui::underlined(ftxui::text(keyword)) |
                             ftxui::color(ftxui::Color::Red),
                         ftxui::text(second_half)}));
      } else {

        elements.push_back(ftxui::text(line));
      }

      count++;
    }

    return ftxui::vbox(elements) | ftxui::focusPositionRelative(0, scroll_y) |
           ftxui::frame | ftxui::flex;
  });

  auto text_preview = ftxui::Container::Horizontal({

      content,
      scrollbar_y,

  });

  // auto text_preview_with_input = CatchEvent(text_preview, [&](Event event) {
  //   // Mouse scroll wheel
  //   if (event.is_mouse()) {
  //     if (event.mouse().button == Mouse::WheelDown) {
  //       scroll_y = std::min(1.f, scroll_y + 0.05f);
  //       return true;
  //     }
  //     if (event.mouse().button == Mouse::WheelUp) {
  //       scroll_y = std::max(0.f, scroll_y - 0.05f);
  //       return true;
  //     }
  //   }

  //  if (event == Event::ArrowDown) {
  //    scroll_y = std::min(1.f, scroll_y + 0.05f);
  //    return true;
  //  }
  //  if (event == Event::ArrowUp) {
  //    scroll_y = std::max(0.f, scroll_y - 0.05f);
  //    return true;
  //  }
  //  if (event == Event::PageDown) {
  //    scroll_y = std::min(1.f, scroll_y + 0.2f);
  //    return true;
  //  }
  //  if (event == Event::PageUp) {
  //    scroll_y = std::max(0.f, scroll_y - 0.2f);
  //    return true;
  //  }

  //  return false;
  //});

  auto window_1 = Renderer(text_preview, [&] {
    return vbox({
               text("Preview Window") | bold | center,
               separator(),
               text_preview->Render() | flex,
           }) |

           border;
  });

  auto left = Container::Vertical(
      {input_path | frame |
           size(ftxui::WIDTH, ftxui::EQUAL, (Terminal::Size().dimx / 2)) |
           size(ftxui::HEIGHT, EQUAL, 1) | border,
       menu | frame | border | flex});

  auto left_renderer = Renderer(left, [&] {
    return vbox({
        menu->Render() | frame | border | flex,
        input_path->Render() | frame |
            size(ftxui::WIDTH, ftxui::EQUAL, (Terminal::Size().dimx / 2)) |
            size(ftxui::HEIGHT, EQUAL, 1) | border,
    });
  });
  bool editor_opened = false; // declare outside the CatchEvent lambda
  auto exit_loop = screen.ExitLoopClosure();
  auto window_container = CatchEvent(
      Container::Horizontal({left_renderer | flex, window_1 | flex}),
      [&](Event event) {
        if (event == Event::Escape ||
            event.is_character() && event.character() == "q") {
          screen.ExitLoopClosure();
          return true;
        }

        if (event == Event::ArrowUp) {
          if (current_matches.empty()) {
            return true;
          }
          // move up in the list
          selected = std::max(0, selected - 1);
          option.on_change();
          scroll_y = static_cast<float>(current_matches[selected].line_number) /
                     std::max(1, (int)lines.size() - 1);
          load_file(lines, current_matches, selected);

          return true;
        }

        if (event == Event::ArrowDown) {
          // move down in the list
          if (current_matches.empty()) {
            return true;
          }
          selected = std::min((int)entries.size() - 1, selected + 1);
          option.on_change();
          screen.PostEvent(Event::Custom);
          // scrolled position
          scroll_y = static_cast<float>(current_matches[selected].line_number) /
                     std::max(1, (int)lines.size() - 1);

          load_file(lines, current_matches, selected);
          return true;
        }

        // if (event == Event::Return && !editor_opened) {
        //   if (current_matches.empty()) {
        //     return true;
        //   }
        //   editor_opened = true;

        //   std::string editor =
        //       std::getenv("EDITOR") ? std::getenv("EDITOR") : "vi";
        //   std::string file = current_matches[selected].path;
        //   int line = std::max(0, current_matches[selected].line_number - 1);

        //   std::thread([&, editor, file, line]() {
        //     pid_t pid = fork();
        //     if (pid == 0) {
        //       execlp(editor.c_str(), editor.c_str(),
        //              std::format("+{}", line).c_str(), file.c_str(),
        //              nullptr);
        //       std::perror("execlp failed");
        //       std::exit(1);
        //     } else if (pid > 0) {
        //       int status;
        //       waitpid(pid, &status, 0); // wait for editor
        //       load_file(lines, current_matches, selected);
        //     } else {
        //       std::perror("fork failed");
        //     }
        //     editor_opened = false;
        //   }).detach();
        //   }

        return false;
      });

  input_path->TakeFocus();
  screen.Loop(window_container);
  return 1;
}
