import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.Scanner;

public class JavaClient{


    public static void client(String multiCastAddr, int multiCastPort, String address, int port){
        ClientReceiveMulticast recv = new ClientReceiveMulticast(multiCastAddr, multiCastPort);
        Thread thread = new Thread(recv);
        thread.start();
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

        if(args.length == 0){
            client("225.0.0.0", 5001, "localhost", 4242);
        }
        else if(args.length == 4){
            client(args[0], Integer.parseInt(args[1]), args[2], Integer.parseInt(args[3]));
        }
        else System.out.println("Wrong options");

    }

}