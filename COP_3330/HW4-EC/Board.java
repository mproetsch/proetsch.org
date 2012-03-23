import java.util.Random;

public class Board {

    private final static int MAX_ROWS = 10;
    private final static int MAX_COLS = 10;

    Cave[][] gameBoard = new Cave[MAX_ROWS][MAX_COLS];

    //Generate a random board of size rows x cols
    //Place Adventurer in top-left, treasure in bottom-right
    public Board(int rows, int cols) {
        
        for(int i = 0; i < MAX_ROWS; i++) {
            for(int j = 0; j < MAX_COLS; j++) {

                double rand = Math.random() * 100;
                gameBoard[i][j] = new Cave(i, j);

                if(rand < 10) {
                    gameBoard[i][j].makeOpen();
                } else if(rand >= 10 && rand < 50) {
                    gameBoard[i][j].makeBlocked();
                } else if(rand >= 50 && rand < 90) {
                    gameBoard[i][j].makePit();
                } else if(rand >= 90) {
                    gameBoard[i][j].makeTeleport();
                }


            }
        }

        gameBoard[0][0].makeOpen();
        gameBoard[MAX_ROWS-1][MAX_COLS-1].makeOpen();
    }

    //Get the Cave at row r, column c
    public Cave getCave(int r, int c) {
        if(ok(r, c)) {
            return gameBoard[r][c];
        }
        return null;
    }

    //Check if the location is inside the bounds of the board
    public boolean ok(int r, int c) {
        if(r < MAX_ROWS &&
           c < MAX_COLS ) {
            return true;
        }

        return false;
    }

    //Get a random unoccupied open location from the board
    public Cave getUnoccupiedOpenLocation() {
        
        while(true) {

            //Conveniently, Math.random() will return a number between 0 and 9
            //(inclusive) when multiplied by 10 and typecast to an int
            int randRow = (int) (Math.random() * 10);
            int randCol = (int) (Math.random() * 10);

            //Ensure we aren't dealing with the treasure location
            if(randRow == (MAX_ROWS - 1) &&
               randCol == (MAX_COLS - 1)) {
                   continue;
               }

            if(ok(randRow, randCol)) {
                Cave c = getCave(randRow, randCol);

                if( c.isOpen() && !c.isOccupied() ) {
                    return c;
            
                } else {
                    continue;
                }
            }
        }
    }
}
