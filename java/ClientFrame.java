import javax.swing.*;
import java.awt.*;

public class ClientFrame extends JFrame{

    private static final long serialVersionUID = 1000000000;

    private JTextArea messages = new JTextArea("");

    public ClientFrame(){
        this.setSize(400, 500);
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.add(messages);
        final JScrollPane comp = new JScrollPane(this.messages);
        comp.setVerticalScrollBarPolicy(22);
        comp.setPreferredSize(new Dimension(250, 250));
        this.add(comp);
        this.setVisible(true);
    }

    public void addMess(String mess){
        this.messages.insert(mess, 0);
    }

}