project('cv_intro', 'cpp', default_options:['cpp_std=c++17'])

first_main_src = files(['srcs/check.cpp'])

inc_dir = include_directories('include')

fmt_dep = dependency('fmt')
opencv_dep = dependency('opencv')
project_dep = declare_dependency(include_directories: inc_dir, dependencies:[fmt_dep, opencv_dep], cpp_args:['--fvisibility=default'])

executable('check', sources: first_main_src, dependencies:[project_dep])

subdir('part_one')
subdir('part_two')
