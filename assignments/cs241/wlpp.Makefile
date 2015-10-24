.SUFFIXES: .asm .hex .merl .mips .wlpp

.asm.merl:
	java cs241.wordasm < $^ > $@

.asm.mips:
	java cs241.binasm < $^ > $@

.hex.mips:
	java cs241.wordasm < $^ > $@

.wlpp.mips:
	java cs241.wlpp.wlppc < $^ > $@
