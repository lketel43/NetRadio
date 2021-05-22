import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.Scanner;

public class JavaClientManager{


    public static void connectToManager(String address, int port){
        try{
            Socket socket = new Socket(address, port);
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
            Scanner scanner = new Scanner(System.in);
            String mess = scanner.nextLine();
            byte[] messB = Message.createMsg(mess);
            writer.print(new String(messB));
            writer.flush();
            String msg;
            while((msg = reader.readLine()) != null) System.out.println(msg);
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    public static void main(String[] args){
        if(args.length == 2){
            connectToManager(args[0], Integer.parseInt(args[1]));
        }
    }

}
