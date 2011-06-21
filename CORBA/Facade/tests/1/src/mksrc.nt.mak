# $Id: mksrc.nt.mak,v 1.1 2000-08-02 09:07:21 alik Exp $

include src/test1/mktest1.nt.mak

SERVER_OBJS= TestService.obj $(IDL_OBJS) $(TEST1_OBJS)
CLIENT_OBJS= TestClient.obj $(IDL_OBJS)

test1: TestService TestClient

TestService: TestService.exe
TestClient: TestClient.exe

TestService.exe: $(SERVER_OBJS)
	$(LD) $(SERVER_OBJS) $(LDFLAGS)
TestClient.exe: $(CLIENT_OBJS)
	$(LD) $(CLIENT_OBJS) $(LDFLAGS)

TestService.obj: $(TEST1_SRC_DIR)/TestService.cpp
	$(CXX) $(CXXFLAGS) $(TEST1_SRC_DIR)/TestService.cpp
TestClient.obj: $(TEST1_SRC_DIR)/TestClient.cpp
	$(CXX) $(CXXFLAGS) $(TEST1_SRC_DIR)/TestClient.cpp
