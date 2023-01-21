add_rules("mode.debug", "mode.release")

target("cwheel_lib")
    set_kind("static")
    set_languages("cxx20")
    set_toolchains("clang")
    add_files("*.cpp","*.cc")
    add_includedirs("$(projectdir)/", {public = true})

