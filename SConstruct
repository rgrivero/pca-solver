# vim : set filetype=python :

env = Environment(CPPFLAGS='-Wall')
env.Append(CPPPATH = '/usr/include/boost')

def runUnitTest(env,target,source):
    import subprocess
    app = str(source[0].abspath)
    if not subprocess.call(app):
        open(str(target[0]),'w').write("PASSED\n")

env.Program(target='pca-solver', source=['sample.cpp', 'pcasolver.cpp', 'sortvector.cpp'])

env.Program(target='test', source=['test.cpp', 'pcasolver.cpp', 'sortvector.cpp'])
Command("test.passed",'test',runUnitTest)

