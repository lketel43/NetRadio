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
            Socket socket = new Socket(this.addressManager, this.portManager);
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
            String sendMsg = new String(Message.createMsg("IMOK"));
            while(true){
                String recv = reader.readLine();
                if(recv.equals("RUOK")){
                    writer.print(sendMsg);
                    writer.flush();
                }

            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

}