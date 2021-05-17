import java.lang.*;
import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.*;

public class ClientReceiveImagesMulticast implements Runnable{

    private String mAddress;
    private int mPort;

    public ClientReceiveImagesMulticast(String _mAddress, int _mPort){
        this.mAddress = _mAddress;
        this.mPort = _mPort;
    }

    public void run(){
        try{
            ClientImagesFrame imFrame = new ClientImagesFrame();
            MulticastSocket mSocket = new MulticastSocket(this.mPort);
            mSocket.joinGroup(InetAddress.getByName(this.mAddress));
            byte[] data = new byte[10000];
            DatagramPacket packet = new DatagramPacket(data,data.length);
            JLabel l = null;
            while(true){
                mSocket.receive(packet);
                if(l != null){
                    imFrame.remove(l);
                }
                byte[] image = packet.getData();
                l = imFrame.addImage(image);
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}