import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

/**
 * GUI class to drive the game and display it to the user. Do not make any modifications to this
 * class unless you are working on the extra credit part of this assignment (remember to first turn
 * in the original assignment though!).
 * 
 * @author Stephen Fulwider
 */
public class AdventureGame implements KeyListener,MouseListener {
	
	public static final int DEFAULT_ROWS = 10;
	public static final int DEFAULT_COLS = 10;
	
	private JFrame frame;
	private ImageLabel[][] grid;
	private JLabel statusBar;
	
	private Board gameBoard; // Underlying board game.
	private List<Character> characters; // Characters on the board.
	private int selected; // Which character is currently selected.
	
	private boolean treasureClaimed;
	
	/** Start the first game. */
	public AdventureGame() {
		buildGui();
		newGame();
		updateGameBoard();
	}
	
	/** Build the initial GUI of the game board. */
	private void buildGui() {
		// Make the frame.
		frame = new JFrame("Adventurer Assistance, Inc.");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new BorderLayout());
		
		// Build the grid.
		grid = new ImageLabel[DEFAULT_ROWS][DEFAULT_COLS];
		JPanel gridPanel = new JPanel(new GridLayout(DEFAULT_ROWS,DEFAULT_COLS));
		for (int i=0; i<grid.length; ++i)
			for (int j=0; j<grid[i].length; ++j) {
				grid[i][j]=new ImageLabel("icons64/pit.png",i,j);
				grid[i][j].addMouseListener(this);
				gridPanel.add(grid[i][j]);
			}
		frame.add(gridPanel,BorderLayout.CENTER);
		
		// Add the status bar.
		statusBar = new JLabel();
		statusBar.setBorder(BorderFactory.createLoweredBevelBorder());
		frame.add(statusBar,BorderLayout.SOUTH);
		
		// Add the listener for key strokes.
		frame.addKeyListener(this);
		frame.setFocusTraversalKeysEnabled(false);
		
		// Make it visible.
		frame.setVisible(true);
		frame.pack();
	}
	
	/** Initialize a new underlying game. */
	private void newGame() {
		// Set up the game board.
		gameBoard = new Board(DEFAULT_ROWS, DEFAULT_COLS);
		
		// Set up the 3 characters.
		characters = new ArrayList<Character>();
		
		// Add the adventurer (always in the top left).
		characters.add(new Adventurer(gameBoard.getCave(0, 0)));
		selected = 0; // Initially select the adventurer.
		
		// Get a location for the miner and add him.
		Cave minerLoc = gameBoard.getUnoccupiedOpenLocation();
		characters.add(new Miner(gameBoard.getCave(minerLoc.getRow(), minerLoc.getCol())));
		
		// Get a location for the filler and add him.
		Cave fillerLoc = gameBoard.getUnoccupiedOpenLocation();
		characters.add(new Filler(gameBoard.getCave(fillerLoc.getRow(), fillerLoc.getCol())));
		
		// We seek the treasure!
		treasureClaimed = false;
		
		updateStatus("Welcome! Select characters with the mouse (or just use TAB) " +
				"and use the arrow keys to move.");
	}
	
	/** Update the visible state of the game board based on the internal state of the game. */
	private void updateGameBoard() {
		
		// Put icons on the board.
		for (int i=0; i<grid.length; ++i)
			for (int j=0; j<grid[i].length; ++j) {
				Cave c = gameBoard.getCave(i, j);
				String img = "icons64/";
				if (c.isBlocked())
					img+="cave.png";
				else if (c.isPit())
					img+="pit.png";
				else if (c.isTeleport() && c.isMarked())
					img+="portal.png";
				else // open OR teleport and not marked
					img+="ground.png";
				grid[i][j].setIcon(img);
				grid[i][j].setBorder(BorderFactory.createBevelBorder(1));
			}
		
		// Show the characters. Highlight the selected one.
		int idx=0;
		for (Character ch : characters) {
			Cave c = ch.getLocation();
			if (ch instanceof Adventurer)
				grid[c.getRow()][c.getCol()].setIcon("icons64/hat.png");
			else if (ch instanceof Miner)
				grid[c.getRow()][c.getCol()].setIcon("icons64/pickaxe.png");
			else if (ch instanceof Filler)
				grid[c.getRow()][c.getCol()].setIcon("icons64/wheelbarrow.png");
			if (idx++==selected) {
				grid[c.getRow()][c.getCol()].
					setBorder(BorderFactory.createBevelBorder(1, Color.red, Color.red));
			}
		}
		
		// Show the treasure if it's not already claimed.
		if (!treasureClaimed)
			grid[grid.length-1][grid[0].length-1].setIcon("icons64/treasure.png");
	}
	
	/** Show the given message on the status bar. */
	private void updateStatus(String msg) {
		statusBar.setText(msg);
	}
	
	/** Handle user input from keyboard. */
	public void keyPressed(KeyEvent e) {
		
		// Check if this is a tab event to move characters.
		if (e.getKeyCode()==KeyEvent.VK_TAB) {
			selected=(selected+1)%characters.size();
			updateStatus("You have selected " + characters.get(selected).getName());
			updateGameBoard();
			return;
		}
		
		// Check for F2 (new game)
		if (e.getKeyCode()==KeyEvent.VK_F2) {
			newGame();
			updateGameBoard();
			return;
		}
		
		// Only move if the selected character is valid.
		if (selected<0 || selected>=characters.size()) {
			return;
		}
		
		// Get the direction of movement.
		int dr=0,dc=0;
		if (e.getKeyCode()==KeyEvent.VK_UP) {
			dr=-1;
		}
		else if (e.getKeyCode()==KeyEvent.VK_DOWN) {
			dr=1;
		}
		else if (e.getKeyCode()==KeyEvent.VK_LEFT) {
			dc=-1;
		}
		else if (e.getKeyCode()==KeyEvent.VK_RIGHT) {
			dc=1;
		}
		else { // non-recognized key event
			return;
		}
		
		// Make the move.
		Character ch = characters.get(selected);
		Cave c = ch.getLocation();
		if (gameBoard.ok(c.getRow()+dr,c.getCol()+dc)) {
			Cave newC = gameBoard.getCave(c.getRow()+dr, c.getCol()+dc);
			
			// Make sure only the adventurer moves to the treasure.
			if (newC.getRow()==DEFAULT_ROWS-1 && newC.getCol()==DEFAULT_COLS-1 &&
					!(ch instanceof Adventurer)) {
				updateStatus("Only the adventurer can claim the treasure!");
			}
			
			// Try and make the move.
			else if (ch.move(newC)) {
				
				// Try and modify this cave is possible.
				CaveWorker cw = ch;
				if (cw.modifyCave(newC)) {
					updateGameBoard();
					updateStatus(ch.getName()+" successfully moved and "+
							cw.describeModification()+"!");
				}
				else if (newC.isPit()) { // This character falls in the pit and dies.
					updateStatus(ch.getName()+" fell in the pit and died!");
					newC.setOccupied(false);
					characters.remove(selected);
					selected%=characters.size();
					updateGameBoard();
					if (ch instanceof Adventurer) {
						if (!treasureClaimed) {
							JOptionPane.showMessageDialog(frame, ch.getName()+" is now dead :( " +
									"No way to get the treausre now. Better luck next time!");
						}
						else {
							JOptionPane.showMessageDialog(frame, ch.getName()+" is now dead :( " +
									"She fell in the pit and took all the treasure with her!");
						}
					}
				}
				else if (newC.isTeleport()) { // Transport this character to a random location.
					Cave randomLoc = gameBoard.getUnoccupiedOpenLocation();
					ch.move(randomLoc); // Guaranteed to return true.
					updateGameBoard();
					updateStatus(ch.getName()+" was teleported to a mystery cave!");
				}
				else {
					updateStatus(ch.getName()+" successfully moved!");
					updateGameBoard();
				}
				
				if (!treasureClaimed && newC.getRow()==DEFAULT_ROWS-1 && 
						newC.getCol()==DEFAULT_COLS-1 && ch instanceof Adventurer) {
					treasureClaimed = true;
					updateGameBoard();
					JOptionPane.showMessageDialog(frame, ch.getName()+" has claimed the treasure!" +
							" Fame and fortune are now yours!");
					updateStatus("Keep exploring if you desire!");
				}
			}
			else if (newC.isBlocked()) { // No move can be made.
				updateStatus(ch.getName()+" isn't a miner so can't fill in blocked caves!");
			}
			else {
				updateStatus("Only one character can occupy a cave");
			}
			
		}
		else { // This location is off the board
			updateStatus("No character can leave the caves! Get back to work!");
		}
			
	}

	/** Handle input from user with a mouse. */
	public void mouseClicked(MouseEvent e) {
		if (e.getComponent() instanceof ImageLabel) {
			
			// Get which ImageLabel was clicked.
			ImageLabel il = (ImageLabel)e.getComponent();
			
			// Get its row & column.
			int row = il.r, col = il.c;
			
			// See if any characters on the grid are at this spot.
			int idx=0;
			for (Character ch : characters) {
				Cave c = ch.getLocation();
				if (c.getRow()==row && c.getCol()==col) {
					selected=idx;
					updateGameBoard();
					updateStatus("You have selected " + ch.getName());
					return;
				}
				++idx;
			}
		}
		
	}
	
	// Don't need to do anything with these, but must implement per the KeyListener interface.
	public void keyReleased(KeyEvent e) {}
	public void keyTyped(KeyEvent e) {}
	
	// Don't need to do anything with these, but must implement per the MouseListener interface.
	public void mousePressed(MouseEvent e) {}
	public void mouseReleased(MouseEvent e) {}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}

	public static void main(String[] args) {
		new AdventureGame();
	}

	
	/** Helper class for an Image label that displays an icon instead of text. */
	private static class ImageLabel extends JLabel {
		
		static TreeMap<String,ImageIcon> M=new TreeMap<String,ImageIcon>();
		private int r,c;

		public ImageLabel(String img, int r, int c) {
			this(new ImageIcon(img),r,c);
		}

		public ImageLabel(ImageIcon icon, int r, int c) {
			setIcon(icon);
//			setSize(icon.getImage().getWidth(null), icon.getImage().getHeight(null));
			this.r=r;
			this.c=c;
		}
		
		public void setIcon(String img) {
			if (!M.containsKey(img))
				M.put(img, new ImageIcon(img));
			setIcon(M.get(img));
		}

	}

}
