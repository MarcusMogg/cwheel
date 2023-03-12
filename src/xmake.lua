add_rules("mode.debug", "mode.release")

add_requires("fmt")

target("cwheel_lib")
    set_kind("static")
    set_languages("cxx20")
    set_warnings("all", "error")
    add_files("*.cpp","*.cc")
    add_packages("fmt", {public = true})
    add_includedirs("$(projectdir)/", {public = true})
    add_options("test")
    if is_mode("release") then
        add_toolchains("clang")
    end