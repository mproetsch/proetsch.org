/* Matthew Proetsch
 * COP 3330_0001
 * FractalWindow.java: A window that displays a canvas on which Sierpinski
 * triangles are drawn, as well as options to customize the depthField and colors of
 * them
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
    private String[] colors = {"White", "Black", "Red", "Orange", "Yellow", "Green", "Blue", "Magenta"};
    private JComboBox color1;
    private JComboBox color2;
    private JComboBox color3;
    private JComboBox color4;
    private JComboBox color5;

    private JCheckBox randomizeColors;
    private JButton goButton;

    public FractalWindow() {
        //Set basic window params
        //Rather than finagle gridlayout to accept uneven column width 
        //(512x512 canvas alongside a combo boxes that aren't nearly that wide) I opt
        //for a flowlayout consisting of the canvas, and a superframe
        //containing the depth textbox frame, combobox frame, and button frame
        window = new JFrame("JSierpinski v0.0.0.0.1pre-alpha");
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setLayout(new FlowLayout(FlowLayout.CENTER, 20, 5));
        window.setSize(733, 550);

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
        
        //Set up a BoxLayout'd panel to contain our JComboBoxes
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

        JCheckBox randomizeColors = new JCheckBox("Randomize colors?");
        randomizeColors.addItemListener(this);
        
        goButton = new JButton("Go");
        goButton.setActionCommand("DRAW");
        goButton.addActionListener(this);

        //Finally add those two panels to our main frame
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
            color1.setSelectedIndex((int) (Math.floor((Math.random() * 8))));
            color2.setSelectedIndex((int) (Math.floor((Math.random() * 8))));
            color3.setSelectedIndex((int) (Math.floor((Math.random() * 8))));
            color4.setSelectedIndex((int) (Math.floor((Math.random() * 8))));
            color5.setSelectedIndex((int) (Math.floor((Math.random() * 8))));
            color1.setEnabled(false);
            color2.setEnabled(false);
            color3.setEnabled(false);
            color4.setEnabled(false);
            color5.setEnabled(false);
        }
        else {
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
        // in the figure. We will use fillPolygon() method of the Graphics)
        // object of our Canvas
        //
        // MY_CODE_HERE
        //
        Graphics g = canvas.getGraphics();
        if(d == 1) setColor(g, color1);
        if(d == 2) setColor(g, color2);
        if(d == 3) setColor(g, color3);
        if(d == 4) setColor(g, color4);
        if(d == 5) setColor(g, color5);

        //int[] xPts = new int[(int)(Math.pow(3, S))];
        //int[] yPts = new int[(int)(Math.pow(3, S))];
        
        //for(int i = 0; i < (int)Math.pow(3, S); i++) {
            //xPts[i] = 
        
        int[] xa = new int[3];
        xa[0] = x;
        xa[1] = x+S/2;
        xa[2] = x+S;
        int[] ya = new int[3];
        ya[0] = y+S;
        ya[1] = y;
        ya[2] = y+S;
        g.fillPolygon(xa, ya, 3);

        draw(d-1, x+S/4, y, S/2);
        draw(d-1, x, y+S/2, S/2);
        draw(d-1, x+S/2, y+S/2, S/2);
    }

    public void setColor(Graphics g, JComboBox combo) {
        if(combo.getSelectedItem().equals("White"))
            g.setColor(Color.white);
        if(combo.getSelectedItem().equals("Black"))
            g.setColor(Color.black);
        if(combo.getSelectedItem().equals("Red"))
            g.setColor(Color.red);
        if(combo.getSelectedItem().equals("Orange"))
            g.setColor(Color.orange);
        if(combo.getSelectedItem().equals("Yellow"))
            g.setColor(Color.yellow);
        if(combo.getSelectedItem().equals("Green"))
            g.setColor(Color.green);
        if(combo.getSelectedItem().equals("Blue"))
            g.setColor(Color.blue);
        if(combo.getSelectedItem().equals("Magenta"))
            g.setColor(Color.magenta);
        if(combo.getSelectedItem().equals("Black"))
            g.setColor(Color.black);

    }


    public void checkInput(JTextField field) {
        String contents = field.getText();
        try {
            int sierpinskyDepth = Integer.parseInt(contents);
            if(sierpinskyDepth > 10) {
                JOptionPane.showMessageDialog(window,
                        "Enter a value between 1 and 10");
                field.setText("");
            }

        } catch(NumberFormatException ex) {
            JOptionPane.showMessageDialog(window,
                    "Only integer values are accepted");
            field.setText("");
        }
    }
        
    public static void main(String[] args) {
        new FractalWindow();
    }
}
