install_subdir('images', install_dir:join_paths(meson.build_root(), 'part_four'))
install_subdir('data', install_dir:join_paths(meson.build_root(), 'part_four'))

dlib_dep = dependency('dlib-1')
meme_main_src = files(['srcs/meme.cpp'])
executable('meme', sources: meme_main_src, dependencies:[project_dep, dlib_dep])
