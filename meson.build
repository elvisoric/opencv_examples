project('cv_intro', 'cpp', default_options:['cpp_std=c++17'])

first_main_src = files(['srcs/check.cpp'])

fmt_dep = dependency('fmt')
opencv_dep = dependency('opencv')
project_dep = declare_dependency(dependencies:[fmt_dep, opencv_dep])

executable('check', sources: first_main_src, dependencies:[project_dep])
