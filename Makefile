# GNU makefile to run all the RISC OS makefiles

# The makefiles are ordered so that they build upon one another - later ones require the earlier ones to have worked.
all:
	riscos-amu -f MakefileGContext,fe1 BUILD32=1
	riscos-amu -f MakefileColours,fe1 BUILD32=1
	riscos-amu -f MakefileImage,fe1 BUILD32=1
	riscos-amu -f MakefileScreen,fe1 BUILD32=1
	riscos-amu -f MakefileBorder,fe1 BUILD32=1
	riscos-amu -f MakefileMD,fe1 BUILD32=1
	riscos-amu -f MakefileTextBlock,fe1 BUILD32=1
	riscos-amu -f MakefileFontFamily,fe1 BUILD32=1
	riscos-amu -f MakefileSlidedeck,fe1 BUILD32=1
	riscos-amu -f MakefileSlideMD,fe1 BUILD32=1
	riscos-amu -f MakefileSlideRender,fe1 BUILD32=1
	riscos-amu -f MakefilePresent,fe1 BUILD32=1

clean:
	-rm -rf o*/* aif*/*

