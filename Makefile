OBJ = ChessMain.o ChessBoard.o
EXE = chess
CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)
