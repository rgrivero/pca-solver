# vim : set filetype=python :

env = Environment(CPPFLAGS='-Wall')
env.Append(CPPPATH = '/usr/include/boost')

env.Program(target='sample', source=['sample.cpp', 'pcasolver.cpp', 'sortvector.cpp'])
env.Program(target='test', source=['test.cpp', 'pcasolver.cpp', 'sortvector.cpp'])
