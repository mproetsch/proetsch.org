public class Filler extends Character {
   
    //Make a new Filler by calling superclass constructor
    public Filler(Cave initLoc) {
        super(initLoc);
    }

    //Return a string describing modifications made to a Cave
    public String describeModification() {
        return "Cave filled in!";
    }

    //Return the name of this Filler
    public String getName() {
        return "Filler";
    }
   
    //Attempt to modify Cave "loc"
    //Return true on success, false on failure
    public boolean modifyCave(Cave loc) {
        if(loc.isPit()) {
            loc.makeOpen();
            return true;
        }

        return false;
    }

    //Check to see if this Filler can move to the requested location
    @Override
    public boolean move(Cave to) {
        if(to.isOpen() &&
           !to.isOccupied() ) {
            return super.move(to);
        }

        return false;
    }
}
