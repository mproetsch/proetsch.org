import java.util.*;

public class Cave {

    private int row;
    private int column;
    private boolean blocked;
    private boolean marked;
    private boolean occupied;
    private boolean open;
    private boolean pit;
    private boolean teleport;
    private CaveType initial;

    enum CaveType {
        BLOCKED,
        OPEN,
        PIT,
        TELEPORT
    }
    public Cave(int r, int c) {
        row = r;
        column = c;

        initial = CaveType.OPEN;
        blocked = false;
        marked = false;
        occupied = false;
        open = true;
        pit = false;
        teleport = false;
    }

    //Get the column of this cave
    public int getCol() {
        return column;
    }

    //Get the row of this cave
    public int getRow() {
        return row;
    }

    //Get whether or not this cave is blocked
    public boolean isBlocked() {
        return this.blocked;
    }

    //Get whether this cave is marked
    public boolean isMarked() {
        return this.marked;
    }

    //Get whether this cave is occupied
    public boolean isOccupied() {
        return this.occupied;
    }

    //Get whether this cave is open
    public boolean isOpen() {
        return this.open;
    }

    //Get whether this cave is a pit
    public boolean isPit() {
        return this.pit;
    }

    //Get whether this teleports players to a random location
    public boolean isTeleport() {
        return this.teleport;
    }
   
    //Mark this cave as blocked
    public void makeBlocked() {
        this.open = false;
        this.blocked = true;
    }

    //Mark this cave as open
    public void makeOpen() {
        this.pit = false;
        this.blocked = false;
        this.open = true;
    }

    //Mark this cave as a pit
    public void makePit() {
        this.open = true;
        this.pit = true;
    }

    //Mark this cave as a teleport
    public void makeTeleport() {
        this.teleport = true;
    }

    //Set whether this cave is marked
    public void setMarked(boolean set) {
        this.marked = set;
    }

    //Set whether this cave is occupied
    public void setOccupied(boolean set) {
        this.occupied = set;
    }

}
