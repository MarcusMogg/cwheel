add_rules("mode.debug", "mode.release")

target("cwheel_lib")
    set_kind("static")
    set_warnings("all", "error")
    add_files("*.cc")
    add_headerfiles("*.hpp","*.h")
    add_options("test")
