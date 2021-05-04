import java.io.*;
import java.net.*;

public class ClientReceiveMulticast implements Runnable{

    private String mAddress;
    private int mPort;

    public ClientReceiveMulticast(String _mAddress, int _mPort){
        this.mAddress = _mAddress;
        this.mPort = _mPort;
    }

    public void run(){
        try{
            ClientFrame frame = new ClientFrame();
            MulticastSocket mSocket = new MulticastSocket(this.mPort);
            mSocket.joinGroup(InetAddress.getByName(this.mAddress));
            byte[] data = new byte[161];
            DatagramPacket packet = new DatagramPacket(data,data.length);
            while(true){
                mSocket.receive(packet);
                String st = new String(packet.getData(), 0, packet.getLength());
                frame.addMess(st);
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    } 

}