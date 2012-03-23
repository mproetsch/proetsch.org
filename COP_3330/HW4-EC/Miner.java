public class Miner extends Character {

    //Make a new Miner by calling superclass constructor
    public Miner(Cave initLoc) {
        super(initLoc);
    
    }

    //Describe the modifications a Miner makes to a Cave
    public String describeModification() {
        return "Opened up a Cave";

    }

    //Return the name of this Miner
    public String getName() {
        return "Miner";
    }

    //Attempt to open up Cave "loc"
    public boolean modifyCave(Cave loc) {
        if(loc.isBlocked()) {
            loc.makeOpen();
            return true;

        }
        return false;
    }

    //Return true if Miner can move to Cave "To"
    //False otherwise
    @Override
    public boolean move(Cave to) {
        //"to" should be unoccupied and either open or blocked
        if(!to.isOccupied() &&
           (to.isOpen() || to.isBlocked())) {

               return super.move(to);
        }
        
        return false;
    }
}
