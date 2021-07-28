from . import wrapper


def show_text(text: str):
    wrapper.show_text(text)


class Frame:
    def __init__(self, window: wrapper.GLFWWindow):
        self.window = window

    def __enter__(self):
        wrapper.enter_frame()
        wrapper.glfw_poll_events()

    def __exit__(self, exc_type, exc_value, traceback):
        wrapper.exit_frame(self.window)


class Open:
    def __init__(self, name: str) -> None:
        self.name = name

    def __enter__(self):
        self.window = wrapper.enter_glfw_window(self.name)
        wrapper.enter_imgui_window(self.window)
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        wrapper.exit_imgui_window(self.window)
        wrapper.exit_glfw_window(self.window)
        return

    def should_close(self):
        return wrapper.glfw_window_should_close(self.window)

    def open(self):
        return Frame(self.window)
