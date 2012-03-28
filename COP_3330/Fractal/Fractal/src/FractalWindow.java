/* Matthew Proetsch
 * COP 3330_0001
 * FractalWindow.java: A window that displays a canvas on which Sierpinski
 * triangles are drawn, as well as options to customize the depthField and colors of
 * them, and to randomize these colors if so requested
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.BoxLayout;

public class FractalWindow implements ActionListener, ItemListener {

    //Declare GUI components
    private JFrame window;
    private Canvas canvas;

    private JPanel superPanel;

    private JPanel depthPanel;
    private JLabel depthLabel;
    public JTextField depthField;

    private JPanel comboBoxesPanel;
    private String[] colors = {"black", "blue", "cyan", "darkGray", "gray", "green",
                                        "lightGray", "magenta", "orange", "pink", "red", "white", "yellow"};
    
    //This array will store random indices into the colors array so that we can
    //draw random colors that are consistent for all polys of equal level
    private int[] rIndices;

    private JComboBox color1;
    private JComboBox color2;
    private JComboBox color3;
    private JComboBox color4;
    private JComboBox color5;

    private JCheckBox randomizeColors;
    //Since we'll be attaching an ItemListener to the JCheckBox
    //randomizeColors, we will be unable to passively check its state using the
    //normal isSelected() method and must keep track of it using a variable
    private boolean rColorsSelected = false;
    private JButton goButton;

    public FractalWindow() {
        
        //Set basic window params
        //Rather than finagle gridlayout to accept uneven column width 
        //(512x512 canvas alongside a combo boxes that aren't nearly that wide) I opt
        //for a flowlayout consisting of the canvas, and a superframe
        //containing the depth textbox frame, combobox frame, and button frame
        window = new JFrame("JSierpinski - Matthew Proetsch");
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setLayout(new FlowLayout(FlowLayout.CENTER, 20, 5));
        window.setSize(830, 550);


        //Get our canvas up and running
        canvas = new Canvas();
        canvas.setBackground(Color.black);
        canvas.setSize(512, 512);
        window.add(canvas);


        //Set up a panel with a FlowLayout to contain our Depth textbox
        depthPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        //Add Depth label
        depthLabel = new JLabel("Depth:", JLabel.LEFT);
        depthPanel.add(depthLabel);
        //Add our depthField text field
        depthField = new JTextField(2); //2 columns
        depthField.setText("0");
        //Add an ActionListener to depthField
        depthField.addActionListener(this);
        depthPanel.add(depthField);
        

        //Set up a GridLayout'd panel to contain our JComboBoxes
        //as well as labels describing them: want 5 rows of two cols
        comboBoxesPanel = new JPanel(new GridLayout(5, 2, 1, 2));

        //don't bother making vars for the labels; no need to track them
        comboBoxesPanel.add(new JLabel("Color 1"));
        color1 = new JComboBox(colors); //then do some repetitive object generation
        comboBoxesPanel.add(color1);    //and some repetitive adding

        comboBoxesPanel.add(new JLabel("Color 2"));
        color2 = new JComboBox(colors);         
        comboBoxesPanel.add(color2);

        comboBoxesPanel.add(new JLabel("Color 3"));
        color3 = new JComboBox(colors);
        comboBoxesPanel.add(color3);

        comboBoxesPanel.add(new JLabel("Color 4"));
        color4 = new JComboBox(colors);
        comboBoxesPanel.add(color4);

        comboBoxesPanel.add(new JLabel("Color 5"));
        color5 = new JComboBox(colors);
        comboBoxesPanel.add(color5);

        JCheckBox randomizeColors = new JCheckBox("Randomize colors at each level");
        randomizeColors.addItemListener(this);
        
        goButton = new JButton("Go");
        goButton.setActionCommand("DRAW");
        goButton.addActionListener(this);

        //Finally add those two panels to our main frame
        //superPanel will contain the right-hand side of the window:
        // - Depth info
        // - JComboBoxes
        // - Randomization checkbox
        // - Go button
        superPanel = new JPanel(new GridLayout(4, 1, 0, 0));
        superPanel.add(depthPanel);
        superPanel.add(comboBoxesPanel);
        superPanel.add(randomizeColors);
        superPanel.add(goButton);
        window.add(superPanel);

        window.setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {

        if(e.getSource() == depthField) {
            //User pressed "Enter" after entering depth
            //Let's check their input but don't run the triangle generator just
            //yet
            checkInput(depthField);
        }

        else if("DRAW".equals(e.getActionCommand())) {
            //Fire up the triangles
            checkInput(depthField);
            if(!depthField.getText().isEmpty())
                draw(Integer.parseInt(depthField.getText()), 0, 0, 512);
        }
        
    }

    //The only ItemEvent that can be thrown here is by our JCheckBox for
    //randomization
    public void itemStateChanged(ItemEvent e) {
        if(e.getStateChange() == ItemEvent.SELECTED) {
            rColorsSelected = true;

            color1.setSelectedIndex((int) (Math.floor(Math.random() * 13)));
            color2.setSelectedIndex((int) (Math.floor(Math.random() * 13)));
            color3.setSelectedIndex((int) (Math.floor(Math.random() * 13)));
            color4.setSelectedIndex((int) (Math.floor(Math.random() * 13)));
            color5.setSelectedIndex((int) (Math.floor(Math.random() * 13)));

            color1.setEnabled(false);
            color2.setEnabled(false);
            color3.setEnabled(false);
            color4.setEnabled(false);
            color5.setEnabled(false);
            
            //fill rIndices with valid random indices into colors[]
            rIndices = new int[5];
            for(int i = 0; i < 5; i++) {
                rIndices[i] = (int) Math.floor(Math.random() * 13);
            }
        }

        else {
            //Ensure randomization of colors is turned off
            rColorsSelected = false;

            color1.setEnabled(true);
            color2.setEnabled(true);
            color3.setEnabled(true);
            color4.setEnabled(true);
            color5.setEnabled(true);
        }
    }

    public void draw(int d, int x, int y, int S) {
        
        //Done drawing, stop.
        if(d == 0) return;

        // Otherwise, draw big triangle at this level, between the points shown
        // in the figure. We will use fillPolygon() method of the Graphics
        // object of our Canvas
        Graphics g = canvas.getGraphics();
        if(d == 1) setColor(g, colors[color1.getSelectedIndex()]);
        if(d == 2) setColor(g, colors[color2.getSelectedIndex()]);
        if(d == 3) setColor(g, colors[color3.getSelectedIndex()]);
        if(d == 4) setColor(g, colors[color4.getSelectedIndex()]);
        if(d == 5) setColor(g, colors[color5.getSelectedIndex()]);

        if(d > 5 && rColorsSelected == true)
            //Get a random color from colors array at index d-6
            //by using index from rIndices[]
            setColor(g, colors[rIndices[d-6]]);
        else if(d > 5 && rColorsSelected == false)
            //Random colors not turned on, so just use the highest level of
            //color
            setColor(g, colors[color5.getSelectedIndex()]);
        

        //fillPolygon expects two arrays, one full of x-values and the other
        //full of corresponding y-values
        //These values were derived from the sheet we were given
        int[] xPts = new int[3];
        xPts[0] = x;
        xPts[1] = x+S/2;
        xPts[2] = x+S;
        int[] yPts = new int[3];
        yPts[0] = y+S;
        yPts[1] = y;
        yPts[2] = y+S;

        g.fillPolygon(xPts, yPts, 3);

        //Draw the polys that should go inside this one
        draw(d-1, x+S/4, y, S/2);
        draw(d-1, x, y+S/2, S/2);
        draw(d-1, x+S/2, y+S/2, S/2);
    }


    //Set the color of graphics object g to the Color.<color> that corresponds
    //to string sColor
    public void setColor(Graphics g, String sColor) {
        if(sColor.equals("black"))
            g.setColor(Color.black);
        if(sColor.equals("blue"))
            g.setColor(Color.blue);
        if(sColor.equals("cyan"))
            g.setColor(Color.cyan);
        if(sColor.equals("darkGray"))
            g.setColor(Color.darkGray);
        if(sColor.equals("gray"))
            g.setColor(Color.gray);
        if(sColor.equals("green"))
            g.setColor(Color.green);
        if(sColor.equals("lightGray"))
            g.setColor(Color.lightGray);
        if(sColor.equals("magenta"))
            g.setColor(Color.magenta);
        if(sColor.equals("orange"))
            g.setColor(Color.orange);
        if(sColor.equals("pink"))
            g.setColor(Color.pink);
        if(sColor.equals("red"))
            g.setColor(Color.red);
        if(sColor.equals("white"))
            g.setColor(Color.white);
        if(sColor.equals("yellow"))
            g.setColor(Color.yellow);
       
    }


    //Ensure that input from depthField is valid (less than 10 and only
    //numeric)
    public void checkInput(JTextField field) {
        String contents = field.getText();
        try {
            int sierpinskyDepth = Integer.parseInt(contents);
            if(sierpinskyDepth > 10 || sierpinskyDepth < 0) {
                JOptionPane.showMessageDialog(window,
                        "Enter a value between 0 and 10");
                field.setText("");
            }

        } catch(NumberFormatException ex) {
            JOptionPane.showMessageDialog(window,
                    "Only integer values are accepted");
            field.setText("");
        }
    }
        
    public static void main(String[] args) {
        //Instantiate our hard-earned class
        new FractalWindow();
    }
}
