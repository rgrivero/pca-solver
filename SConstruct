env = Environment(CPPFLAGS='-Wall')
env.Append(CPPPATH = '/usr/include/boost')

env.Program(target='main', source=['main.cpp', 'pcasolver.cpp', 'sortvector.cpp'])
env.Program(target='test', source=['test.cpp', 'pcasolver.cpp', 'sortvector.cpp'])