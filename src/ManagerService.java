import java.io.*;
import java.net.*;

public class ManagerService implements Runnable{

    private Streamer stream;
    private int portManager;
    private String addressManager;

    public ManagerService(Streamer _stream, int _portManager, String _addressManager){
        this.stream = _stream;
        this.portManager = _portManager;
        this.addressManager = _addressManager;
    }

    public void run(){
        try{
            String addr = Streamer.getStreamerAddress();
            String mess = "REGI " + this.stream.getId() + " " + this.stream.getMultiCastAddr() + " " + this.stream.getMultiCastPort() + " " + addr + " " + this.stream.getRecvPort();
            byte[] messB = Message.createMsg(mess);
            Socket socket = new Socket(this.addressManager, this.portManager);
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
            writer.print(new String(messB));
            writer.flush();
            String str = reader.readLine();
            //System.out.println(str);
            if(str.equals("REOK")){

                String sendMsg = new String(Message.createMsg("IMOK"));
                while(true){
                    String recv = reader.readLine();
                    System.out.println(recv);
                    if(recv.equals("RUOK")){
                        writer.print(sendMsg);
                        writer.flush();
                    }
                }

            }
            else if(str.equals("RENO")){
                socket.close();
                reader.close();
                writer.close();
            }
            else System.out.println("ERROR");
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}