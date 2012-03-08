public abstract class Character implements CaveWorker {

    protected Cave location;

    //Construct a new Character at a given cave
    public Character(Cave initLoc) {
        
        location = initLoc;
        initLoc.setOccupied(true);
    }
    
    //Get the character's current cave
    public Cave getLocation() {
        return this.location;
    }

    //Move the character to the given cave
    public boolean move(Cave to) {
        location.setOccupied(false);
        location = to;
        location.setOccupied(true);

        return true;
    }

    //Return a string representation of this name
    //Should be implemented by non-abstract classes extending Character
    public abstract String getName();

}

