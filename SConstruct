env = Environment()
env.Append(LINKFLAGS = ['-lOpenThreads', '-losg', '-losgDB', '-losgUtil', '-losgGA', '-losgViewer', '-losgShadow', '-losgWidget', '-larmadillo'])
env.Program(
	target = 'PrtDemo',
	source = ['Recording.cpp', 'Math/ShLight.cpp'],
)
