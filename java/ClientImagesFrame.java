import javax.swing.*;
import java.awt.*;
import java.awt.Container;
import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;

public class ClientImagesFrame extends JFrame{

    private static final long serialVersionUID = 1000000001;

    public ClientImagesFrame(){
        this.setSize(400, 500);
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setVisible(true);
    }

    public JLabel addImage(byte[] image){
        try{
            ImageIcon im = new ImageIcon(image);
            JLabel l = new JLabel(im);
            this.add(l);
            this.revalidate();
            return l;
        }
        catch(Exception e){
            e.printStackTrace();
            return null;
        }
    }
}