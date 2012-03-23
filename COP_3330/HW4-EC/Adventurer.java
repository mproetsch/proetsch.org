public class Adventurer extends Character {

    //Make a new Adventurer by calling superclass constructor
    public Adventurer(Cave initLoc) {
        super(initLoc);
    }

    //Describe modification made to a Cave by an Adventurer
    public String describeModification() {
        return "Adventurer has marked a teleport location";
    }

    //Attempt to modify Cave "loc", return true on success/false otherwise
    public boolean modifyCave(Cave loc) {

        if(!loc.isMarked() && loc.isTeleport() ) {
            loc.setMarked(true);

            return true;
        }
        return false;
    }

    //Return the name of this adventurer
    public String getName() {
        return "Adventurer";
    }

    //Check to see if Adventurer can move to Cave "to"
    @Override
    public boolean move(Cave to) {
        if(to.isOpen() &&
           !to.isOccupied() ) {
            super.move(to);
            return true;

        } else {
            return false;
        }
    }

}
