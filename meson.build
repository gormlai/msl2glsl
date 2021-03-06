project('Metal Parser', 'cpp')

# The point of this test is that one generator
# may output headers that are necessary to build
# the sources of a different generator.

flex = find_program('flex', required: false)
bison = find_program('bison', required: false)

if not flex.found()
  error('MESON_SKIP_TEST flex not found.')
endif

if not bison.found()
  error('MESON_SKIP_TEST bison not found.')
endif

lgen = generator(flex,
output : '@PLAINNAME@.yy.cpp',
arguments : ['-o', '@OUTPUT@', '@INPUT@'])

lfiles = lgen.process('metal.ll')

pgen = generator(bison,
output : ['@BASENAME@.tab.cpp', '@BASENAME@.tab.h'],
arguments : ['-r','all','-t','@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])

pfiles = pgen.process('metal.yy')

sources = [
  'Ast2GL.cpp',
  'Driver.cpp',
  'main.cpp',
  'PrettyPrinter.cpp',
  'Scanner.cpp',
  'ShaderCollector.cpp',
  'Transpiler.cpp',
  'Types.cpp',
  'Visitor.cpp',
  lfiles, pfiles]

add_global_arguments(['-std=c++14','-g','-Wall','-Wextra'], language : 'cpp')
e = executable('metalparser', sources, )

test('parsertest', e)
