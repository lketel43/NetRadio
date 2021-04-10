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
                    int cs = stream.getCurrentStreamed();
                    data = stream.getDiffMess()[current];
                    if(current == Streamer.MAX_NUM_MESS - 1) stream.setCurrent(0);
                    else if(stream.getDiffMess()[current + 1] == null) stream.setCurrent(0);
                    else stream.setCurrent(current + 1);
                    if(cs == 1000){
                        stream.setCurrentStreamed(0);
                        stream.addLast(data, 0);
                    }
                    else if(cs < 1000){
                        stream.addLast(data, cs);
                        stream.setCurrentStreamed(cs + 1);
                    }
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