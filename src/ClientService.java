import java.lang.*;
import java.io.*;
import java.net.*;

public class ClientService implements Runnable{

    private Streamer stream;
    private Socket socket;

    public ClientService(Streamer _stream, Socket _socket){
        this.stream = _stream;
        this.socket = _socket;
    }

    public void run(){
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(this.socket.getOutputStream()));
            String mess = reader.readLine();
            byte[] messB = Message.createMsg(mess);
            Message.MsgType type = Message.getType(messB);
            switch(type){
                case LAST :
                    synchronized(this.stream){
                        byte[][] last = stream.getLastMess(Message.getNbMsg(messB));
                        for(int i = 0; i < last.length; i++){
                            writer.print(new String(Message.DiffToOldm(last[i])));
                            writer.flush();
                        }
                        byte[] end = Message.createMsg("ENDM");
                        writer.print(new String(end));
                        writer.flush();
                        reader.close();
                        writer.close();
                        socket.close();
                        break;
                    }
                case MESS :
                    synchronized(this.stream){
                        String num = Integer.toString(this.stream.getLastMess() + 1);
                        String id = Message.getID(messB);
                        String msg = Message.getMsg(messB);
                        String newMess = "DIFF " + num + " " + id + " " + msg;
                        byte[] newMessB = Message.createMsg(newMess);
                        this.stream.addMess(newMessB);
                        byte[] end2 = Message.createMsg("ACKM");
                        writer.print(new String(end2));
                        writer.flush();
                        reader.close();
                        writer.close();
                        socket.close();
                        break;
                    }
                default : return;
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}