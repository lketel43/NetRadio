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
                byte[] messB;
                synchronized(this.stream){
                    int nbStreamedMess = this.stream.getNbMess();
                    int current = this.stream.getCurrent();
                    int num = this.stream.getNum();
                    String data = this.stream.getMess(current);
                    String mess = "DIFF " + Integer.toString(num) + " " + this.stream.getIDs().get(current) + " " + data;
                    messB = Message.createMsg(mess);
                    if(num + 1 == Streamer.MAX_NUM_MESS) this.stream.setNum(0);
                    else if(num + 1 < Streamer.MAX_NUM_MESS) this.stream.setNum(num + 1);
                    if(current + 1 == nbStreamedMess) this.stream.setCurrent(0);
                    else if (current + 1 < nbStreamedMess) this.stream.setCurrent(current + 1);
                    
                    int lastSend = this.stream.getLastSend();
                    this.stream.addLast(messB, lastSend);
                    if(lastSend + 1 == 1000) this.stream.setLastSend(0);
                    else if(lastSend + 1 < 1000) this.stream.setLastSend(lastSend + 1);
                }
                InetSocketAddress inetSocket = new InetSocketAddress(stream.getMultiCastAddr(), stream.getMultiCastPort());
                DatagramPacket packet = new DatagramPacket(messB, messB.length, inetSocket);
                dataSocket.send(packet);
                Thread.sleep(stream.getFrequency());
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}