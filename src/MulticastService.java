import java.lang.*;
import java.io.*;
import java.net.*;

public class MulticastService implements Runnable{

    private Streamer stream;

    public MulticastService(Streamer _stream){
        this.stream = _stream;
    }

    public void run(){
        try{
            DatagramSocket dataSocket = new DatagramSocket();
            while(true){
                byte[] data;
                synchronized(this.stream){
                    int current = stream.getCurrent();
                    data = stream.getDiffMess()[current];
                    if(current == Streamer.MAX_NUM_MESS) stream.setCurrent(0);
                    else if(stream.getDiffMess()[current + 1] == null) stream.setCurrent(0);
                    else stream.setCurrent(current + 1);
                }
                InetSocketAddress inetSocket = new InetSocketAddress(stream.getMultiCastAddr(), stream.getMultiCastPort());
                DatagramPacket packet = new DatagramPacket(data, data.length, inetSocket);
                dataSocket.send(packet);
                Thread.sleep(stream.getFrequency());
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}