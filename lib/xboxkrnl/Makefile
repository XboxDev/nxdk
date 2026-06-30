$(NXDK_DIR)/lib/libxboxkrnl.lib: $(NXDK_DIR)/lib/xboxkrnl/xboxkrnl.exe.def
	@echo "[ DLLTOOL  ] $@"
	$(VE) llvm-dlltool -m i386 -l '$@' -k -d '$<'

CLEANRULES += clean-xboxkrnl

.PHONY: clean-xboxkrnl
clean-xboxkrnl:
	$(VE) rm -f $(NXDK_DIR)/lib/libxboxkrnl.lib
