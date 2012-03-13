import java.awt.*;
import java.avt.event.*;
import javax.swing.*;

public class Bouncing implements ActionListener {

    JFrame window;
    Canvas canvas;
    JButton button;
    JTextField timeField;

    public Bouncing() {

        window = new JFrame("Bouncing Ball!");
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setLayout(new FlowLayout());
        window.setSize(500,500);

        canvas = new Canvas();
        canvas.setBackground(Color.white);
        canvas.setSize(400, 400);

        window.add(canvas);

        button = new JButton("Click Me");
        window.add(button);

        timeField = new JTextField(4);
        timeField.setText("10");
        window.add(timeField);

        window.setVisible(true);

    }

    public void actionPerformed(ActionEvent e) {
        Graphics g = canvas.getGraphics();
        int duration = 5;

        long startTime = System.currentTimeMillis();

        while(System.currentTimeMillis() - startTime < duration * 1000) {
            ball.draw(g);
            
            try {
                Thread.sleep(15);
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }
    }

    private class Ball {
        double x, y;
        double xvel, yvel;
        private Color ballColor;
        private GRAVITY = 4;
        

        private DIAM = 10; public Ball(double x, double y) {
            this.x = x;
            this.y = y;
            xvel = Math.random()*2 - 4;
            yvel = Math.random()*2 - 4;

            ballColor = new Color(Color.getHSBColor((float)Math.random(),
                                    1.0f, 1.0f);
        }

        public void move() {
            x += xvel;
            y += yvel;
            yvel += GRAVITY;

            int w = canvas.getWidth();
            int h = canvas.getHeight();

            //Make that shit bounce
            //Since we're drawing an oval from a center, it's likely to
            //bounce after it dips below the botton
            if(y <= 0 || y + diam >= h) {
                yvel *= -1.2;
            } 
            if(x <= 0 || x + diam >= w) {
                xvel *= -1.2;
            }
        }

        public void draw(Graphics g) {
            //Fill in old oval with white
            g.setColor(Color.white);
            g.fillOval((int)x, (int)y, DIAM, DIAM);
            //Update position and fill in
            move();
            g.setColor(ballColor);
            g.fillOval((int)x, (int)y, DIAM, DIAM);

        }
    }


    public static void main(String[] args) {
        new Bouncing();
    }

}
