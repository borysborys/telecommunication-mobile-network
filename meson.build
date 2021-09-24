project('SysrepoListener', ['c', 'cpp'],
  version : '1.0',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

libpcre = dependency('libpcre', required : true)
sysrepo = dependency('sysrepo', required : true)
libyang = dependency ('libyang', required : true)

gtest_proj = subproject('gtest')

gtest_dep = gtest_proj.get_variable('gtest_main_dep')

gmock_dep = gtest_proj.get_variable('gmock_dep')


#gtest_dep = dependency ('gtest', main : true, required : true)     
#gmock_dep = dependency ('gmock', main : false, required : true)     


subdir ('libyangextension/source')
subdir ('libyangcpp/source')
subdir ('libsysrepocpp/source')
subdir ('source')
subdir ('test')


libyangextension = static_library(
    'libyangextension',
    sources : libyangextension_sources_with_path,
    include_directories : [
        libyangextension_includes
    ]
)

libyangextension_dep = declare_dependency (
    link_with :[libyangextension],
    dependencies : libpcre,
    include_directories : [
      libyangextension_includes
    ]
)

libyangcpp = static_library(
    'libyangcpp',
    sources : libyangcpp_sources_with_path,
    include_directories : [
        libyangcpp_includes
    ]
)

libyangcpp_dep = declare_dependency (
    link_with :[libyangcpp],
    dependencies : libyangextension_dep,
    include_directories : [
      libyangcpp_includes
    ]
)


libsysrepocpp = static_library(
    'libsysrepocpp',
    sources : libsysrepocpp_sources_with_path,
    include_directories : [
        libsysrepocpp_includes
    ]
)

libsysrepocpp_dep = declare_dependency (
    link_with :[libsysrepocpp],
    dependencies : libyangcpp_dep,
    include_directories : [
      libsysrepocpp_includes
    ]
)

commutator = static_library(
    'commutator',
    sources : commutator_sources_with_path,
    include_directories : [
        commutator_includes
    ]
)

commutator_dep = declare_dependency (
    link_with :[commutator],
    dependencies : libsysrepocpp_dep,
    include_directories : [
      commutator_includes
    ]
)






#add_project_arduments('LIBS=-ldl', language : 'c')

exe = executable('Listener', 
sources : ['main.cpp'],
dependencies : [sysrepo, commutator_dep],
  install : true,
  link_args: ['-lpthread', '-ldl'],
  cpp_args : '-pthread',
  override_options : ['cpp_std=c++17'])


tests = executable (
    'tests', 
    sources : test_sources_with_path,
    dependencies : [sysrepo, commutator_dep, gtest_dep, gmock_dep],
        install : true,
        link_args: ['-lpthread', '-ldl'],
        cpp_args : '-pthread',
        override_options : ['cpp_std=c++17']
)

test('tests', tests)


