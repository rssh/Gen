# $Id: mktest1.nt.mak,v 1.1 2000-08-02 09:07:23 alik Exp $

TEST1_OBJS=Test1_impl.obj TestServer.obj

Test1_impl.obj: $(TEST1_SRC_DIR)/test1/Test1_impl.cpp
	$(CXX) $(CXXFLAGS) $(TEST1_SRC_DIR)/test1/Test1_impl.cpp
TestServer.obj: $(TEST1_SRC_DIR)/test1/TestServer.cpp 
	$(CXX) $(CXXFLAGS) $(TEST1_SRC_DIR)/test1/TestServer.cpp 
