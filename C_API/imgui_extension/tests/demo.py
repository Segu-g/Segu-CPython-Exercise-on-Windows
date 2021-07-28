import imgui_adapter


def main():
    float_array = [imgui_adapter.variable.FloatVariable(1 / (i + 1)) for i in range(100)]
    with imgui_adapter.window.Open("Hello") as root_window:
        while not root_window.should_close():
            with root_window.open():
                imgui_adapter.window.show_text("Hello!")
                f = imgui_adapter.wrapper.get_framerate()
                imgui_adapter.window.show_text(f"{f}")
                for i in range(100):
                    imgui_adapter.wrapper.show_float_slider(f"float{i}", float_array[i])


if __name__ == "__main__":
    main()
