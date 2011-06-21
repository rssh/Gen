# $Id: mkidl.nt.mak,v 1.1 2000-08-02 09:07:20 alik Exp $

IDLS=Test1
IDL_OBJS=Test1.obj Test1_skel.obj

generate:
	@for %j in ($(IDLS)) do \
		@$(IDL2CXX) $(INCLUDES) $(IDLFLAGS) %j.idl

idl-clean::
	@if exist *.h $(RM) *.h
	@if exist *.cpp $(RM) *.cpp
	@if exist *.obj $(RM) *.obj

Test1.obj: $(TEST1_IDL_DIR)/Test1.cpp
	$(CXX) $(CXXFLAGS) $(TEST1_IDL_DIR)/Test1.cpp
Test1_skel.obj: $(TEST1_IDL_DIR)/Test1_skel.cpp
	$(CXX) $(CXXFLAGS) $(TEST1_IDL_DIR)/Test1_skel.cpp
