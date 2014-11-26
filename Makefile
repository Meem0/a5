CXX = g++
CXXFLAGS = -Wall -MMD
EXEC = ss5k
OBJECTS = Board.o BoardManip.o DebugDisplay.o BoardDisplay.o EmptySquare.o GraphicalDisplay.o LateralSquare.o Level.o Level0.o Level1.o Level2.o LevelTest.o Pos.o PsychSquare.o Score.o Square.o ss5k.o TextDisplay.o UnstableSquare.o UprightSquare.o window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
