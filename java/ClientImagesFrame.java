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

    public static void main(String[] args){
        try{
        BufferedImage image = ImageIO.read(new File("./cr.png"));
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        ImageIO.write(image, "png", baos);
        baos.flush();
        byte[] buffer = baos.toByteArray();
        System.out.println(buffer.length);
        ClientImagesFrame frame = new ClientImagesFrame();
        frame.addImage(buffer);
        }
        catch(Exception e){}
        //JLabel l = new JLabel(new ImageIcon("./im.png"));
        //frame.add(l);
        //Container parent = l.getParent(l);
        //frame.remove(l);
        //frame.validate();
        //frame.repaint();
    }

}