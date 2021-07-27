import imgui_adapter


def main():
    floatv = imgui_adapter.variable.FloatVariable(1)
    breakpoint()
    with imgui_adapter.window.Open("Hello") as window:
        while not window.should_close():
            with window.open():
                imgui_adapter.window.show_text("Hello!")
                imgui_adapter.window.show_float_slider(floatv)
            floatv.print()


if __name__ == "__main__":
    main()
